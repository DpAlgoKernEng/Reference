# 事务内存 (Transactional Memory)

## 1. 概述 (Overview)

**事务内存**（Transactional Memory）是一种并发同步机制，它将语句组组合成事务（transaction），这些事务具有以下核心特性：

### 核心概念

- **原子性（Atomic）**：事务中的所有语句要么全部执行，要么完全不执行
- **隔离性（Isolated）**：事务中的语句不能观察到其他事务的半写入状态，即使它们并行执行

### 技术定位

事务内存技术规范（TM TS）是 ISO/IEC TS 19841:2015 定义的一项**实验性功能**。这是一个技术规范（Technical Specification），尚未正式合并到 C++ 标准中。

### 特性测试宏

如果支持特性测试，可以通过宏常量 `__cpp_transactional_memory`（值大于等于 201505）来检测此功能是否可用。

### 典型实现方式

典型实现使用以下策略：
1. **硬件事务内存**（Hardware Transactional Memory）：在支持的硬件上使用，直到变更集饱和为止
2. **软件事务内存**（Software Transactional Memory）：当硬件支持不可用或不足时回退使用，通常使用乐观并发实现

### 重要警告

在事务中访问变量和在事务外访问同一变量，如果没有其他外部同步，则构成**数据竞争**（data race）。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

事务内存最初源于数据库领域的事务概念，后来被引入到编程语言的并发控制中。它提供了一种比传统锁机制更高层次的并发控制抽象。

### 设计动机

传统的锁机制存在以下问题：
- 死锁（deadlock）风险
- 优先级反转（priority inversion）
- 难以组合（composability）
- 细粒度锁管理复杂

事务内存通过提供原子块和同步块，简化了并发编程模型，让编译器和运行时系统负责底层的同步细节。

### 版本变更历史

| 时间 | 版本 | 说明 |
|------|------|------|
| GCC 4.7 | 早期版本 | 支持早期版本的事务内存规范 |
| GCC 6.1 | TM TS 支持 | 支持 TM TS 规范（需要 `-fgnu-tm` 编译选项） |
| 2015 | ISO/IEC TS 19841:2015 | 正式发布事务内存技术规范 |

### 编译器支持

| 编译器 | 版本要求 | 编译选项 |
|--------|---------|---------|
| GCC | 6.1+ | `-fgnu-tm` |

---

## 3. 语法与参数 (Syntax and Parameters)

### 同步块 (Synchronized Block)

```cpp
synchronized compound-statement
```

执行复合语句，如同在全局锁下执行：
- 程序中所有最外层的同步块按**单一全序**（single total order）执行
- 每个同步块的结束与该顺序中下一个同步块的开始同步
- 嵌套在其他同步块中的同步块没有特殊语义

**退出方式**：
- 到达块末尾
- 执行 `goto`、`break`、`continue`、`return`
- 抛出异常

**限制**：
- 不能通过 `goto` 或 `switch` 进入同步块
- 使用 `std::longjmp` 退出同步块是未定义行为

### 原子块 (Atomic Block)

```cpp
atomic_noexcept compound-statement    // (1)
atomic_cancel compound-statement      // (2)
atomic_commit compound-statement      // (3)
```

| 形式 | 异常处理行为 |
|------|-------------|
| `atomic_noexcept` | 如果抛出异常，调用 `std::abort()` |
| `atomic_cancel` | 如果抛出异常，调用 `std::abort()`，除非异常是用于事务取消的异常类型，此时事务被取消，所有修改回滚，异常继续栈展开 |
| `atomic_commit` | 如果抛出异常，事务正常提交 |

**可取消事务的异常类型**：
- `std::bad_alloc`
- `std::bad_array_new_length`
- `std::bad_cast`
- `std::bad_typeid`
- `std::bad_exception`
- `std::exception` 及其所有标准库派生类
- 特殊异常类型 `std::tx_exception<T>`

**限制**：
- 原子块中的复合语句不允许执行任何非 `transaction_safe` 的表达式或语句（编译时错误）
- 通过任何非异常方式退出原子块（到达末尾、`goto`、`break`、`continue`、`return`）将提交事务
- 使用 `std::longjmp` 退出原子块是未定义行为

