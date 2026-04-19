# Go 文档风格指南

## 术语规范

### 基础概念

| 中文 | 英文 | 说明 |
|------|------|------|
| 包 | package | Go 代码组织单元 |
| 模块 | module | Go 1.11+ 依赖管理单元 |
| 导入 | import | 引入其他包 |
| 导出 | exported | 首字母大写的标识符 |
| 非导出 | unexported | 首字母小写的标识符 |

### 类型系统

| 中文 | 英文 | 说明 |
|------|------|------|
| 结构体 | struct | 数据结构类型 |
| 接口 | interface | 类型抽象定义 |
| 切片 | slice | 动态长度数组视图 |
| 映射 | map | 键值对集合 |
| 指针 | pointer | 内存地址引用 |
| 数组 | array | 固定长度数组 |
| 泛型 | generics | Go 1.18+ 类型参数 |
| 类型断言 | type assertion | 运行时类型检查 |
| 类型转换 | type conversion | 显式类型转换 |

### 并发编程

| 中文 | 英文 | 说明 |
|------|------|------|
| 协程 | goroutine | Go 并发执行单元 |
| 通道 | channel | 协程间通信机制 |
| 缓冲通道 | buffered channel | 带缓冲的通道 |
| 无缓冲通道 | unbuffered channel | 同步通道 |
| 多路复用 | select | 同时监听多个通道 |
| 互斥锁 | Mutex | 互斥同步原语 |
| 读写锁 | RWMutex | 读写分离锁 |
| 等待组 | WaitGroup | 等待一组协程完成 |
| 原子操作 | atomic | 不可中断的操作 |
| 条件变量 | Cond | 条件同步机制 |

### 上下文与错误

| 中文 | 英文 | 说明 |
|------|------|------|
| 上下文 | context | 请求作用域控制 |
| 错误 | error | 错误接口类型 |
| 错误包装 | error wrapping | %w 包装错误 |
| 取消信号 | cancellation | context 取消机制 |
| 超时 | timeout | 操作时间限制 |
| 截止时间 | deadline | 操作最后期限 |
| panic | panic | 运行时恐慌 |
| recover | recover | 捕获 panic |

### 方法与函数

| 中文 | 英文 | 说明 |
|------|------|------|
| 函数 | function | 独立代码块 |
| 方法 | method | 绑定到类型的函数 |
| 接收器 | receiver | 方法的接收者 |
| 值接收器 | value receiver | 值类型接收者 |
| 指针接收器 | pointer receiver | 指针类型接收者 |
| 可变参数 | variadic | 可变数量参数 |
| 闭包 | closure | 捕获外部变量的函数 |
| 延迟调用 | defer | 延迟执行语句 |
| 初始化函数 | init | 包初始化函数 |

### 结构体特性

| 中文 | 英文 | 说明 |
|------|------|------|
| 字段 | field | 结构体成员 |
| 嵌入 | embedding | 结构体组合机制 |
| 标签 | tag | 结构体字段元数据 |
| 组合 | composition | 通过嵌入实现复用 |
| 匿名字段 | anonymous field | 嵌入的类型字段 |
| 提升字段 | promoted field | 嵌入类型的字段 |

### 测试与工具

| 中文 | 英文 | 说明 |
|------|------|------|
| 测试 | test | 单元测试 |
| 基准测试 | benchmark | 性能测试 |
| 示例测试 | example | 文档示例 |
| 模糊测试 | fuzzing | Go 1.18+ 随机测试 |
| 覆盖率 | coverage | 测试覆盖度 |
| 性能分析 | profiling | pprof 性能分析 |
| 构建标签 | build tag | 条件编译标签 |

### 常用标准库

| 中文 | 英文 | 说明 |
|------|------|------|
| 格式化 I/O | fmt | 格式化输入输出 |
| 编码 JSON | encoding/json | JSON 编解码 |
| 编码 XML | encoding/xml | XML 编解码 |
| 网络 HTTP | net/http | HTTP 客户端/服务端 |
| 网络 TCP | net | TCP/UDP 网络编程 |
| 操作系统 | os | 操作系统接口 |
| 输入输出 | io | I/O 原语接口 |
| 字符串 | strings | 字符串操作 |
| 字节 | bytes | 字节切片操作 |
| 时间 | time | 时间和持续时间 |
| 同步 | sync | 同步原语 |
| 日志 | log | 简单日志 |
| 数学 | math | 数学函数 |
| 加密 | crypto | 加密算法 |
| 压缩 | compress | 压缩算法 |

## 代码示例规范

### 导入规范

```go
import (
    // 标准库（分组）
    "fmt"
    "os"
    "context"

    // 第三方库（分组）
    "github.com/gin-gonic/gin"
    "go.uber.org/zap"

    // 本地包（分组）
    "myproject/pkg/utils"
    "myproject/internal/service"
)
```

### 命名规范

```go
// 包名：小写单词，不使用下划线
package mypackage

// 导出标识符：首字母大写（PascalCase）
type UserService struct {}
func GetUser() {}

// 非导出标识符：首字母小写（camelCase）
type userConfig struct {}
func getUser() {}

// 接口：单方法接口以 "-er" 结尾
type Reader interface {
    Read(p []byte) (n int, err error)
}

// 常量：导出常量 PascalCase，非导出 camelCase
const MaxSize = 100
const maxSize = 50
```

