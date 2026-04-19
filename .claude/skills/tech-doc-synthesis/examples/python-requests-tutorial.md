# 使用 Python requests 库发送 HTTP 请求

## 1. 学习目标

完成本教程后，你将能够：

- 使用 `requests` 库发送 GET、POST、PUT、DELETE 请求
- 处理请求参数、请求头、请求体
- 解析 JSON 响应和处理错误
- 使用 Session 管理持久连接

## 2. 前置知识

开始本教程前，你需要：

### 必备知识

- Python 基础语法（变量、函数、条件语句）
- 基本的 HTTP 概念（请求方法、状态码、请求头）

### 环境准备

```bash
# 安装 requests 库
pip install requests

# 验证安装
python -c "import requests; print(requests.__version__)"
```

### 测试 API

本教程使用 `https://httpbin.org` 作为测试 API，该服务专门用于测试 HTTP 请求。

## 3. 步骤详解

### 步骤 1：发送基础 GET 请求

```python
import requests

# 发送 GET 请求
response = requests.get('https://httpbin.org/get')

# 查看响应状态码
print(f"状态码: {response.status_code}")  # 200

# 查看响应内容
print(f"响应类型: {type(response.json())}")  # <class 'dict'>

# 查看响应头
print(f"Content-Type: {response.headers['Content-Type']}")
```

**关键概念：**
- `status_code`：HTTP 状态码（200 表示成功）
- `json()`：将 JSON 响应解析为 Python 字典
- `headers`：响应头信息

### 步骤 2：添加查询参数

```python
import requests

# 方式 1：直接拼接 URL
response = requests.get('https://httpbin.org/get?key1=value1&key2=value2')

# 方式 2：使用 params 参数（推荐）
params = {
    'key1': 'value1',
    'key2': 'value2',
    'page': 1,
    'size': 10
}
response = requests.get('https://httpbin.org/get', params=params)

# 查看实际请求的 URL
print(response.url)  # https://httpbin.org/get?key1=value1&key2=value2&page=1&size=10
```

**最佳实践：** 使用 `params` 参数，requests 会自动处理 URL 编码。

### 步骤 3：发送 POST 请求

```python
import requests

# 发送表单数据
form_data = {
    'username': 'alice',
    'password': 'secret'
}
response = requests.post('https://httpbin.org/post', data=form_data)
print(response.json()['form'])  # {'username': 'alice', 'password': 'secret'}

# 发送 JSON 数据
json_data = {
    'name': 'Alice',
    'email': 'alice@example.com'
}
response = requests.post('https://httpbin.org/post', json=json_data)
print(response.json()['json'])  # {'name': 'Alice', 'email': 'alice@example.com'}
```

**关键区别：**
- `data`：发送表单数据（`application/x-www-form-urlencoded`）
- `json`：发送 JSON 数据（`application/json`）

### 步骤 4：添加请求头

```python
import requests

headers = {
    'User-Agent': 'MyApp/1.0',
    'Accept': 'application/json',
    'Authorization': 'Bearer your-token-here'
}

response = requests.get(
    'https://httpbin.org/headers',
    headers=headers
)

print(response.json()['headers']['User-Agent'])    # MyApp/1.0
print(response.json()['headers']['Authorization']) # Bearer your-token-here
```

### 步骤 5：处理响应和错误

```python
import requests
from requests.exceptions import HTTPError, Timeout, ConnectionError

url = 'https://httpbin.org/status/404'

try:
    # 设置超时
    response = requests.get(url, timeout=5)

    # 检查状态码（4xx/5xx 会抛出异常）
    response.raise_for_status()

except HTTPError as e:
    print(f"HTTP 错误: {e}")
except Timeout:
    print("请求超时")
except ConnectionError:
    print("连接错误")
except Exception as e:
    print(f"其他错误: {e}")
else:
    print("请求成功")
    print(response.json())
```

**常用异常：**
| 异常 | 说明 |
|------|------|
| `HTTPError` | HTTP 状态码 4xx/5xx |
| `Timeout` | 请求超时 |
| `ConnectionError` | 网络连接问题 |

### 步骤 6：使用 Session 管理连接

```python
import requests

# 创建 Session
session = requests.Session()

# 设置全局请求头
session.headers.update({
    'User-Agent': 'MyApp/1.0'
})

# 登录获取 Cookie
login_response = session.post(
    'https://httpbin.org/post',
    data={'username': 'alice', 'password': 'secret'}
)

# 后续请求自动携带 Cookie
profile_response = session.get('https://httpbin.org/cookies')
print(profile_response.json())

# 关闭 Session
session.close()

# 或使用上下文管理器
with requests.Session() as s:
    s.get('https://httpbin.org/get')
```

**Session 优势：**
- 保持 Cookie（自动处理登录状态）
- 连接池复用（性能提升）
- 统一配置请求头

### 步骤 7：文件上传和下载

```python
import requests

# 上传文件
files = {
    'file': ('report.txt', open('report.txt', 'rb'), 'text/plain')
}
response = requests.post(
    'https://httpbin.org/post',
    files=files
)
print(response.json()['files'])

# 下载文件
response = requests.get('https://httpbin.org/image/png', stream=True)
with open('downloaded.png', 'wb') as f:
    for chunk in response.iter_content(chunk_size=8192):
        f.write(chunk)
print("文件下载完成")
```

## 4. 完整示例

以下是一个完整的 API 客户端类：

