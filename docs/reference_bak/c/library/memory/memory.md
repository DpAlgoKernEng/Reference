# 动态内存管理（Dynamic memory management）

来源：[cppreference.com](https://en.cppreference.com)

---

## 简介

动态内存管理是指在程序运行期间动态分配和释放内存的技术。C 语言提供了标准库函数用于手动控制内存资源，这对于创建大小可变的数据结构（如链表、树、缓冲区）至关重要。

---

## 内存管理函数一览

| 函数 | 功能描述 |
|------|----------|
| `malloc(size_t size)` | 分配指定字节数的未初始化内存 |
| `calloc(size_t num, size_t size)` | 分配并清零多个对象的内存 |
| `realloc(void *ptr, size_t new_size)` | 调整已有内存块大小 |
| `free(void *ptr)` | 释放之前分配的内存 |
| `free_sized(void *ptr, size_t size)`（C23） | 按指定大小释放已分配内存 |
| `free_aligned_sized(void *ptr, size_t alignment, size_t size)`（C23） | 按对齐要求和大小释放内存 |
| `aligned_alloc(size_t alignment, size_t size)`（C11） | 分配特定对齐要求的内存 |

---

## 函数详解

### 1. `malloc()`

```c
void *malloc(size_t size);
```

#### 说明

- 分配 `size` 字节的内存空间。
- 成功返回指向首地址的 `void*`，失败返回 `NULL`。
- 返回的内存内容是**未初始化**的。

#### 示例代码

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = (int *)malloc(5 * sizeof(int));
    if (!arr) {
        perror("内存分配失败");
        return 1;
    }

    for (int i = 0; i < 5; ++i)
        arr[i] = i + 1;

    for (int i = 0; i < 5; ++i)
        printf("%d ", arr[i]);  // 输出：1 2 3 4 5

    free(arr);  // 注意释放内存
    return 0;
}
```

---

### 2. `calloc()`

```c
void *calloc(size_t num, size_t size);
```

#### 说明

- 分配 `num * size` 字节内存，且**自动置为零**。
- 成功返回指向首地址的 `void*`，失败返回 `NULL`。

#### 示例代码

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = (int *)calloc(5, sizeof(int));
    if (!arr) {
        perror("内存分配失败");
        return 1;
    }

    for (int i = 0; i < 5; ++i)
        printf("%d ", arr[i]);  // 输出：0 0 0 0 0

    free(arr);
    return 0;
}
```

---

### 3. `realloc()`

```c
void *realloc(void *ptr, size_t new_size);
```

#### 说明

- **重新调整**已分配内存块的大小：
  - 如果 `new_size >` 原大小，扩展内存；
  - 如果 `new_size <` 原大小，缩小内存；
  - 若无法原地调整，将复制旧数据到新位置。

#### 示例代码

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = (int *)malloc(3 * sizeof(int));
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    // 扩展数组容量
    arr = (int *)realloc(arr, 5 * sizeof(int));
    if (!arr) {
        perror("重新分配失败");
        return 1;
    }

    arr[3] = 4;
    arr[4] = 5;

    for (int i = 0; i < 5; ++i)
        printf("%d ", arr[i]);  // 输出：1 2 3 4 5

    free(arr);
    return 0;
}
```

---

### 4. `free()`

```c
void free(void *ptr);
```

#### 说明

- 释放之前通过 `malloc`、`calloc` 或 `realloc` 分配的内存。
- 若 `ptr == NULL`，调用无效果。

> ⚠️ 注意事项：
- 不要重复 `free()`；
- 释放后不要继续使用该指针（悬空指针）；
- 建议 `free(ptr)` 后将 `ptr = NULL`；

---

### 5. `aligned_alloc()`（C11）

```c
void *aligned_alloc(size_t alignment, size_t size);
```

#### 说明

- 分配对齐内存，要求 `size` 是 `alignment` 的倍数。
- `alignment` 必须是 2 的幂，且是平台支持的对齐值。

#### 示例代码

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    // 分配 16 字节对齐的 100 字节内存
    void *ptr = aligned_alloc(16, 100);
    if (!ptr) {
        perror("对齐内存分配失败");
        return 1;
    }

    printf("对齐内存地址：%p\n", ptr);
    free(ptr);
    return 0;
}
```

---

### 6. `free_sized()` 和 `free_aligned_sized()`（C23）

```c
void free_sized(void *ptr, size_t size);
void free_aligned_sized(void *ptr, size_t alignment, size_t size);
```

#### 说明

- 显式指定内存大小和对齐信息以优化释放性能（C23 新增）。
- 在某些高性能场景（如嵌入式系统）中更高效。

---

## 最佳实践建议

| 原则 | 建议做法 |
|------|----------|
| ✅ 检查分配结果 | 所有 `malloc()` 结果都要判断是否为 `NULL` |
| ✅ 成对使用 | 每次 `malloc()` 都应有对应的 `free()` |
| ✅ 避免重复释放 | 释放后将指针置为 `NULL` |
| ✅ 使用工具检测 | 使用 Valgrind、AddressSanitizer 等工具排查内存泄漏 |
| ✅ 安全释放 | 先释放嵌套结构中的子节点，再释放主结构 |

---

## 常见错误与防范方法

| 错误类型 | 描述 | 防范措施 |
|----------|------|----------|
| 内存泄漏 | 分配后未释放 | 使用 `valgrind` 检测 |
| 悬空指针 | 释放后仍然使用 | 释放后设为 `NULL` |
| 重复释放 | 同一块内存被多次释放 | 释放前检查是否为 `NULL` |
| 缓冲区溢出 | 写入超出分配范围 | 使用 `calloc` 清零并注意边界检查 |
| 错误对齐使用 | `aligned_alloc` 大小不满足对齐要求 | 确保 `size % alignment == 0` |

---

## 示例：安全内存分配封装

```c
#include <stdio.h>
#include <stdlib.h>

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void safe_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;  // 防止悬空指针
    }
}
```

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 内存管理库 | [C++ Memory Management](https://zh.cppreference.com/w/cpp/memory) |

---

> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=c/memory&oldid=143505>  
> 最后修改时间：2022年9月18日 09:41 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

✅ **小结提示**：
- 使用动态内存时应严格遵守“谁分配谁释放”原则；
- 开发大型项目建议使用内存池或智能指针替代裸指针；
- 始终结合调试器或内存分析工具确保程序健壮性。