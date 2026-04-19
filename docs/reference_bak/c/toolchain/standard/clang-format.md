### 代码格式化 clang-format 

主流组织/项目的 **clang-format 代码格式化规范**对比，涵盖 **GNU、LLVM、Google、Microsoft、Chromium、Mozilla、WebKit** 的风格差异及关键参数解析：

---

### **1. 核心规范对比表**
| 风格名称      | 缩进 | 大括号风格       | 指针对齐   | 行宽限制 | 命名约定           | 典型项目       |
|---------------|------|------------------|------------|----------|--------------------|----------------|
| **GNU**       | 2    | 左大括号换行     | 左对齐     | 80       | 小写下划线         | GCC, Glibc    |
| **LLVM**      | 4    | 左大括号不换行   | 左对齐     | 80       | 帕斯卡命名（类）   | LLVM, Clang   |
| **Google**    | 2    | 左大括号不换行   | 右对齐     | 80       | 小写下划线         | Protobuf      |
| **Microsoft** | 4    | 左大括号换行     | 右对齐     | 120      | 驼峰命名           | Windows SDK   |
| **Chromium**  | 2    | 左大括号不换行   | 右对齐     | 80       | 小写下划线         | Chrome        |
| **Mozilla**   | 2    | 左大括号换行     | 右对齐     | 100      | 混合风格           | Firefox       |
| **WebKit**    | 4    | 左大括号换行     | 右对齐     | 120      | 帕斯卡命名（类）   | Safari        |

---

### **2. 关键参数详解**
#### **（1）缩进与空格**
- **GNU**：`IndentWidth: 2`，`UseTab: Never`  
  ```cpp
  if (condition)
    {
      do_something();  // 左大括号换行 + 2 空格缩进
    }
  ```
- **LLVM**：`IndentWidth: 4`，`BreakBeforeBraces: Allman`（部分项目自定义）  
  ```cpp
  if (condition) {
      doSomething();  // 左大括号不换行 + 4 空格缩进
  }
  ```

#### **（2）指针与引用对齐**
- **左对齐（LLVM/GNU）**：  
  ```cpp
  int* p;      // 符号紧贴类型
  void foo(int& x);
  ```
- **右对齐（Google/Chromium）**：  
  ```cpp
  int *p;      // 符号紧贴变量
  void foo(int &x);
  ```

#### **（3）大括号风格**
- **Attach（Google/LLVM）**：左大括号不换行  
  ```cpp
  if (x > 0) {
      // ...
  }
  ```
- **Linux（GNU）**：左大括号换行  
  ```cpp
  if (x > 0)
  {
      // ...
  }
  ```

#### **（4）行宽与换行**
- **严格行宽（GNU/LLVM）**：`ColumnLimit: 80`  
  ```cpp
  // 超出行宽自动换行
  int result = veryLongFunctionName(parameter1, parameter2,
                                    parameter3);
  ```
- **宽松行宽（Microsoft/WebKit）**：`ColumnLimit: 120`  
  ```cpp
  // 允许较长单行代码
  auto result = SomeTemplateType<VeryLongTypeName>::CreateInstance(arg1, arg2, arg3);
  ```

---

### **3. 配置文件示例**
#### **Google 风格**
```yaml
BasedOnStyle: Google
Language: Cpp
ColumnLimit: 80
PointerAlignment: Right
...
```

#### **Chromium 风格**
```yaml
# 基于 Google 风格调整
BasedOnStyle: Google
DerivePointerAlignment: false
PointerAlignment: Right
AllowShortIfStatementsOnASingleLine: false
...
```

#### **Mozilla 混合风格**
```yaml
# 结合 Google 和自定义规则
BasedOnStyle: Google
ColumnLimit: 100
BreakBeforeBraces: Custom
BraceWrapping:
  AfterFunction: true
  AfterClass: true
...
```

---

### **4. 工具使用示例**
#### **应用 LLVM 风格**
```bash
clang-format -style=LLVM -i *.cpp
```

#### **生成 Chromium 风格配置文件**
```bash
clang-format -style=Chromium -dump-config > .clang-format
```

#### **自定义混合风格**
```yaml
# .clang-format
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 100
BreakBeforeBraces: Allman
...
```

---

### **5. 规范选择建议**
- **开源项目贡献**：直接使用对应项目的 `.clang-format` 文件  
- **企业项目**：  
  - **C/C++ 库**：推荐 LLVM 或 Google 风格  
  - **Windows 应用**：推荐 Microsoft 风格  
  - **跨平台项目**：Chromium 风格  
- **个人项目**：基于 Google/LLVM 调整，保持团队统一

---

通过合理配置 `clang-format`，可显著提升代码一致性。建议结合 CI 工具（如 GitHub Actions）强制格式化检查