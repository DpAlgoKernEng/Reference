# Arc<Mutex> - Rust 线程安全共享状态

## 1. 概述

`Arc<Mutex>` 是 Rust 中实现线程安全共享状态的经典组合。`Arc`（Atomic Reference Counting）提供多线程共享所有权，`Mutex` 提供内部可变性（interior mutability）和互斥访问。

定义在 `std::sync` 模块中，是 Rust 并发编程的核心工具。

与其他语言对比：
- 类似 Java 的 `synchronized` + 共享引用
- 类似 Go 的 `sync.Mutex` + 指针共享
- Rust 的优势：编译时保证线程安全

## 2. 来源与演变

### Rust 1.0 引入

`Arc` 和 `Mutex` 在 **Rust 1.0**（2015 年）中就已存在，是标准库的基础组件。

### 设计动机

- Rust 的所有权系统默认禁止跨线程共享可变状态
- 需要一种安全的机制实现共享状态并发
- 通过 `Arc` 共享所有权 + `Mutex` 控制访问，实现安全的共享状态

### 主要版本变更

| 版本 | 变更 |
|------|------|
| Rust 1.0 | 首次引入 Arc、Mutex |
| Rust 1.63+ | Mutex::new 成为 const fn |

## 3. 语法与参数

### Arc 定义

```rust
use std::sync::Arc;

// 创建 Arc
let arc = Arc::new(value);

// 克隆（增加引用计数）
let arc2 = Arc::clone(&arc);

// 获取强引用计数
let count = Arc::strong_count(&arc);

// 获取弱引用
let weak = Arc::downgrade(&arc);
```

### Mutex 定义

```rust
use std::sync::Mutex;

// 创建 Mutex
let mutex = Mutex::new(value);

// 获取锁
let guard = mutex.lock().unwrap();

// 尝试获取锁
let result = mutex.try_lock();

// 访问数据
*guard = new_value;
```

### 组合使用

```rust
use std::sync::{Arc, Mutex};

// 线程安全的共享状态
let shared = Arc::new(Mutex::new(data));

// 多线程共享
let shared2 = Arc::clone(&shared);
```

### 主要方法

| 类型 | 方法 | 说明 |
|------|------|------|
| `Arc` | `new(value)` | 创建 Arc |
| `Arc` | `clone(&arc)` | 克隆（增加引用计数） |
| `Arc` | `strong_count(&arc)` | 获取强引用计数 |
| `Mutex` | `lock()` | 获取锁（阻塞） |
| `Mutex` | `try_lock()` | 尝试获取锁（非阻塞） |
| `Mutex` | `into_inner()` | 消费 Mutex，获取内部值 |

## 4. 底层原理

### Arc 内存布局

```
Arc 内部结构：

┌─────────────────────────────────┐
│         ArcInner<T>             │
├─────────────────────────────────┤
│  strong: atomic usize           │ ← 强引用计数
│  weak: atomic usize             │ ← 弱引用计数
│  data: T                        │ ← 实际数据
└─────────────────────────────────┘
         ↑
         │ pointer
┌────────┴────────┐
│      Arc<T>     │
└─────────────────┘
```

### 引用计数机制

- **强引用**：所有权引用，计数归零时释放数据
- **弱引用**：不持有所有权，避免循环引用

### Mutex 机制

```
Mutex 工作流程：

线程A                    Mutex                    线程B
  │                       │                         │
  │──── lock() ──────────>│                         │
  │                       │<──── lock() ────────────│
  │   访问数据 ✅          │     等待 ⏳              │
  │<─── guard drop ───────│                         │
  │                       │<──── 获得锁 ────────────│
  │                       │     访问数据 ✅          │
```

### 死锁风险

```rust
// ❌ 潜在死锁
let guard1 = mutex1.lock().unwrap();
let guard2 = mutex2.lock().unwrap();  // 如果另一个线程相反顺序持有锁

// ✅ 避免死锁：按固定顺序获取锁
// 或使用 parking_lot::Mutex 的 try_lock
```

## 5. 使用场景

### 适合使用

- 多线程共享状态
- 共享缓存
- 共享配置
- 生产者-消费者模式

### 不适合使用

- 只读共享 → 使用 `Arc<T>` 即可
- 高性能并发读 → 考虑 `Arc<RwLock<T>>`
- 消息传递 → 考虑 `mpsc` channel

### 最佳实践

