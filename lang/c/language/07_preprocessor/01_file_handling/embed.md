# C 二进制资源包含 (Binary Resource Inclusion) (C23 起)

## 1. 概述 (Overview)

`#embed` 是 C23 标准引入的预处理器指令，用于在编译时将二进制资源嵌入到程序中。资源（resource）定义为可从翻译环境访问的数据源。

### 核心功能

- **二进制嵌入**：直接将二进制文件内容嵌入到源代码中
- **编译时处理**：在预处理阶段完成资源加载
- **类型安全**：生成整数常量表达式列表
- **参数控制**：支持 limit、prefix、suffix、if_empty 等参数

### 与 `#include` 的区别

| 特性 | `#include` | `#embed` |
|------|------------|----------|
| 引入版本 | C89 | C23 |
| 处理内容 | 源代码文本 | 二进制数据 |
| 输出形式 | 文本替换 | 整数列表 |
| 典型用途 | 头文件包含 | 资源嵌入 |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C23 之前，嵌入二进制资源的常见方法包括：
- 使用 `xxd -i` 等工具将二进制转换为 C 数组
- 编写构建脚本在编译前生成资源文件
- 使用汇编器的 `.incbin` 指令

`#embed` 标准化了这一过程，提供了编译器原生支持。

### C23 新特性

`#embed` 是 C23 标准的重要新特性之一，解决了长期存在的二进制资源嵌入问题。

### 设计动机

`#embed` 解决了以下核心问题：

1. **简化构建流程**：无需外部工具生成资源数组
2. **编译时验证**：资源在编译时加载，错误早发现
3. **跨平台一致**：标准化语法，消除平台差异
4. **性能优化**：编译器可优化资源加载

## 3. 语法与参数 (Syntax and Parameters)

### `#embed` 指令语法

| 语法形式 | 说明 |
|----------|------|
| `#embed <h-char-sequence> embed-parameter-sequence(可选)` | (1) 尖括号形式 |
| `#embed "q-char-sequence" embed-parameter-sequence(可选)` | (2) 双引号形式 |
| `#embed pp-tokens` | (3) 宏替换形式 |

### `__has_embed` 运算符语法

| 语法形式 | 说明 |
|----------|------|
| `__has_embed("q-char-sequence" embed-parameter-sequence(可选))` | (4) 检测资源 |
| `__has_embed(<h-char-sequence> embed-parameter-sequence(可选))` | (4) 检测资源 |
| `__has_embed(string-literal pp-balanced-token-sequence(可选))` | (5) 宏替换形式 |
| `__has_embed(<h-pp-tokens> pp-balanced-token-sequence(可选))` | (5) 宏替换形式 |

### 参数说明

#### h-char-sequence 和 q-char-sequence

与 `#include` 类似的文件名指定方式。

#### embed-parameter-sequence

一个或多个 `pp-parameter` 的序列。与属性列表不同，此序列**不是**逗号分隔的。

### `__has_embed` 返回值

| 返回值 | 说明 |
|--------|------|
| `__STDC_EMBED_FOUND__` | 资源找到且非空，所有参数支持 |
| `__STDC_EMBED_EMPTY__` | 资源为空，所有参数支持 |
| `__STDC_EMBED_NOT_FOUND__` | 资源未找到或参数不支持 |

### 标准参数

#### limit 参数

```
limit(常量表达式)
__limit__(常量表达式)
```

- 限制读取的字节数
- 必须是非负整数常量表达式
- 资源宽度设为 `min(表达式 × 元素宽度, 实现资源宽度)`

#### prefix 参数

```
prefix(pp-balanced-token-sequence)
__prefix__(pp-balanced-token-sequence)
```

- 在非空资源展开前插入指定标记序列

#### suffix 参数

```
suffix(pp-balanced-token-sequence)
__suffix__(pp-balanced-token-sequence)
```

- 在非空资源展开后插入指定标记序列

#### if_empty 参数

```
if_empty(pp-balanced-token-sequence)
__if_empty__(pp-balanced-token-sequence)
```

- 当资源为空时，用指定标记序列替换整个指令

## 4. 底层原理 (Underlying Principles)

### 处理流程

```
预处理阶段
    ↓
遇到 #embed 指令
    ↓
搜索资源文件
    ↓
┌─────────────────────────────────┐
│ 资源找到       │ 资源未找到     │
└─────────────────────────────────┘
    ↓                ↓
读取二进制数据    程序非法
    ↓
处理参数（limit等）
    ↓
生成整数常量表达式列表
    ↓
应用 prefix/suffix/if_empty
    ↓
替换到源代码中
```

### 展开结果

`#embed` 展开为逗号分隔的整数常量表达式列表：

```c
// 假设文件 data.bin 包含字节 0x89, 0x50, 0x4E, 0x47
const unsigned char data[] = {
#embed "data.bin"
};
// 展开为：
// const unsigned char data[] = {0x89, 0x50, 0x4E, 0x47};
```

