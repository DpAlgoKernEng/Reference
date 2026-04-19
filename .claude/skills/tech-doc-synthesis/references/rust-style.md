# Rust 文档风格指南

## 术语规范

### 基础概念

| 中文 | 英文 | 说明 |
|------|------|------|
| 所有权 | ownership | Rust 内存管理核心 |
| 借用 | borrowing | 获取引用 |
| 生命周期 | lifetime | 引用有效期 |
| 作用域 | scope | 变量有效范围 |
| 移动 | move | 所有权转移 |
| 克隆 | clone | 深拷贝 |
| 复制 | copy | 栈上类型拷贝 |

### 类型系统

| 中文 | 英文 | 说明 |
|------|------|------|
| 结构体 | struct | 数据结构类型 |
| 枚举 | enum | 可变类型 |
| trait | trait | 共享行为定义 |
| 泛型 | generics | 类型参数化 |
| 关联类型 | associated type | trait 关联的类型 |
| 类型别名 | type alias | 类型命名 |
| 新类型 | newtype | 包装类型模式 |
| 单元类型 | unit type | `()` 无返回值类型 |
| 永不返回 | never type | `!` 发散类型 |

### 引用与指针

| 中文 | 英文 | 说明 |
|------|------|------|
| 不可变引用 | immutable reference | `&T` 只读引用 |
| 可变引用 | mutable reference | `&mut T` 可写引用 |
| 原始指针 | raw pointer | `*const T`、`*mut T` |
| 悬垂指针 | dangling pointer | 无效指针 |
| 空指针 | null pointer | Rust 无空指针 |
| 引用计数 | reference counting | Rc/Arc 引用计数 |

### 智能指针

| 中文 | 英文 | 说明 |
|------|------|------|
| Box | Box<T> | 堆分配指针 |
| Rc | Rc<T> | 单线程引用计数 |
| Arc | Arc<T> | 原子引用计数 |
| RefCell | RefCell<T> | 内部可变性（单线程） |
| Mutex | Mutex<T> | 互斥锁（多线程） |
| RwLock | RwLock<T> | 读写锁（多线程） |
| Cell | Cell<T> | Copy 类型内部可变性 |

### 错误处理

| 中文 | 英文 | 说明 |
|------|------|------|
| Option | Option<T> | 可能存在或不存在 |
| Result | Result<T, E> | 成功或错误 |
| Some | Some | Option 的有值变体 |
| None | None | Option 的无值变体 |
| Ok | Ok | Result 的成功变体 |
| Err | Err | Result 的错误变体 |
| 错误传播 | error propagation | `?` 运算符 |
| panic | panic | 运行时错误 |
| unwrap | unwrap | 获取值或 panic |

### 并发编程

| 中文 | 英文 | 说明 |
|------|------|------|
| 线程 | thread | 原生线程 |
| 消息传递 | message passing | channel 通信 |
| 通道 | channel | mpsc 通信管道 |
| 原子类型 | atomic | AtomicUsize 等 |
| 屏障 | barrier | 线程同步屏障 |
| 条件变量 | Condvar | 条件等待 |
| Send | Send | 线程间安全传递 trait |
| Sync | Sync | 线程间安全共享 trait |

### 异步编程

| 中文 | 英文 | 说明 |
|------|------|------|
| Future | Future | 异步计算抽象 |
| async | async | 异步函数标记 |
| await | await | 等待 Future 完成 |
| 异步运行时 | async runtime | Tokio/async-std |
| 任务 | task | 异步任务 |
| 异步块 | async block | 异步代码块 |

### 宏与元编程

| 中文 | 英文 | 说明 |
|------|------|------|
| 宏 | macro | 元编程 |
| 声明宏 | declarative macro | `macro_rules!` |
| 过程宏 | procedural macro | 自定义派生等 |
| 派生宏 | derive macro | 自动实现 trait |
| 属性宏 | attribute macro | 函数/模块属性 |

### 常用标准库

| 中文 | 英文 | 说明 |
|------|------|------|
| 向量 | Vec<T> | 动态数组 |
| 字符串 | String | 堆分配字符串 |
| 哈希映射 | HashMap<K, V> | 键值对集合 |
| 哈希集合 | HashSet<T> | 唯一值集合 |
| 迭代器 | Iterator | 遍历序列 |
| 范围 | Range | 范围迭代 |
| 切片 | slice | 数组视图 |
| 路径 | Path | 文件系统路径 |
| 文件 | File | 文件操作 |
| 格式化 | format! | 格式化字符串 |
| 打印 | println! | 标准输出打印 |
| 断言 | assert! | 测试断言 |

## 代码示例规范

### 导入规范

```rust
use std::collections::HashMap;
use std::io::{self, Read};

// 相对导入
use crate::models::User;
use super::utils;

// 重命名
use serde_json::Value as JsonValue;
```

### 命名规范