```rust
// ✅ 正确：使用 Arc 共享，Mutex 保护可变状态
let shared = Arc::new(Mutex::new(vec![]));

// ✅ 正确：及时释放锁
{
    let mut data = shared.lock().unwrap();
    data.push(1);
}  // guard drop，自动释放锁

// ❌ 错误：长时间持有锁
let guard = mutex.lock().unwrap();
long_operation();  // 锁未释放，阻塞其他线程
```

## 6. 代码示例

### 基础用法

```rust
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    // 创建共享数据
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    // 启动多个线程
    for _ in 0..10 {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();
            *num += 1;
        });
        handles.push(handle);
    }

    // 等待所有线程完成
    for handle in handles {
        handle.join().unwrap();
    }

    println!("Result: {}", *counter.lock().unwrap());  // Result: 10
}
```

### 共享向量

```rust
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let data = Arc::new(Mutex::new(Vec::new()));
    let mut handles = vec![];

    for i in 0..5 {
        let data = Arc::clone(&data);
        let handle = thread::spawn(move || {
            let mut vec = data.lock().unwrap();
            vec.push(i);
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("{:?}", *data.lock().unwrap());  // [0, 1, 2, 3, 4]（顺序可能不同）
}
```

### 结构体中的使用

```rust
use std::sync::{Arc, Mutex};

pub struct SharedState {
    pub counter: i32,
    pub items: Vec<String>,
}

pub struct AppState {
    state: Arc<Mutex<SharedState>>,
}

impl AppState {
    pub fn new() -> Self {
        Self {
            state: Arc::new(Mutex::new(SharedState {
                counter: 0,
                items: Vec::new(),
            })),
        }
    }

    pub fn increment(&self) {
        let mut state = self.state.lock().unwrap();
        state.counter += 1;
    }

    pub fn add_item(&self, item: String) {
        let mut state = self.state.lock().unwrap();
        state.items.push(item);
    }

    pub fn get_counter(&self) -> i32 {
        let state = self.state.lock().unwrap();
        state.counter
    }
}

fn main() {
    let app = AppState::new();
    app.increment();
    app.add_item("hello".to_string());
    println!("Counter: {}", app.get_counter());  // Counter: 1
}
```

### RwLock 对比

```rust
use std::sync::{Arc, RwLock};

fn main() {
    // RwLock：读多写少场景
    let data = Arc::new(RwLock::new(vec![1, 2, 3]));

    // 多个读锁可以同时存在
    let r1 = data.read().unwrap();
    let r2 = data.read().unwrap();
    println!("{:?} {:?}", *r1, *r2);  // 读锁可并发
    drop(r1);
    drop(r2);

    // 写锁独占
    let mut w = data.write().unwrap();
    w.push(4);
    drop(w);

    // 选择建议：
    // - Mutex：读写均衡或写多
    // - RwLock：读多写少
}
```

### 简化 API 设计

```rust
use std::sync::{Arc, Mutex};

// 封装简化使用
pub struct ThreadSafeVec<T> {
    inner: Arc<Mutex<Vec<T>>>,
}

impl<T> ThreadSafeVec<T> {
    pub fn new() -> Self {
        Self {
            inner: Arc::new(Mutex::new(Vec::new())),
        }
    }

    pub fn push(&self, value: T) {
        self.inner.lock().unwrap().push(value);
    }

    pub fn pop(&self) -> Option<T> {
        self.inner.lock().unwrap().pop()
    }

    pub fn len(&self) -> usize {
        self.inner.lock().unwrap().len()
    }
}

fn main() {
    let vec = ThreadSafeVec::new();
    vec.push(1);
    vec.push(2);
    println!("Len: {}", vec.len());  // Len: 2
}
```

## 7. 总结

`Arc<Mutex>` 提供线程安全的共享状态：

| 特性 | Arc<Mutex> | Arc<RwLock> | 消息传递 |
|------|------------|-------------|----------|
| 共享状态 | ✅ | ✅ | ❌ |
| 读写并发 | ❌ 互斥 | ✅ 读并发 | ❌ |
| 复杂度 | 低 | 中 | 中 |
| 死锁风险 | ⚠️ 有 | ⚠️ 有 | ✅ 无 |

选择建议：
- 共享可变状态 → `Arc<Mutex>`
- 读多写少 → `Arc<RwLock>`
- 消息传递 → `mpsc` channel
- 不可变共享 → `Arc<T>` 即可