### 资源宽度与元素宽度

| 概念 | 说明 |
|------|------|
| 实现资源宽度 | 实现定义的资源位大小 |
| 资源宽度 | 实现资源宽度，可被 limit 参数修改 |
| 嵌入元素宽度 | 默认为 `CHAR_BIT`，可被实现定义参数修改 |

**约束**：资源宽度必须能被嵌入元素宽度整除。

### 与 `fread` 的关系

如果：
- 整数列表用于初始化 `unsigned char` 或非负 `char` 数组
- 嵌入元素宽度等于 `CHAR_BIT`

则初始化后的数组内容**如同**在翻译时将资源的二进制数据 `fread` 到数组中。

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 嵌入图像/图标资源

```c
const uint8_t icon_png[] = {
#embed "icon.png"
};
```

#### 2. 嵌入字体数据

```c
const uint8_t font_data[] = {
#embed "font.bin"
};
```

#### 3. 嵌入配置文件

```c
const char default_config[] = {
#embed "default.conf"
, '\0'  // 添加空终止符
};
```

#### 4. 嵌入着色器代码

```c
const char vertex_shader[] = {
#embed "shader.vert"
, '\0'
};
```

#### 5. 条件资源加载

```c
#if __has_embed("optional.bin")
const uint8_t optional_data[] = {
#embed "optional.bin"
};
#endif
```

### 最佳实践

1. **使用 `__has_embed` 检测**：确保资源存在
2. **添加空终止符**：嵌入文本时记得添加 `'\0'`
3. **使用 `limit` 控制大小**：防止意外的大文件
4. **使用 `prefix`/`suffix`**：简化数组初始化语法
5. **使用 `if_empty` 处理空文件**：提供默认值

### 常见陷阱

#### 陷阱 1：忘记空终止符

```c
// 错误：文本数据没有空终止符
const char text[] = {
#embed "message.txt"
};  // 可能导致字符串操作越界

// 修正：添加空终止符
const char text[] = {
#embed "message.txt"
, '\0'
};
```

#### 陷阱 2：资源未找到

```c
// 危险：资源不存在会导致编译失败
const uint8_t data[] = {
#embed "missing.bin"
};

// 修正：使用 __has_embed 检测
#if __has_embed("missing.bin") == __STDC_EMBED_FOUND__
const uint8_t data[] = {
#embed "missing.bin"
};
#else
const uint8_t data[] = {0};  // 默认值
#endif
```

#### 陷阱 3：大文件未限制

```c
// 危险：大文件可能导致编译器内存问题
const uint8_t large[] = {
#embed "huge_file.bin"
};

// 修正：使用 limit 参数
const uint8_t large[] = {
#embed "huge_file.bin" limit(1024)
};
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：嵌入图像文件

```c
#include <stdint.h>
#include <stdio.h>

const uint8_t image_data[] = {
#embed "image.png"
};

void dump(const uint8_t arr[], size_t size)
{
    for (size_t i = 0; i != size; ++i)
        printf("%02X%c", arr[i], (i + 1) % 16 ? ' ' : '\n');
    puts("");
}

int main()
{
    puts("image_data[]:");
    dump(image_data, sizeof image_data);
    return 0;
}
```

**输出**：
```
image_data[]:
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52
00 00 00 01 00 00 00 01 01 03 00 00 00 25 DB 56
...
```

#### 示例 2：处理空文件

```c
#include <stdint.h>
#include <stdio.h>

const char message[] = {
#embed "message.txt" if_empty('M', 'i', 's', 's', 'i', 'n', 'g', '\n')
,'\0' // null terminator
};

void dump(const uint8_t arr[], size_t size)
{
    for (size_t i = 0; i != size; ++i)
        printf("%02X ", arr[i]);
    puts("");
}

int main()
{
    puts("message[]:");
    dump((const uint8_t*)message, sizeof message);
    return 0;
}
```

**输出**（当 message.txt 为空时）：
```
message[]:
4D 69 73 73 69 6E 67 0A 00
```

### 高级用法

#### 示例 3：使用 `__has_embed` 检测

```c
#include <stdio.h>

// 检测资源是否存在
#if __has_embed("config.bin") == __STDC_EMBED_FOUND__
    #define HAS_CONFIG 1
    const unsigned char config[] = {
    #embed "config.bin"
    };
#elif __has_embed("config.bin") == __STDC_EMBED_EMPTY__
    #define HAS_CONFIG 0
    const unsigned char config[] = {0};
#else
    #define HAS_CONFIG -1
    const unsigned char config[] = {0};  // 默认值
#endif

int main()
{
    printf("Config status: %d\n", HAS_CONFIG);
    printf("Config size: %zu\n", sizeof config);
    return 0;
}
```

#### 示例 4：使用 `prefix` 和 `suffix` 参数

```c
#include <stdint.h>