### 事务安全函数 (Transaction-Safe Function)

```cpp
// 函数声明
int f() transaction_safe;

// 函数定义
int f() transaction_safe
{
    // 函数体
}

// Lambda 声明
auto lambda = []() transaction_safe { /* ... */ };
```

在 Lambda 声明中，`transaction_safe` 出现在捕获列表之后，或 `mutable` 关键字之后（如果使用）。

**事务安全函数的限制**：
- 不能通过 volatile 泛左值读取
- 不能调用非事务安全的析构函数
- 不能调用非事务安全函数

**隐式事务安全**：如果函数的所有操作都是事务安全的，则函数隐式为事务安全。

**指针转换**：
- 指向事务安全函数的指针可以隐式转换为指向普通函数的指针
- 指向事务安全成员函数的指针可以隐式转换为指向普通成员函数的指针
- 结果指针是否等于原始指针是未指定的

**警告**：如果通过引用或指针调用非事务安全函数，行为是未定义的。

### 事务安全虚函数 (Transaction-Safe Virtual Function)

```cpp
virtual void f() transaction_safe_dynamic;
```

如果 `transaction_safe_dynamic` 函数的最终覆盖器（final overrider）未声明为 `transaction_safe`，在原子块中调用它是未定义行为。

### 属性 [[optimize_for_synchronized]]

```cpp
[[optimize_for_synchronized]] void function_declaration();
```

**说明**：
- 可应用于函数声明的声明符
- 必须出现在函数的首次声明中
- 指示函数定义应该针对从同步语句调用进行优化

**ODR 要求**：如果在某个翻译单元中函数声明带有 `[[optimize_for_synchronized]]`，而在另一个翻译单元中同一函数声明不带有此属性，则程序是非良构的，无需诊断。

---

## 4. 底层原理 (Underlying Principles)

### 实现机制

典型实现使用混合策略：

1. **硬件事务内存（HTM）**：在支持的硬件上使用（如 Intel TSX）
   - 性能开销低
   - 受限于硬件容量（变更集饱和）

2. **软件事务内存（STM）**：当硬件支持不可用或不足时回退使用
   - 通常使用乐观并发（optimistic concurrency）实现
   - 检测到冲突时静默重试

### 同步块的实现

虽然同步块如同在全局锁下执行，但实现预期会：
- 检查每个块内的代码
- 对事务安全代码使用乐观并发（在可用时由硬件事务内存支持）
- 对非事务安全代码使用最小锁定

**序列化问题**：
当同步块调用非内联函数时，编译器可能需要：
1. 退出推测执行
2. 在整个调用周围持有锁

除非：
- 函数被声明为 `transaction_safe`，或
- 使用了 `[[optimize_for_synchronized]]` 属性

### [[optimize_for_synchronized]] 的优化效果

**无属性时的汇编代码**（GCC）：
```asm
insert_key(char*, char*):
    subq    $8, %rsp
    movq    %rsi, %rdx
    movq    %rdi, %rsi
    movl    $hash, %edi
    call    Hash::insert(char*, char*)
    testb   %al, %al
    je      .L20
    movb    $1, rehash(%rip)
    mfence
.L20:
    addq    $8, %rsp
    ret
```

**有属性时的汇编代码**（GCC）：
```asm
transaction clone for insert_key(char*, char*):
    subq    $8, %rsp
    movq    %rsi, %rdx
    movq    %rdi, %rsi
    movl    $hash, %edi
    call    transaction clone for Hash::insert(char*, char*)
    testb   %al, %al
    je      .L27
    xorl    %edi, %edi
    call    _ITM_changeTransactionMode  # 序列化点
    movb    $1, rehash(%rip)
    mfence
.L27:
    addq    $8, %rsp
    ret
```

**关键区别**：使用属性后，只有实际需要修改共享状态时才序列化，大部分调用可以保持推测执行。

### 乐观并发

软件事务内存的典型实现：
1. 事务开始时记录读集和写集
2. 事务执行期间不获取锁
3. 提交时验证是否有冲突
4. 如有冲突，回滚并重试

---

