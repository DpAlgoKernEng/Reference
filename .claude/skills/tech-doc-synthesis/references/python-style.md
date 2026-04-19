# Python 文档风格指南

## 术语规范

### 基础概念

| 中文 | 英文 | 说明 |
|------|------|------|
| 解释器 | interpreter | Python 执行环境 |
| 脚本 | script | Python 脚本文件 |
| 模块 | module | Python 文件 |
| 包 | package | 包含 `__init__.py` 的目录 |
| 命名空间 | namespace | 名称到对象的映射 |
| 作用域 | scope | 变量有效范围 |
| 闭包 | closure | 捕获外部变量的函数 |

### 数据类型

| 中文 | 英文 | 说明 |
|------|------|------|
| 整数 | int | 整数类型 |
| 浮点数 | float | 浮点数类型 |
| 复数 | complex | 复数类型 |
| 布尔 | bool | 布尔类型 |
| 字符串 | str | 字符串类型 |
| 字节 | bytes | 字节序列 |
| 字节数组 | bytearray | 可变字节序列 |
| None | None | 空值类型 |

### 容器类型

| 中文 | 英文 | 说明 |
|------|------|------ |
| 列表 | list | 有序可变序列 |
| 元组 | tuple | 有序不可变序列 |
| 字典 | dict | 键值对映射 |
| 集合 | set | 无序唯一元素集合 |
| 冻结集合 | frozenset | 不可变集合 |
| 列表推导式 | list comprehension | 列表生成语法 |
| 字典推导式 | dict comprehension | 字典生成语法 |
| 集合推导式 | set comprehension | 集合生成语法 |

### 函数特性

| 中文 | 英文 | 说明 |
|------|------|------|
| 函数 | function | def 定义的函数 |
| Lambda | lambda | 匿名函数 |
| 装饰器 | decorator | 函数修饰器 |
| 生成器 | generator | 惰性迭代器 |
| 协程 | coroutine | async/await 异步函数 |
| 可变参数 | *args | 位置可变参数 |
| 关键字参数 | **kwargs | 关键字可变参数 |
| 默认参数 | default argument | 参数默认值 |
| 仅位置参数 | positional-only | `/` 前的参数 |
| 仅关键字参数 | keyword-only | `*` 后的参数 |

### 面向对象

| 中文 | 英文 | 说明 |
|------|------|------|
| 类 | class | 类型定义 |
| 实例 | instance | 类的实例化对象 |
| 继承 | inheritance | 类继承关系 |
| 多重继承 | multiple inheritance | 多个父类 |
| 方法 | method | 类中定义的函数 |
| 静态方法 | static method | `@staticmethod` |
| 类方法 | class method | `@classmethod` |
| 属性 | property | `@property` |
| 魔法方法 | dunder method | `__xxx__` 特殊方法 |
| 数据类 | dataclass | `@dataclass` 装饰器 |

### 类型提示

| 中文 | 英文 | 说明 |
|------|------|------|
| 类型提示 | type hint | 类型注解 |
| 类型别名 | type alias | 类型命名 |
| 联合类型 | Union | 多种类型之一 |
| 可选类型 | Optional | 可能为 None |
| 列表类型 | List | 列表类型提示 |
| 字典类型 | Dict | 字典类型提示 |
| 可调用类型 | Callable | 可调用对象类型 |
| 任意类型 | Any | 任意类型 |
| 自身类型 | Self | Python 3.11+ 自身类型 |

### 异步编程

| 中文 | 英文 | 说明 |
|------|------|------|
| async | async | 异步函数声明 |
| await | await | 等待协程 |
| 协程 | coroutine | 异步函数 |
| 任务 | Task | 异步任务 |
| Future | Future | 异步结果 |
| 事件循环 | event loop | 异步执行循环 |
| 异步上下文管理器 | async context manager | `async with` |
| 异步迭代器 | async iterator | `async for` |

### 上下文管理