// 使用 prefix 和 suffix 简化数组定义
const uint8_t data[] = {
#embed "data.bin" prefix(0xFF, 0xFE) suffix(0x00)
};

// 等效于：
// const uint8_t data[] = {
//     0xFF, 0xFE,  // prefix
//     <data.bin 的内容>,
//     0x00         // suffix
// };

int main()
{
    printf("Data size: %zu\n", sizeof data);
    return 0;
}
```

#### 示例 5：使用 `limit` 参数

```c
#include <stdint.h>
#include <stdio.h>

// 只读取前 256 字节
const uint8_t header[] = {
#embed "large_file.bin" limit(256)
};

int main()
{
    printf("Header size: %zu bytes\n", sizeof header);
    return 0;
}
```

#### 示例 6：完整的资源管理示例

```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// 定义资源结构
typedef struct {
    const uint8_t* data;
    size_t size;
    const char* name;
} Resource;

// 安全嵌入宏
#define EMBED_RESOURCE(name, file, default_data) \
    static const uint8_t name##_data[] = { \
    #if __has_embed(file) == __STDC_EMBED_FOUND__ \
        #embed file \
    #else \
        default_data \
    #endif \
    }; \
    static const Resource name = { \
        .data = name##_data, \
        .size = sizeof(name##_data), \
        .name = #file \
    }

// 使用宏定义资源
EMBED_RESOURCE(logo, "logo.png", 0x89, 0x50, 0x4E, 0x47);
EMBED_RESOURCE(config, "config.txt", 'd', 'e', 'f', 'a', 'u', 'l', 't', '\0');

void print_resource(const Resource* res)
{
    printf("Resource: %s\n", res->name);
    printf("Size: %zu bytes\n", res->size);
    printf("First 16 bytes: ");
    for (size_t i = 0; i < 16 && i < res->size; i++) {
        printf("%02X ", res->data[i]);
    }
    printf("\n\n");
}

int main()
{
    print_resource(&logo);
    print_resource(&config);
    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：资源路径错误

```c
// 错误：资源未找到，编译失败
const uint8_t data[] = {
#embed "nonexistent.bin"
};

// 修正：使用 __has_embed 检测
#if __has_embed("nonexistent.bin") == __STDC_EMBED_FOUND__
const uint8_t data[] = {
#embed "nonexistent.bin"
};
#else
const uint8_t data[] = {0};
#endif
```

#### 错误示例 2：参数格式错误

```c
// 错误：参数之间不应该有逗号
#embed "file.bin" limit(100), prefix(0xFF)

// 修正：参数之间用空格分隔
#embed "file.bin" limit(100) prefix(0xFF)
```

#### 错误示例 3：忽略空终止符

```c
// 错误：文本可能不是以空字符结尾
const char* get_text(void) {
    static const char text[] = {
    #embed "text.txt"
    };
    return text;  // 可能导致未定义行为
}

// 修正：手动添加空终止符
const char* get_text(void) {
    static const char text[] = {
    #embed "text.txt"
    , '\0'
    };
    return text;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **C23 新特性**：`#embed` 是 C23 标准引入的预处理器指令
2. **二进制嵌入**：将二进制资源编译时嵌入源代码
3. **整数列表展开**：展开为逗号分隔的整数常量表达式
4. **四个标准参数**：`limit`、`prefix`、`suffix`、`if_empty`
5. **`__has_embed` 检测**：编译时检测资源是否存在

### 标准参数对比

| 参数 | 功能 | 作用时机 |
|------|------|----------|
| `limit` | 限制读取字节数 | 资源读取 |
| `prefix` | 插入前置标记 | 非空资源展开前 |
| `suffix` | 插入后置标记 | 非空资源展开后 |
| `if_empty` | 替换空资源 | 资源为空时 |

### `__has_embed` 返回值

| 值 | 宏常量 | 说明 |
|----|--------|------|
| 0 | `__STDC_EMBED_NOT_FOUND__` | 资源未找到或参数不支持 |
| 1 | `__STDC_EMBED_FOUND__` | 资源找到且非空 |
| 2 | `__STDC_EMBED_EMPTY__` | 资源为空 |

### 与传统方法对比

| 方法 | 优点 | 缺点 |
|------|------|------|
| `xxd -i` | 广泛支持 | 需要构建步骤 |
| `ld --format=binary` | 链接器原生 | 平台特定 |
| `#embed` | 标准化、编译时 | 需要 C23 编译器 |

### 学习建议

1. **检查编译器支持**：确保编译器支持 C23 的 `#embed`
2. **使用 `__has_embed`**：始终检测资源是否存在
3. **注意空终止符**：嵌入文本时手动添加 `'\0'`
4. **使用参数简化**：`prefix`/`suffix` 可简化数组初始化
5. **限制大文件**：使用 `limit` 防止内存问题

---

**标准参考**：
- C23 (ISO/IEC 9899:2024): 6.4.7 Header names, 6.10.1 Conditional inclusion, 6.10.2 Binary resource inclusion