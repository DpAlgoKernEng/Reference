# sync.Map - Go 并发安全映射

## 1. 概述

`sync.Map` 是 Go 标准库中的并发安全映射类型，专为高并发读多写少场景优化。它通过读写分离和原子操作实现无锁读取，避免了使用 `map` + `sync.RWMutex` 的锁竞争问题。

定义在 `sync` 包中，无需导入额外依赖。

与普通 `map` 的关键区别：
- `map` 非并发安全，并发读写会 panic
- `sync.Map` 并发安全，零配置即可使用

## 2. 来源与演变

### Go 1.9 引入

`sync.Map` 在 **Go 1.9**（2017 年）中首次引入，解决了标准库中并发安全映射的需求。

### 设计动机

- 加锁的 `map` 在高并发读场景下存在锁竞争
- 需要一种针对读多写少优化的并发安全映射
- 借鉴 Java `ConcurrentHashMap` 的思想，采用读写分离设计

### 主要版本变更

| 版本 | 变更 |
|------|------|
| Go 1.9 | 首次引入 |
| Go 1.20 | 性能优化，减少内存分配 |

## 3. 语法与参数

### 主要方法

```go
type Map struct {
    // 内部字段不对外暴露
}

// 存储键值对
func (m *Map) Store(key, value interface{})

// 加载值（如果存在）
func (m *Map) Load(key interface{}) (value interface{}, ok bool)

// 删除键值对
func (m *Map) Delete(key interface{})

// 加载或存储（原子操作）
func (m *Map) LoadOrStore(key, value interface{}) (actual interface{}, loaded bool)

// 加载并删除
func (m *Map) LoadAndDelete(key interface{}) (value interface{}, loaded bool)

// 遍历所有键值对
func (m *Map) Range(f func(key, value interface{}) bool)
```

### 方法说明

| 方法 | 说明 | 复杂度 |
|------|------|--------|
| `Store` | 存储键值对 | 平均 O(1) |
| `Load` | 加载值 | 平均 O(1) |
| `Delete` | 删除键值对 | 平均 O(1) |
| `LoadOrStore` | 原子加载或存储 | 平均 O(1) |
| `Range` | 遍历所有元素 | O(n) |

## 4. 底层原理

### 读写分离结构

```
sync.Map 内部结构：

┌─────────────────────────────────────┐
│             sync.Map                │
├─────────────────────────────────────┤
│  read: atomic.Value                 │
│    └── readOnly struct {            │
│          m: map[interface{}]*entry  │
│          amended: bool              │
│        }                            │
├─────────────────────────────────────┤
│  dirty: map[interface{}]*entry      │
│  misses: int                        │
└─────────────────────────────────────┘
```

### 工作机制

1. **读取路径**：优先从 `read` 读取（无锁）
2. **写入路径**：写入 `dirty`，标记 `amended = true`
3. **miss 提升**：当 `misses` 达到阈值，将 `dirty` 提升为 `read`

### 性能特征

| 操作 | 性能 | 说明 |
|------|------|------|
| 读取已存在键 | O(1) 无锁 | 从 read 读取 |
| 读取新键 | O(1) 加锁 | 从 dirty 读取 |
| 写入 | O(1) 加锁 | 写入 dirty |
| 删除 | O(1) | 标记删除 |

## 5. 使用场景

### 适合使用

- 读多写少的并发场景
- 键值对相对稳定
- 需要并发安全的缓存

### 不适合使用

- 写多读少场景 → 使用 `map` + `sync.RWMutex`
- 需要严格一致性 → 考虑其他方案
- 键集合频繁变化

### 最佳实践

```go
// ✅ 正确：读多写少场景
var m sync.Map
m.Store("key", "value")
if v, ok := m.Load("key"); ok {
    fmt.Println(v)
}

// ✅ 正确：使用 LoadOrStore 避免重复计算
value, loaded := m.LoadOrStore("key", computeValue())

// ❌ 错误：频繁写入
for i := 0; i < 100000; i++ {
    m.Store(i, i)  // 性能不佳
}
```

## 6. 代码示例

### 基础用法

