# Clang-Tidy 深度指南

Clang-Tidy 是一个强大的 C++ 静态分析工具，用于检测代码中的潜在问题、风格违规和现代化改进机会。它基于 Clang 编译器框架，能提供精准的代码诊断和改进建议。

## 核心功能

1. **代码质量检查**：
   - 潜在 bug 检测（空指针解引用、内存泄漏等）
   - 性能问题识别（不必要的拷贝、循环优化）
   - 可维护性问题（复杂度过高、冗余代码）

2. **风格一致性**：
   - 强制执行编码规范
   - 与 Clang-Format 协同工作

3. **现代化迁移**：
   - C++11/14/17/20 特性建议
   - 弃用 API 替换

## 安装与基本使用

### 安装
```bash
# Linux
sudo apt-get install clang-tidy

# macOS
brew install llvm
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"

# Windows (通过 LLVM 安装包)
https://releases.llvm.org/download.html
```

### 基本命令
```bash
# 单个文件检查
clang-tidy source.cpp -- -std=c++17 -I./include

# 完整项目检查 (需 compile_commands.json)
clang-tidy -p build/ src/**/*.cpp

# 指定检查项
clang-tidy -checks='modernize-*,clang-analyzer-*' source.cpp
```

## 与 Clangd/VSCode 集成

### VSCode 配置
```json
{
  "clangd.arguments": [
    "--background-index",
    "--clang-tidy",
    "--clang-tidy-checks=modernize-*,bugprone-*,performance-*,cppcoreguidelines-*",
    "--clang-tidy-extra-arg=-std=c++17",
    "--compile-commands-dir=build"
  ]
}
```

### 常用检查类别
| 类别             | 前缀             | 示例检查项                          |
|------------------|------------------|-------------------------------------|
| 现代化改进       | `modernize-*`    | modernize-use-auto, modernize-loop-convert |
| 性能优化         | `performance-*`  | performance-unnecessary-copy-initialization |
| Bug 预防         | `bugprone-*`     | bugprone-null-pointer-dereference   |
| Google 规范      | `google-*`       | google-readability-namespace-comments |
| C++ Core Guidelines | `cppcoreguidelines-*` | cppcoreguidelines-pro-type-member-init |
| Clang 分析器     | `clang-analyzer-*` | clang-analyzer-core.NullDereference |

## 配置文件 (.clang-tidy)

创建项目级配置文件：

```yaml
# .clang-tidy
Checks: >
    modernize-*,
    bugprone-*,
    performance-*,
    -modernize-use-trailing-return-type,
    -bugprone-exception-escape
WarningsAsErrors: '*'
HeaderFilterRegex: 'src/.*'
AnalyzeTemporaryDtors: true
FormatStyle: file  # 与 .clang-format 协同
```

## 实用场景示例

### 1. 现代化改造旧代码
```bash
clang-tidy -checks='modernize-*' -fix src/legacy.cpp
```

改进点：
- 替换 `typedef` 为 `using`
- 转换 C 风格数组为 `std::array`
- 替换 `NULL` 为 `nullptr`

### 2. 性能优化
```cpp
// 优化前
std::string createString() {
    std::string s = "Hello";
    return s; // 触发拷贝
}

// clang-tidy 建议：
std::string createString() {
    return "Hello"; // 直接返回字面量
}
```

### 3. 安全加固
```cpp
void process(int* ptr) {
    *ptr = 42; // 潜在空指针解引用
}

// clang-tidy 警告：
warning: Dereference of null pointer [clang-analyzer-core.NullDereference]
```

## 高级用法

### 自定义检查规则
创建自定义检查模块：
```cpp
// MyCheck.cpp
class MyCheck : public ClangTidyCheck {
public:
    void registerMatchers(ast_matchers::MatchFinder* Finder) override {
        Finder->addMatcher(
            functionDecl(isDefinition()).bind("func"),
            this
        );
    }
    
    void check(const MatchResult& Result) override {
        const auto* Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
        if (Func->getBody()->size() > 20) {
            diag(Func->getLocation(), "函数过长，考虑拆分");
        }
    }
};
```

注册检查：
```yaml
Checks: 'myproject-*'
CheckOptions:
  myproject.FunctionLengthThreshold: 20
```

### 与构建系统集成

CMake 集成示例：
```cmake
# CMakeLists.txt
find_program(CLANG_TIDY_EXE "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY 
        ${CLANG_TIDY_EXE}
        -extra-arg=-std=c++17
        -checks=modernize-*,performance-*
    )
endif()
```

## 常见问题解决

### 1. 误报处理
```yaml
# .clang-tidy
Checks: >
    -bugprone-narrowing-conversions,
    -modernize-pass-by-value
```

或使用代码注释：
```cpp
int value = 3.14; // NOLINT(bugprone-narrowing-conversions)
```

### 2. 检查项冲突
```yaml
CheckOptions:
  modernize-use-nullptr.IgnoreMacros: true
  readability-magic-numbers.IgnoredNumbers: '1,2,3,4,8,16,32,64'
```

### 3. 性能优化
```bash
# 限制并行检查数量
clang-tidy -j 4 -p build/ src/**/*.cpp

# 仅检查修改文件
git diff --name-only | xargs clang-tidy -p build/
```

## Clang-Tidy 最佳实践

1. **渐进式采用**：
   - 从关键检查项开始（如 `bugprone-*`, `clang-analyzer-*`）
   - 逐步添加风格检查

2. **CI/CD 集成**：
   ```yaml
   # GitHub Actions 示例
   - name: Run clang-tidy
     run: |
       cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
       run-clang-tidy -p build -checks='modernize-*,bugprone-*'
   ```

3. **与编辑器实时集成**：
   - VSCode: Clang-Tidy 扩展
   - CLion: 内置支持
   - Vim: ALE 插件

4. **定期扫描**：
   ```bash
   # 每周全量扫描
   clang-tidy -p build/ --export-fixes=tidy_fixes.yaml
   clang-apply-replacements tidy_fixes.yaml
   ```

Clang-Tidy 是现代 C++ 开发不可或缺的工具链组成部分。结合 Clang-Format 和 Clangd，它能显著提升代码质量、一致性和可维护性。根据团队需求定制检查规则，并将其集成到开发工作流中，可最大化其价值。