## 5. 使用场景 (Use Cases)

### 同步块适用场景

| 场景 | 说明 |
|------|------|
| 简单互斥访问 | 需要保护共享资源但不需要回滚语义 |
| 混合代码 | 需要调用非事务安全函数 |
| 快速原型 | 不需要严格事务语义的并发控制 |

### 原子块适用场景

| 场景 | 说明 |
|------|------|
| 复杂原子操作 | 需要原子性和隔离性的多步操作 |
| 回滚需求 | 需要事务回滚能力的场景 |
| 性能关键 | 高竞争、低冲突的场景 |

### 最佳实践

1. **优先使用同步块**：当不需要回滚语义时，同步块更灵活，可以调用非事务安全函数
2. **标记事务安全函数**：确保在原子块中调用的所有函数都是事务安全的
3. **使用优化属性**：对于频繁从同步块调用的函数，使用 `[[optimize_for_synchronized]]` 避免不必要的序列化
4. **避免数据竞争**：不要在事务内外混合访问同一变量

### 注意事项

1. **进入限制**：
   - 不能通过 `goto` 或 `switch` 进入同步块
   - 使用 `std::longjmp` 退出同步块或原子块是未定义行为

2. **事务安全检查**：
   - 原子块中只能调用事务安全函数
   - 不能通过 volatile 泛左值读取
   - 不能调用不安全的析构函数

3. **虚函数陷阱**：
   - 通过基类指针调用 `transaction_safe_dynamic` 函数时，确保最终覆盖器是事务安全的

### 常见陷阱

1. **数据竞争**：
   ```cpp
   int shared = 0;

   // 线程 1
   synchronized { shared++; }

   // 线程 2 - 数据竞争！
   shared = 100;
   ```

2. **非事务安全调用**：
   ```cpp
   void unsafe_function() { /* ... */ }

   void g()
   {
       atomic_noexcept {
           unsafe_function(); // 编译时错误
       }
   }
   ```

3. **volatile 访问**：
   ```cpp
   volatile int v = 0;

   int f() transaction_safe
   {
       return v; // 错误：通过 volatile 泛左值读取
   }
   ```

---

## 6. 代码示例 (Examples)

### 基础用法：同步块

```cpp
#include <iostream>
#include <thread>
#include <vector>

int f()
{
    static int i = 0;
    synchronized { // 开始同步块
        std::cout << i << " -> ";
        ++i;       // 每次调用 f() 获得唯一的 i 值
        std::cout << i << '\n';
        return i;  // 结束同步块
    }
}

int main()
{
    std::vector<std::thread> v(10);
    for (auto& t : v)
        t = std::thread([] { for (int n = 0; n < 10; ++n) f(); });
    for (auto& t : v)
        t.join();
    return 0;
}
```

**输出**：
```
0 -> 1
1 -> 2
2 -> 3
...
99 -> 100
```

### 原子块示例

```cpp
// 每次调用 f() 获得唯一的 i 值，即使并行执行
int f()
{
    static int i = 0;
    atomic_noexcept { // 开始事务
        // printf("before %d\n", i); // 错误：不能调用非事务安全函数
        ++i;
        return i; // 提交事务
    }
}
```

### atomic_cancel 异常处理

```cpp
#include <exception>
#include <tx_exception> // TM TS 特有头文件

void transactional_operation()
{
    atomic_cancel {
        // 执行一些操作
        if (some_error_condition) {
            throw std::bad_alloc(); // 事务将被取消，修改回滚
        }
        // 其他操作
    } // 正常提交
}
```

### 事务安全函数

```cpp
extern volatile int * p = 0;
struct S
{
    virtual ~S();
};

int f() transaction_safe
{
    int x = 0;  // 正确：非 volatile
    p = &x;     // 正确：指针本身不是 volatile
    // int i = *p; // 错误：通过 volatile 泛左值读取
    // S s;        // 错误：调用不安全的析构函数
    return x;
}

// 隐式事务安全函数
int factorial(int x)
{
    if (x <= 0)
        return 0;
    return x + factorial(x - 1);
}
```

### 优化属性使用

