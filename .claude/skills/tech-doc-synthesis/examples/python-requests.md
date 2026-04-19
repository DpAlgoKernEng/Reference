# requests - Python HTTP 库

## 1. 概述

`requests` 是 Python 最流行的 HTTP 客户端库，以简洁的 API 设计著称。它简化了 HTTP 请求的发送和响应处理，是 Python 标准库 `urllib` 的高级封装。

安装：`pip install requests`

## 2. 来源与演变

### 历史背景

- **2011 年 2 月**：Kenneth Reitz 发布 requests
- 设计理念："HTTP for Humans" —— 让 HTTP 请求变得简单直观
- 作者 Kenneth Reitz 曾被称为 "Python 界最帅程序员"，requests 是他最著名的作品

### 版本里程碑

| 版本 | 发布时间 | 重要变更 |
|------|----------|----------|
| 0.x | 2011 | 初始版本，奠定 API 设计 |
| 1.0 | 2012 | 重构内部架构，稳定性提升 |
| 2.0 | 2013 | Session 对象、Cookie 持久化 |
| 2.4 | 2014 | JSON 请求体支持、响应改进 |
| 2.10 | 2016 |PrepareRequest 模式、钩子系统 |
| 2.18 | 2017 | urllib3 集成优化 |
| 2.25 | 2020 | Python 3.6+ 支持、性能优化 |
| 2.28 | 2022 | 安全性增强、编码处理改进 |
| 2.31 | 2023 | SSRF 安全修复 |

### 设计动机

Python 标准库 `urllib` API 复杂，requests 旨在提供：
- 简洁的 API：`requests.get()` vs `urllib.request.urlopen()`
- 自动处理编码：响应自动解码为 UTF-8
- 完善的会话支持：跨请求保持状态
- 简单的 JSON 处理：内置 `json()` 方法

### 底层依赖

requests 底层使用 `urllib3` 作为 HTTP 客户端：
- `urllib3` 提供连接池、重试机制、线程安全
- requests 封装 urllib3，提供更友好的 API
- requests 2.x 后 urllib3 成为必需依赖

### 生态演进

| 库 | 说明 |
|---|------|
| `urllib` | Python 标准库，API 繁琐 |
| `urllib2` | Python 2.x HTTP 库（已废弃） |
| `urllib3` | requests 底层依赖，功能强大 |
| `httplib2` | Google 出品，支持缓存 |
| `httpx` | 异步 HTTP 库，requests 风格 API |
| `aiohttp` | asyncio 异步 HTTP 客户端 |
| `treq` | Twisted 异步 HTTP 客户端 |

### 现代替代

```python
# httpx（同步/异步统一 API）
import httpx
response = httpx.get('https://api.example.com')

# httpx 异步
async with httpx.AsyncClient() as client:
    response = await client.get('https://api.example.com')

# aiohttp（纯异步）
import aiohttp
async with aiohttp.ClientSession() as session:
    async with session.get('https://api.example.com') as resp:
        data = await resp.json()
```

### 社区影响

- GitHub Star 数：超过 50k
- 依赖项目数：超过 1M（PyPI 统计）
- Python HTTP 库事实标准
- 被广泛用于 API 调用、爬虫、自动化测试

## 3. 语法与参数

### 核心方法

```python
import requests

# GET 请求
requests.get(url, params=None, **kwargs)

# POST 请求
requests.post(url, data=None, json=None, **kwargs)

# 其他方法
requests.put(url, data=None, **kwargs)
requests.delete(url, **kwargs)
requests.patch(url, data=None, **kwargs)
```

### 常用参数

| 参数 | 类型 | 说明 |
|------|------|------|
| `url` | str | 请求 URL |
| `params` | dict | URL 查询参数 |
| `data` | dict/list | 请求体（表单） |
| `json` | dict | 请求体（JSON） |
| `headers` | dict | 请求头 |
| `cookies` | dict | Cookies |
| `timeout` | float/tuple | 超时时间 |
| `auth` | tuple | 认证信息 |