| 中文 | 英文 | 说明 |
|------|------|------|
| 上下文管理器 | context manager | with 语句支持 |
| 进入方法 | __enter__ | 进入上下文 |
| 退出方法 | __exit__ | 退出上下文 |
| 异步进入 | __aenter__ | 异步进入上下文 |
| 异步退出 | __aexit__ | 异步退出上下文 |

### 异常处理

| 中文 | 英文 | 说明 |
|------|------|------|
| 异常 | exception | 错误情况表示 |
| 基本异常 | BaseException | 所有异常基类 |
| 普通异常 | Exception | 常规异常基类 |
| 抛出 | raise | 抛出异常 |
| 捕获 | except | 捕获异常 |
| 最终执行 | finally | 无论是否异常都执行 |
| 异常链 | exception chaining | `raise ... from e` |

### 常用标准库

| 中文 | 英文 | 说明 |
|------|------|------|
| 操作系统接口 | os | 操作系统功能 |
| 路径操作 | pathlib | 面向对象的路径 |
| 文件通配 | glob | 文件模式匹配 |
| 正则表达式 | re | 正则匹配 |
| 数学函数 | math | 数学运算 |
| 随机数 | random | 随机数生成 |
| 日期时间 | datetime | 日期时间处理 |
| JSON | json | JSON 编解码 |
| 系统 | sys | 系统相关 |
| 输入输出 | io | 流操作 |
| 类型提示 | typing | 类型提示模块 |
| 数据类 | dataclasses | 数据类装饰器 |
| 集合抽象 | collections | 高级集合类型 |
| 迭代工具 | itertools | 迭代器工具 |
| 函数工具 | functools | 函数工具 |
| 上下文工具 | contextlib | 上下文管理工具 |

### 运行环境

| 中文 | 英文 | 说明 |
|------|------|------|
| pip | pip | Python 包管理器 |
| 虚拟环境 | virtual environment | 隔离的 Python 环境 |
| venv | venv | 内置虚拟环境工具 |
| requirements | requirements.txt | 依赖列表文件 |
| setup.py | setup.py | 包安装脚本 |
| pyproject.toml | pyproject.toml | 项目配置文件 |
| GIL | GIL | 全局解释器锁 |

## 代码示例规范

### 导入规范

```python
# 标准库
import os
import sys
from typing import List, Optional

# 第三方库
import numpy as np

# 本地模块
from myproject.models import User
```

### 命名规范

```python
# 类名：PascalCase
class UserManager:
    pass

# 函数/变量：snake_case
def process_data():
    pass

user_count = 0

# 常量：全大写+下划线
MAX_SIZE = 100

# 私有属性：单下划线前缀
class User:
    def __init__(self):
        self._internal = {}
```

### 类型注解

```python
from typing import List, Optional, Dict, Union

def greet(name: str) -> str:
    return f"Hello, {name}"

def find_user(user_id: int) -> Optional[User]:
    return User(user_id) if user_id > 0 else None

def process(items: List[str]) -> Dict[str, int]:
    return {item: len(item) for item in items}

# Python 3.10+ 联合类型
def parse(value: int | str) -> int:
    return int(value)
```

### 文档字符串

```python
def calculate(a: int, b: int) -> float:
    """计算两个数的比率。

    Args:
        a: 被除数
        b: 除数，不能为零

    Returns:
        两数之比

    Raises:
        ValueError: 当 b 为零时
    """
    if b == 0:
        raise ValueError("b cannot be zero")
    return a / b
```

### 类定义规范

```python
from dataclasses import dataclass

# 标准类
class User:
    def __init__(self, user_id: int, name: str) -> None:
        self.id = user_id
        self.name = name

    def __repr__(self) -> str:
        return f"User(id={self.id}, name={self.name!r})"

# 数据类（Python 3.7+）
@dataclass
class Product:
    id: int
    name: str
    price: float
```

## 版本标注