```rust
// 类型：PascalCase
struct UserManager {}
enum Status {}

// 函数/变量：snake_case
fn process_data() {}
let user_count = 0;

// 常量：全大写+下划线
const MAX_SIZE: usize = 100;

// 模块：snake_case
mod user_service;
```

### 类型定义

```rust
// 结构体
pub struct User {
    pub name: String,
    age: u32,
}

// 枚举
pub enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
}

// 泛型
pub struct Container<T> {
    value: T,
}

// trait
pub trait Draw {
    fn draw(&self);
}
```

### 函数定义

```rust
// 基本函数
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}

// 带生命周期
pub fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() { x } else { y }
}

// 泛型
pub fn min<T: Ord>(a: T, b: T) -> T {
    if a < b { a } else { b }
}

// 返回 Result
pub fn read_file(path: &str) -> Result<String, io::Error> {
    std::fs::read_to_string(path)
}
```

## 版本标注

### Rust 2018 Edition

```rust
// NLL (Non-Lexical Lifetimes)
// 模块路径变更
use crate::models::User;  // 显式 crate 前缀

// dyn Trait 显式标注
let r: &dyn Draw = &circle;
```

### Rust 2021 Edition

```rust
// Disjoint capture in closures
let s = "hello";
let f = || s.len();  // 捕获 s.len()

// format_args_capture
let name = "Alice";
println!("{name}");  // 直接使用变量名
```

### 稳定特性

```rust
// 1.39+ async/await
async fn fetch_data() -> Result<String, Error> {
    // ...
}

// 1.58+ format 字符串捕获
let count = 5;
println!("{count}");

// 1.65+ let-else
let Some(value) = option else {
    return;
};
```

## 所有权与借用

### 所有权转移

```rust
// Move
let s1 = String::from("hello");
let s2 = s1;  // s1 被移动
// println!("{}", s1);  // ❌ 编译错误

// Clone
let s1 = String::from("hello");
let s2 = s1.clone();  // 显式克隆
println!("{} {}", s1, s2);  // ✅ 正确
```

### 借用规则

```rust
// ✅ 多个不可变引用
let r1 = &s;
let r2 = &s;  // 允许

// ❌ 可变引用与不可变引用同时存在
let r1 = &s;
let r2 = &mut s;  // 编译错误

// ✅ 不可变引用失效后创建可变引用
let r1 = &s;
println!("{}", r1);
let r2 = &mut s;  // r1 已失效
```

### 生命周期

```rust
// 函数生命周期
fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() { x } else { y }
}

// 结构体生命周期
struct Holder<'a> {
    reference: &'a str,
}
```

## 错误处理

### Result 处理

```rust
// ✅ 使用 ? 运算符
pub fn read_config(path: &str) -> Result<Config, io::Error> {
    let content = std::fs::read_to_string(path)?;
    let config: Config = serde_json::from_str(&content)?;
    Ok(config)
}

// ❌ 生产代码使用 unwrap
let content = std::fs::read_to_string(path).unwrap();
```

### Option 处理

```rust
// ✅ 模式匹配
if let Some(name) = user_name {
    println!("Name: {}", name);
}

// ✅ 组合方法
fn get_user_email(id: u32) -> Option<String> {
    users.get(&id)
        .map(|u| u.email.clone())
        .filter(|e| !e.is_empty())
}

// ✅ 提供默认值
let name = user_name.unwrap_or("Unknown");
```

## 智能指针

```rust
// Box：堆分配
let b = Box::new(5);

// Rc：共享所有权（单线程）
use std::rc::Rc;
let a = Rc::new(5);
let b = Rc::clone(&a);

// Arc：共享所有权（多线程）
use std::sync::Arc;
let a = Arc::new(5);
let b = Arc::clone(&a);

// Mutex：内部可变性（多线程）
use std::sync::Mutex;
let data = Mutex::new(5);
*data.lock().unwrap() = 10;
```

## 异步编程

```rust
// async fn
async fn fetch_data(url: &str) -> Result<String, reqwest::Error> {
    reqwest::get(url).await?.text().await
}

// tokio 运行时
#[tokio::main]
async fn main() {
    let data = fetch_data("url").await.unwrap();
}
```

## 注意事项格式

```rust
// ❌ 错误示例
// 说明错误原因

// ✅ 正确示例
// 说明正确做法
```

## 性能注意事项

| 场景 | 建议 |
|------|------|
| 避免不必要 clone | 使用引用、copy 类型 |
| 字符串操作 | `String::with_capacity` |
| 迭代器链 | 避免中间 `.collect()` |
| 线程安全共享 | `Arc<Mutex>` vs `Arc<RwLock>` |

## 工具与构建

```bash
# 编译
cargo check
cargo build --release

# 测试
cargo test

# 文档
cargo doc --open

# 格式化
cargo fmt

# 静态检查
cargo clippy
```