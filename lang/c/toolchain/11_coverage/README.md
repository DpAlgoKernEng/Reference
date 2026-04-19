# 代码覆盖率工具

本目录收录 C 代码覆盖率分析工具文档。

## 工具概览

| 工具 | 说明 | 用途 |
|------|------|------|
| [gcov](gcov.md) | GCC 覆盖率工具 | 分析代码执行覆盖率 |
| [lcov](lcov.md) | 覆盖率可视化 | 生成 HTML 覆盖率报告 |

## 工具对比

| 特性 | gcov | lcov |
|------|------|------|
| 命令行输出 | ✅ | ❌ |
| HTML 报告 | ❌ | ✅ |
| 图形化展示 | ❌ | ✅ |
| GCC 支持 | ✅ | ✅（基于 gcov） |
| Clang 支持 | ❌ | 部分 |

## 覆盖率类型

### 行覆盖率（Line Coverage）

度量代码行被执行的比例，是最基础的覆盖率指标。

```
Lines executed:85.71% of 14
```

### 分支覆盖率（Branch Coverage）

度量条件分支（if/switch）各路径被执行的比例。

```
Branches executed:100.00% of 6
Taken at least once:66.67% of 6
```

### 函数覆盖率（Function Coverage）

度量函数被调用的比例。

```
Functions called:100.00% of 5
```

## 基本工作流

```
1. 编译程序（启用覆盖率选项）
        ↓
2. 运行测试程序
        ↓
3. 生成 .gcda 覆盖率数据
        ↓
4. 使用 gcov 分析数据
        ↓
5. 使用 lcov 生成 HTML 报告
```

## 快速示例

### gcov 基本用法

```bash
# 编译（启用覆盖率）
gcc --coverage program.c -o program

# 运行测试
./program

# 生成报告
gcov program.c
```

### lcov 完整流程

```bash
# 编译
gcc --coverage program.c -o program

# 运行测试
./program

# 收集数据
lcov --capture --directory . --output-file coverage.info

# 过滤系统头文件
lcov --remove coverage.info '/usr/*' --output-file coverage.info

# 生成 HTML 报告
genhtml coverage.info --output-directory out
```

## CMake 集成

```cmake
option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    add_compile_options(-fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
    
    add_custom_target(coverage
        COMMAND lcov --zerocounters --directory ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} 
                --output-file coverage.info
        COMMAND lcov --remove coverage.info '/usr/*' 
                --output-file coverage.info
        COMMAND genhtml coverage.info 
                --output-directory coverage_html
                --title "Coverage Report" --legend
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
```

## 编译选项

| 选项 | 说明 |
|------|------|
| `-fprofile-arcs` | 记录程序执行路径 |
| `-ftest-coverage` | 生成覆盖率数据文件 |
| `--coverage` | 等同于上面两个 + 链接选项 |

## 文件类型

| 文件 | 说明 |
|------|------|
| `.gcno` | 编译时生成的注释文件 |
| `.gcda` | 运行时生成的数据文件 |
| `.gcov` | gcov 生成的文本报告 |

## 覆盖率目标建议

| 项目类型 | 推荐覆盖率 |
|----------|-----------|
| 关键系统 | ≥ 90% |
| 一般项目 | ≥ 80% |
| 快速原型 | ≥ 60% |
| 测试代码 | 可不统计 |

## CI/CD 集成

### GitHub Actions

```yaml
- name: Build with coverage
  run: cmake -B build -DENABLE_COVERAGE=ON && cmake --build build

- name: Run tests
  run: cd build && ctest --output-on-failure

- name: Generate coverage
  run: |
    cd build
    lcov --capture --directory . --output-file coverage.info
    lcov --remove coverage.info '/usr/*' --output-file coverage.info
    genhtml coverage.info --output-directory coverage_html

- name: Upload coverage
  uses: actions/upload-artifact@v3
  with:
    name: coverage
    path: build/coverage_html
```

## 最佳实践

1. **分离构建目录**：覆盖率构建与普通构建分开
2. **过滤噪音**：移除系统头文件、测试代码
3. **设置阈值**：项目覆盖率目标，CI 中检查
4. **定期检查**：持续跟踪覆盖率变化
5. **覆盖关键路径**：优先覆盖核心业务逻辑

## 相关文档

- [测试框架](../07_test/) - GoogleTest、Catch2 等
- [静态分析](../05_static_analysis/) - clang-tidy、cppcheck 等
- [动态分析](../06_dynamic_analysis/) - Sanitizers、Valgrind 等