### Python 3.6+

```python
name = "Alice"
print(f"Hello, {name}")  # f-string
count: int = 0           # 变量注解
```

### Python 3.8+

```python
if (n := len(items)) > 10:  # 海象运算符
    print(f"Too many: {n}")

def greet(name, /, greeting="Hello"):  # 位置限定参数
    return f"{greeting}, {name}"
```

### Python 3.10+

```python
match status:              # match 语句
    case 200: print("OK")
    case _: print("Other")

def parse(value: int | str) -> int:  # 联合类型语法
    return int(value)
```

### Python 3.11+

```python
from typing import Self

class Builder:
    def set_name(self, name: str) -> Self:  # Self 类型
        self.name = name
        return self
```

## 装饰器

```python
from functools import wraps

def log(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        return func(*args, **kwargs)
    return wrapper

@log
def greet(name: str) -> str:
    return f"Hello, {name}"

# 带参数的装饰器
def retry(max_attempts: int = 3):
    def decorator(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            for _ in range(max_attempts):
                try:
                    return func(*args, **kwargs)
                except Exception:
                    continue
            raise RuntimeError("Max retries exceeded")
        return wrapper
    return decorator
```

## 上下文管理器

```python
from contextlib import contextmanager

# 使用类
class Timer:
    def __enter__(self):
        import time
        self.start = time.time()
        return self

    def __exit__(self, *args):
        print(f"Elapsed: {time.time() - self.start:.2f}s")

# 使用装饰器
@contextmanager
def timer():
    import time
    start = time.time()
    yield
    print(f"Elapsed: {time.time() - start:.2f}s")

with timer():
    # 执行操作
    pass
```

## 生成器与迭代器

```python
# 生成器函数
def count_up(n: int):
    for i in range(n):
        yield i

# 生成器表达式
squares = (x * x for x in range(10))

# 迭代器协议
class Range:
    def __init__(self, n: int):
        self.n = n
        self.i = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self.i >= self.n:
            raise StopIteration
        value = self.i
        self.i += 1
        return value
```

## 异步编程

```python
import asyncio

async def fetch_data(url: str) -> dict:
    await asyncio.sleep(1)
    return {"url": url}

async def fetch_all(urls: list[str]) -> list[dict]:
    tasks = [fetch_data(url) for url in urls]
    return await asyncio.gather(*tasks)

async def main():
    urls = ["url1", "url2"]
    results = await fetch_all(urls)
    print(results)

asyncio.run(main())
```

## 错误处理

```python
# ✅ 正确：具体异常
try:
    value = int(input_string)
except ValueError as e:
    print(f"Invalid: {e}")
    value = 0

# ✅ 正确：多个异常
try:
    file = open(path)
except FileNotFoundError:
    print("File not found")
except PermissionError:
    print("Permission denied")

# ❌ 错误：空 except
try:
    do_something()
except:
    pass  # 隐藏所有错误
```

## 推导式

```python
# 列表推导式
squares = [x * x for x in range(10)]
evens = [x for x in range(20) if x % 2 == 0]

# 字典推导式
word_lengths = {word: len(word) for word in ["apple", "banana"]}

# 集合推导式
unique_lengths = {len(word) for word in ["apple", "banana"]}
```

## 注意事项格式

```python
# ❌ 错误示例
# 说明错误原因

# ✅ 正确示例
# 说明正确做法
```

## 性能注意事项

| 场景 | 建议 |
|------|------|
| 大列表操作 | 使用生成器减少内存 |
| 字符串拼接 | 使用 `str.join()` |
| 成员检测 | 使用集合或字典 |
| I/O 操作 | 使用 asyncio |
| CPU 密集 | 使用多进程 |

## 工具与构建

```bash
# 包管理
pip install -r requirements.txt
poetry install

# 虚拟环境
python -m venv .venv
source .venv/bin/activate

# 代码质量
black src/
mypy src/
pytest tests/
```