```go
package main

import (
    "fmt"
    "sync"
)

func main() {
    var m sync.Map

    // 存储
    m.Store("name", "Alice")
    m.Store("age", 30)

    // 加载
    if v, ok := m.Load("name"); ok {
        fmt.Println("name:", v)  // name: Alice
    }

    // 加载不存在的键
    if _, ok := m.Load("unknown"); !ok {
        fmt.Println("not found")
    }

    // 删除
    m.Delete("age")

    // 遍历
    m.Range(func(key, value interface{}) bool {
        fmt.Printf("%v: %v\n", key, value)
        return true  // 继续遍历
    })
}
```

### 并发使用

```go
package main

import (
    "fmt"
    "sync"
)

func main() {
    var m sync.Map
    var wg sync.WaitGroup

    // 并发写入
    for i := 0; i < 100; i++ {
        wg.Add(1)
        go func(n int) {
            defer wg.Done()
            m.Store(n, n*n)
        }(i)
    }

    // 并发读取
    for i := 0; i < 100; i++ {
        wg.Add(1)
        go func(n int) {
            defer wg.Done()
            if v, ok := m.Load(n); ok {
                fmt.Printf("%d: %v\n", n, v)
            }
        }(i)
    }

    wg.Wait()
}
```

### LoadOrStore 模式

```go
package main

import (
    "sync"
    "time"
)

func main() {
    var m sync.Map

    // 模拟昂贵的计算
    compute := func() interface{} {
        time.Sleep(100 * time.Millisecond)
        return "computed_value"
    }

    // LoadOrStore 确保只计算一次
    value, loaded := m.LoadOrStore("key", compute())
    if loaded {
        fmt.Println("已存在:", value)
    } else {
        fmt.Println("新创建:", value)
    }

    // 再次调用不会重新计算
    value2, loaded2 := m.LoadOrStore("key", compute())
    fmt.Println("第二次 loaded:", loaded2)  // true
}
```

### 缓存实现

```go
package main

import (
    "sync"
)

type Cache struct {
    m sync.Map
}

func (c *Cache) Get(key string, loader func() (interface{}, error)) (interface{}, error) {
    // 先尝试从缓存加载
    if v, ok := c.m.Load(key); ok {
        return v, nil
    }

    // 缓存未命中，执行加载
    value, err := loader()
    if err != nil {
        return nil, err
    }

    c.m.Store(key, value)
    return value, nil
}

func main() {
    cache := &Cache{}

    // 使用缓存
    result, err := cache.Get("user:1", func() (interface{}, error) {
        // 模拟数据库查询
        return map[string]string{"name": "Alice"}, nil
    })

    if err == nil {
        fmt.Println(result)
    }
}
```

### 类型安全封装

```go
package main

import "sync"

// 类型安全的 Map 封装
type TypedMap[K comparable, V any] struct {
    m sync.Map
}

func NewTypedMap[K comparable, V any]() *TypedMap[K, V] {
    return &TypedMap[K, V]{}
}

func (m *TypedMap[K, V]) Store(key K, value V) {
    m.m.Store(key, value)
}

func (m *TypedMap[K, V]) Load(key K) (V, bool) {
    var zero V
    v, ok := m.m.Load(key)
    if !ok {
        return zero, false
    }
    return v.(V), true
}

func (m *TypedMap[K, V]) Delete(key K) {
    m.m.Delete(key)
}

func main() {
    // 类型安全的 Map
    userMap := NewTypedMap[string, int]()
    userMap.Store("Alice", 25)

    if age, ok := userMap.Load("Alice"); ok {
        fmt.Println("Age:", age)  // Age: 25
    }
}
```

## 7. 总结

`sync.Map` 提供并发安全的键值存储：

| 特性 | sync.Map | map + RWMutex |
|------|----------|---------------|
| 并发安全 | ✅ 内置 | ✅ 需手动加锁 |
| 读性能 | ✅ 无锁读取 | ⚠️ 读锁竞争 |
| 写性能 | ⚠️ 有开销 | ✅ 写锁 |
| 类型安全 | ❌ interface{} | ✅ 泛型 |

选择建议：
- 读多写少、高并发 → `sync.Map`
- 写多读少 → `map` + `sync.RWMutex`
- 单线程 → 普通 `map`
- 需要类型安全 → 封装或使用泛型