### Response 对象

| 属性/方法 | 说明 |
|-----------|------|
| `status_code` | HTTP 状态码 |
| `text` | 文本响应 |
| `content` | 字节响应 |
| `json()` | 解析 JSON 响应 |
| `headers` | 响应头 |
| `cookies` | 响应 Cookies |

## 4. 底层原理

### 请求流程

```
1. 构建请求（URL、Headers、Body）
2. 建立 TCP 连接（连接池复用）
3. 发送 HTTP 请求
4. 接收响应
5. 自动处理编码和压缩
```

### 连接池

使用 `urllib3` 的连接池，支持：
- Keep-Alive 连接复用
- 自动重试
- SSL 验证

### Session 机制

```python
# Session 跨请求保持
with requests.Session() as session:
    session.post(login_url, data=credentials)
    session.get(protected_url)  # 自动携带登录状态
```

## 5. 使用场景

### 适合场景

- Web API 调用
- 网页抓取
- 自动化测试
- 微服务通信

### 最佳实践

```python
# ✅ 使用 Session 复用连接
with requests.Session() as s:
    s.get(url)

# ✅ 设置超时
response = requests.get(url, timeout=10)

# ✅ 使用 with 确保资源释放
with requests.get(url, stream=True) as r:
    r.iter_content()
```

### 注意事项

- 大文件下载使用 `stream=True`
- 敏感信息使用环境变量
- 设置合理超时避免阻塞

## 6. 代码示例

### 基础用法

```python
import requests

# GET 请求
response = requests.get('https://api.github.com')
print(response.status_code)  # 200
print(response.json())       # 解析 JSON

# 带参数的 GET
response = requests.get(
    'https://httpbin.org/get',
    params={'key': 'value', 'page': 1}
)

# POST JSON
response = requests.post(
    'https://httpbin.org/post',
    json={'name': 'Alice', 'age': 25}
)

# 处理响应
if response.status_code == 200:
    data = response.json()
```

### 高级用法

```python
import requests
from requests.adapters import HTTPAdapter
from urllib3.util.retry import Retry

# 创建带重试的 Session
session = requests.Session()
retry = Retry(
    total=3,
    backoff_factor=0.5,
    status_forcelist=[500, 502, 503, 504]
)
adapter = HTTPAdapter(max_retries=retry)
session.mount('http://', adapter)
session.mount('https://', adapter)

# 认证
response = session.get(
    'https://api.example.com/user',
    auth=('username', 'password'),
    headers={'User-Agent': 'MyApp/1.0'},
    timeout=10
)

# 流式下载大文件
response = session.get(
    'https://example.com/large_file.zip',
    stream=True
)
with open('file.zip', 'wb') as f:
    for chunk in response.iter_content(chunk_size=8192):
        f.write(chunk)
```

### 错误处理

```python
import requests
from requests.exceptions import RequestException

try:
    response = requests.get(
        'https://api.example.com/data',
        timeout=5
    )
    response.raise_for_status()  # 检查状态码
    data = response.json()
except requests.exceptions.Timeout:
    print("请求超时")
except requests.exceptions.ConnectionError:
    print("连接错误")
except requests.exceptions.HTTPError as e:
    print(f"HTTP 错误: {e}")
except requests.exceptions.RequestException as e:
    print(f"请求异常: {e}")
```

## 7. 总结

| 特性 | requests | urllib |
|------|----------|--------|
| API 简洁度 | ✅ 优秀 | ❌ 复杂 |
| JSON 支持 | ✅ 内置 | ❌ 手动 |
| 会话管理 | ✅ Session | ❌ 无 |
| 连接池 | ✅ 自动 | ❌ 无 |

核心优势：
- API 简洁直观
- 自动处理编码
- 完善的异常处理
- 活跃的社区支持