```python
import requests
from typing import Optional, Dict, Any
from requests.exceptions import HTTPError, Timeout, ConnectionError


class APIClient:
    """HTTP API 客户端封装。"""

    def __init__(
        self,
        base_url: str,
        timeout: int = 10,
        default_headers: Optional[Dict[str, str]] = None
    ):
        self.base_url = base_url.rstrip('/')
        self.timeout = timeout
        self.session = requests.Session()

        if default_headers:
            self.session.headers.update(default_headers)

    def _request(self, method: str, endpoint: str, **kwargs) -> Dict[str, Any]:
        url = f"{self.base_url}/{endpoint.lstrip('/')}"
        kwargs.setdefault('timeout', self.timeout)

        try:
            response = self.session.request(method, url, **kwargs)
            response.raise_for_status()
            return response.json()
        except HTTPError as e:
            raise HTTPError(f"API 错误: {e.response.status_code}") from e
        except Timeout:
            raise Timeout(f"请求超时: {url}")
        except ConnectionError:
            raise ConnectionError(f"无法连接: {url}")

    def get(self, endpoint: str, params: Optional[Dict] = None) -> Dict[str, Any]:
        return self._request('GET', endpoint, params=params)

    def post(self, endpoint: str, data: Optional[Dict] = None,
             json: Optional[Dict] = None) -> Dict[str, Any]:
        return self._request('POST', endpoint, data=data, json=json)

    def put(self, endpoint: str, json: Optional[Dict] = None) -> Dict[str, Any]:
        return self._request('PUT', endpoint, json=json)

    def delete(self, endpoint: str) -> Dict[str, Any]:
        return self._request('DELETE', endpoint)

    def set_token(self, token: str) -> None:
        self.session.headers['Authorization'] = f'Bearer {token}'

    def close(self) -> None:
        self.session.close()

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.close()


# 使用示例
if __name__ == '__main__':
    with APIClient('https://jsonplaceholder.typicode.com') as client:
        users = client.get('/users')
        print(f"获取到 {len(users)} 个用户")

        user = client.get('/users/1')
        print(f"用户: {user['name']}")

        new_user = client.post('/users', json={'name': 'Alice'})
        print(f"创建用户 ID: {new_user['id']}")
```

## 5. 常见问题

### Q1: 如何处理 JSON 解析错误？

```python
import requests
import json

response = requests.get('https://example.com/api')

try:
    data = response.json()
except json.JSONDecodeError:
    print("响应不是有效的 JSON")
    print(f"原始响应: {response.text}")
```

### Q2: 如何处理重定向？

```python
# 禁止自动重定向
response = requests.get('https://example.com', allow_redirects=False)

# 查看重定向历史
response = requests.get('https://example.com')
print(f"重定向次数: {len(response.history)}")
print(f"最终 URL: {response.url}")
```

### Q3: 如何设置代理？

```python
proxies = {
    'http': 'http://proxy.example.com:8080',
    'https': 'http://proxy.example.com:8080'
}
response = requests.get('https://httpbin.org/ip', proxies=proxies)
```

### Q4: 如何处理分页 API？

```python
def fetch_all_pages(base_url: str, params: Optional[Dict] = None):
    """获取所有分页数据。"""
    all_items = []
    page = 1

    while True:
        current_params = {**(params or {}), 'page': page}
        response = requests.get(base_url, params=current_params)
        data = response.json()

        items = data.get('items', [])
        if not items:
            break

        all_items.extend(items)
        page += 1

        if page > data.get('total_pages', 1):
            break

    return all_items
```

### Q5: 如何发送异步请求？

```python
# 使用 requests + concurrent.futures
from concurrent.futures import ThreadPoolExecutor
import requests

urls = ['https://httpbin.org/delay/1', 'https://httpbin.org/delay/2']

def fetch(url):
    return requests.get(url).status_code

with ThreadPoolExecutor(max_workers=3) as executor:
    results = list(executor.map(fetch, urls))

print(results)  # [200, 200]
```

## 6. 延伸学习

### 进阶主题

| 主题 | 说明 | 资源 |
|------|------|------|
| **HTTP 认证** | Basic Auth、OAuth2 | [requests Auth 文档](https://requests.readthedocs.io/en/latest/user/authentication/) |
| **Cookie 处理** | 自动管理 Cookie | [requests Cookies 文档](https://requests.readthedocs.io/en/latest/user/advanced/#cookies) |
| **SSL 证书验证** | 自定义证书、跳过验证 | [requests SSL 文档](https://requests.readthedocs.io/en/latest/user/advanced/#ssl-cert-verification) |

### 替代库

| 库 | 特点 | 适用场景 |
|---|------|----------|
| **httpx** | 同步/异步、HTTP/2 | 现代异步应用 |
| **aiohttp** | 纯异步、高性能 | asyncio 项目 |
| **urllib3** | 底层、灵活 | 自定义需求 |

### 推荐阅读

1. **官方文档**：[requests.readthedocs.io](https://requests.readthedocs.io/)
2. **HTTP 协议指南**：[MDN Web Docs - HTTP](https://developer.mozilla.org/zh-CN/docs/Web/HTTP)
3. **Python 异步编程**：[asyncio 官方文档](https://docs.python.org/zh-cn/3/library/asyncio.html)

---

**恭喜完成本教程！** 你已经掌握了使用 Python requests 库发送 HTTP 请求的核心技能。继续实践，探索更多高级用法。