```cpp
#include <atomic>

class HashTable {
public:
    bool insert(char* key, char* value);
    void rehash();
};

std::atomic<bool> rehash{false};
HashTable hash;
bool shutdown = false;

// 维护线程运行此循环
void maintenance_thread(void*)
{
    while (!shutdown)
    {
        synchronized
        {
            if (rehash)
            {
                hash.rehash();
                rehash = false;
            }
        }
    }
}

// 工作线程每秒执行数十万次对此函数的调用
// 来自其他翻译单元中同步块的 insert_key() 调用
// 将导致这些块序列化，除非 insert_key() 标记为
// [[optimize_for_synchronized]]
[[optimize_for_synchronized]] void insert_key(char* key, char* value)
{
    bool concern = hash.insert(key, value);
    if (concern)
        rehash = true;
}
```

### 常见错误示例

```cpp
#include <csetjmp>
#include <iostream>

std::jmp_buf jump_buffer;

// 错误 1：通过 goto 进入同步块
void error_goto_entry()
{
    synchronized {
        label:
        std::cout << "In block\n";
    }
    goto label; // 未定义行为
}

// 错误 2：使用 longjmp 退出同步块
void error_longjmp_exit()
{
    synchronized {
        std::longjmp(jump_buffer, 1); // 未定义行为
    }
}

// 错误 3：在原子块中调用非事务安全函数
void unsafe_print(int x); // 未声明为 transaction_safe

void error_unsafe_call()
{
    atomic_noexcept {
        unsafe_print(42); // 编译时错误
    }
}

// 错误 4：虚函数的事务安全
struct Base
{
    virtual void f() transaction_safe_dynamic;
};

struct Derived : Base
{
    void f() override; // 未声明 transaction_safe
};

void error_virtual_call(Base* b)
{
    atomic_noexcept {
        b->f(); // 如果 b 指向 Derived，未定义行为
    }
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| **状态** | 技术规范（TS），实验性功能 |
| **同步块** | 如同全局锁执行，可调用非事务安全函数 |
| **原子块** | 提供真正的事务语义，只能调用事务安全函数 |
| **事务安全** | 使用 `transaction_safe` 关键字声明 |
| **优化** | `[[optimize_for_synchronized]]` 用于优化同步块调用 |

### 技术对比

| 特性 | 同步块 | 原子块 |
|------|--------|--------|
| **原子性** | 无 | 有 |
| **隔离性** | 有（通过全局顺序） | 有 |
| **可调用非事务安全函数** | 是 | 否 |
| **异常处理** | 正常传播 | 根据类型决定 |
| **回滚能力** | 无 | 有（atomic_cancel） |
| **性能** | 通常较低（可能持有锁） | 通常较高（乐观并发） |

### 标准库支持

TM TS 对标准库的修改包括：

**显式事务安全函数**：
- `std::forward`、`std::move`、`std::move_if_noexcept`
- `std::align`
- `std::abort`
- 全局默认 `operator new` 和 `operator delete`
- `std::allocator::construct`（如果调用的构造函数是事务安全的）
- `std::allocator::destroy`（如果调用的析构函数是事务安全的）
- `std::get_temporary_buffer`、`std::return_temporary_buffer`
- `std::addressof`、`std::pointer_traits::pointer_to`

**显式事务安全动态函数**：
- 所有支持事务取消的异常类型的虚成员函数

**新异常类型**：
- `std::tx_exception<T>`：用于事务取消的特殊异常类型

### 学习建议

1. **从同步块开始**：理解基本的并发控制概念
2. **理解事务安全限制**：明确哪些操作可以在原子块中执行
3. **使用优化属性**：在性能关键场景使用 `[[optimize_for_synchronized]]`
4. **谨慎生产使用**：TM TS 仍是实验性功能，需评估编译器支持

### 关键字

`atomic_cancel`、`atomic_commit`、`atomic_noexcept`、`synchronized`、`transaction_safe`、`transaction_safe_dynamic`

### 属性

`[[optimize_for_synchronized]]`

---

## 参考资料

- ISO/IEC TS 19841:2015 - Transactional Memory Technical Specification
- GCC 文档：Transactional Memory (-fgnu-tm)