### 示例结构

```go
// 1. 包声明
package main

// 2. 导入分组
import (
    "fmt"
)

// 3. 类型定义（如需要）
type User struct {
    Name string
    Age  int
}

// 4. 函数定义
func (u *User) Greet() string {
    return fmt.Sprintf("Hello, %s", u.Name)
}

// 5. main 函数（可执行程序）
func main() {
    u := User{Name: "Alice", Age: 30}
    fmt.Println(u.Greet())
}
```

### 错误处理规范

```go
// ✅ 正确：返回 error，不使用 panic
func ReadFile(path string) ([]byte, error) {
    data, err := os.ReadFile(path)
    if err != nil {
        return nil, fmt.Errorf("read file %s: %w", path, err)
    }
    return data, nil
}

// ✅ 正确：错误检查链
result, err := doSomething()
if err != nil {
    return fmt.Errorf("operation failed: %w", err)
}

// ❌ 错误：生产代码使用 panic
func ReadFile(path string) []byte {
    data, err := os.ReadFile(path)
    if err != nil {
        panic(err)  // 仅在不可恢复场景使用
    }
    return data
}
```

## 版本标注

```go
// Go 1.11+ 模块支持
module myproject

// Go 1.18+ 泛型
func Min[T constraints.Ordered](a, b T) T {
    if a < b {
        return a
    }
    return b
}

// Go 1.21+ slices 包
import "slices"
slices.Sort(data)

// Go 1.21+ maps 包
import "maps"
maps.Copy(dst, src)

// Go 1.22+ for-range 支持整数
for i := range 10 {
    fmt.Println(i)  // 0, 1, 2, ..., 9
}
```

## Go 特有章节

### 并发模式

#### goroutine 启动

```go
// 启动 goroutine
go func() {
    fmt.Println("Running in goroutine")
}()
```

#### channel 使用

```go
// 无缓冲 channel（同步）
ch := make(chan int)

// 有缓冲 channel
ch := make(chan int, 10)

// 发送
ch <- value

// 接收
value := <-ch

// 关闭（仅发送方关闭）
close(ch)

// 检查关闭
for v := range ch {
    fmt.Println(v)
}
```

#### select 多路复用

```go
select {
case msg := <-ch1:
    fmt.Println("Received from ch1:", msg)
case msg := <-ch2:
    fmt.Println("Received from ch2:", msg)
case <-time.After(1 * time.Second):
    fmt.Println("Timeout")
default:
    fmt.Println("No message available")
}
```

#### context 使用

```go
// 创建带超时的 context
ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
defer cancel()

// 在函数中传递 context
func DoWork(ctx context.Context) error {
    select {
    case <-ctx.Done():
        return ctx.Err()
    default:
        // 执行工作
    }
    return nil
}
```

### 接口设计

```go
// 小接口原则
type Reader interface {
    Read(p []byte) (n int, err error)
}

type Writer interface {
    Write(p []byte) (n int, err error)
}

// 组合接口
type ReadWriter interface {
    Reader
    Writer
}
```

### 结构体标签

```go
type User struct {
    Name string `json:"name" validate:"required"`
    Age  int    `json:"age" validate:"min=0,max=150"`
    Email string `json:"email,omitempty"`
}
```

### 初始化与零值

```go
// Go 变量默认零值，无需显式初始化
var i int       // 0
var s string    // ""（空字符串）
var p *int      // nil
var u User      // 所有字段零值

// 推荐显式初始化结构体（提高可读性）
u := User{
    Name: "Alice",
    Age:  30,
}
```

### defer 使用

```go
// defer 在函数返回时执行，常用于资源清理
func ProcessFile(path string) error {
    f, err := os.Open(path)
    if err != nil {
        return err
    }
    defer f.Close()  // 确保文件关闭

    // 处理文件...
    return nil
}

// 多个 defer 按 LIFO 顺序执行
defer fmt.Println("First")
defer fmt.Println("Second")
// 输出顺序：Second, First
```

## 注意事项格式

```go
// ❌ 错误示例
// 说明错误原因

// ✅ 正确示例
// 说明正确做法
```

## 性能注意事项

| 场景 | 建议 |
|------|------|
| 大字符串拼接 | 使用 `strings.Builder` |
| 频繁分配 | 使用 sync.Pool |
| JSON 处理 | 使用 json-iterator 或 sonic |
| 高并发 | channel 替代锁（如适用） |
| 内存逃逸 | 注意指针使用，减少 GC 压力 |

## 工具与构建

```bash
# 编译
go build -o myapp              # 编译当前目录
go build ./...                 # 编译所有包
go build -o myapp ./cmd/main   # 指定输出文件

# 运行
go run main.go                 # 直接运行
go run ./cmd/main              # 运行指定包

# 测试
go test                        # 运行测试
go test -v                     # 详细输出
go test -cover                 # 测试覆盖率
go test -bench .               # 基准测试
go test -race                  # 竞态检测

# 代码质量
go fmt ./...                   # 格式化代码
go vet ./...                   # 静态检查
golangci-lint run              # 全面 lint

# 依赖管理
go mod init myproject          # 初始化模块
go mod tidy                    # 整理依赖
go mod download                # 下载依赖
go get package@version         # 添加依赖

# 文档
go doc fmt.Println             # 查看文档
godoc -http=:6060              # 本地文档服务器
```