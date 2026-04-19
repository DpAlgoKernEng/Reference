# Web API 文档风格指南

## 术语规范

### 基础概念

| 中文 | 英文 | 说明 |
|------|------|------|
| API | Application Programming Interface | 应用程序接口 |
| REST | Representational State Transfer | 表述性状态转移 |
| RESTful | RESTful | 遵循 REST 原则的 API |
| 端点 | endpoint | API 地址 |
| 资源 | resource | API 操作的对象 |
| 表示 | representation | 资源的数据格式 |

### 请求相关

| 中文 | 英文 | 说明 |
|------|------|------|
| 请求 | request | 客户端发送的消息 |
| 请求方法 | HTTP method | GET/POST/PUT/DELETE 等 |
| 请求头 | request header | 请求元数据 |
| 请求体 | request body | 请求数据 |
| 查询参数 | query parameter | URL `?` 后的参数 |
| 路径参数 | path parameter | URL 路径中的变量 |
| 请求头参数 | header parameter | HTTP 头部参数 |

### 响应相关

| 中文 | 英文 | 说明 |
|------|------|------|
| 响应 | response | 服务端返回的消息 |
| 响应头 | response header | 响应元数据 |
| 响应体 | response body | 响应数据 |
| 状态码 | status code | HTTP 状态码 |
| 响应时间 | response time | 请求处理时间 |

### 认证授权

| 中文 | 英文 | 说明 |
|------|------|------|
| 认证 | authentication | 身份验证 |
| 授权 | authorization | 权限验证 |
| API Key | API Key | API 密钥 |
| Token | token | 访问令牌 |
| Bearer Token | Bearer Token | 持有者令牌 |
| JWT | JSON Web Token | JSON 网络令牌 |
| OAuth | OAuth | 开放授权协议 |
| 刷新令牌 | refresh token | 用于刷新访问令牌 |
| 作用域 | scope | 权限范围 |

### 数据格式

| 中文 | 英文 | 说明 |
|------|------|------|
| JSON | JSON | JavaScript 对象表示法 |
| XML | XML | 可扩展标记语言 |
| 表单数据 | form data | `application/x-www-form-urlencoded` |
| 多部分表单 | multipart/form-data | 文件上传格式 |
| 内容类型 | Content-Type | 数据格式声明 |

### 错误处理

| 中文 | 英文 | 说明 |
|------|------|------|
| 错误码 | error code | 错误标识符 |
| 错误消息 | error message | 错误描述 |
| 错误响应 | error response | 错误返回数据 |
| 验证错误 | validation error | 参数验证失败 |
| 业务错误 | business error | 业务逻辑错误 |
| 服务错误 | service error | 服务端错误 |

### 限流与缓存

| 中文 | 英文 | 说明 |
|------|------|------|
| 限流 | rate limiting | 请求频率限制 |
| 配额 | quota | 请求配额 |
| 速率限制 | rate limit | 单位时间请求数 |
| 重试 | retry | 重试机制 |
| 退避 | backoff | 重试间隔策略 |
| 缓存 | cache | 数据缓存 |
| 缓存控制 | Cache-Control | 缓存策略头 |
| ETag | ETag | 资源版本标识 |

### 版本与兼容

| 中文 | 英文 | 说明 |
|------|------|------|
| API 版本 | API version | API 版本号 |
| 版本控制 | versioning | 版本管理策略 |
| 向后兼容 | backward compatibility | 不影响现有客户端 |
| 废弃 | deprecation | 标记将移除 |
| 迁移指南 | migration guide | 升级指导 |

## 请求方法规范

### GET - 获取资源

```http
GET /api/v1/users?page=1&limit=20
Host: api.example.com
Authorization: Bearer <token>
Accept: application/json
```

### POST - 创建资源

```http
POST /api/v1/users
Host: api.example.com
Authorization: Bearer <token>
Content-Type: application/json

{
  "name": "Alice",
  "email": "alice@example.com"
}
```

### PUT - 完整更新

```http
PUT /api/v1/users/123
Host: api.example.com
Authorization: Bearer <token>
Content-Type: application/json

{
  "name": "Alice Updated",
  "email": "alice.updated@example.com"
}
```

### PATCH - 部分更新

```http
PATCH /api/v1/users/123
Host: api.example.com
Authorization: Bearer <token>
Content-Type: application/json

{
  "name": "Alice Updated"
}
```

### DELETE - 删除资源

```http
DELETE /api/v1/users/123
Host: api.example.com
Authorization: Bearer <token>
```

## 认证方式

### Bearer Token

```http
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...
```

### API Key

```http
# 方式 1：请求头
X-API-Key: your-api-key

# 方式 2：查询参数
GET /api/v1/users?api_key=your-api-key
```

### Basic Auth

```http
Authorization: Basic base64(username:password)
```

## 参数表格格式

### Query 参数

| 参数名 | 类型 | 必填 | 默认值 | 说明 |
|--------|------|------|--------|------|
| page | integer | 否 | 1 | 页码，从 1 开始 |
| limit | integer | 否 | 20 | 每页数量，最大 100 |
| sort | string | 否 | created_at | 排序字段 |
| order | string | 否 | desc | 排序方向：asc/desc |

### Path 参数

| 参数名 | 类型 | 必填 | 说明 |
|--------|------|------|------|
| id | string | 是 | 用户唯一标识 |
| userId | integer | 是 | 用户 ID |

### Request Body

```json
{
  "name": "string",        // 用户名，2-50 字符
  "email": "string",       // 邮箱地址
  "age": 0,                // 年龄（可选），18-120
  "role": "user"           // 角色：user/admin
}
```

## 响应格式

### 成功响应

```json
{
  "code": 200,
  "message": "success",
  "data": {
    "id": "123",
    "name": "John Doe",
    "email": "john@example.com",
    "created_at": "2024-01-15T10:30:00Z"
  }
}
```

### 列表响应

```json
{
  "code": 200,
  "message": "success",
  "data": {
    "items": [...],
    "pagination": {
      "page": 1,
      "limit": 20,
      "total": 150,
      "total_pages": 8
    }
  }
}
```

### 错误响应

```json
{
  "code": 400,
  "message": "Validation failed",
  "errors": [
    {
      "field": "email",
      "message": "Invalid email format"
    },
    {
      "field": "age",
      "message": "Age must be between 18 and 120"
    }
  ]
}
```

## HTTP 状态码

### 成功响应 (2xx)

| 状态码 | 说明 | 使用场景 |
|--------|------|----------|
| 200 | OK | 成功处理请求 |
| 201 | Created | 资源创建成功 |
| 202 | Accepted | 请求已接受，异步处理 |
| 204 | No Content | 成功但无返回内容（DELETE） |

### 客户端错误 (4xx)

| 状态码 | 说明 | 处理建议 |
|--------|------|----------|
| 400 | Bad Request | 检查请求格式和参数 |
| 401 | Unauthorized | 检查认证信息是否有效 |
| 403 | Forbidden | 检查是否有访问权限 |
| 404 | Not Found | 检查资源 ID 或路径 |
| 405 | Method Not Allowed | 检查请求方法 |
| 409 | Conflict | 资源冲突，如重复创建 |
| 422 | Unprocessable Entity | 参数验证失败 |
| 429 | Too Many Requests | 触发限流，稍后重试 |

### 服务端错误 (5xx)

| 状态码 | 说明 | 处理建议 |
|--------|------|----------|
| 500 | Internal Server Error | 服务端错误，联系技术支持 |
| 502 | Bad Gateway | 上游服务不可用 |
| 503 | Service Unavailable | 服务暂时不可用 |
| 504 | Gateway Timeout | 请求超时 |

## 代码示例规范

### cURL

```bash
curl -X GET "https://api.example.com/api/v1/users/123" \
  -H "Authorization: Bearer <token>" \
  -H "Content-Type: application/json" \
  -H "Accept: application/json"
```

### Python

```python
import requests

response = requests.get(
    "https://api.example.com/api/v1/users/123",
    headers={
        "Authorization": "Bearer <token>",
        "Content-Type": "application/json"
    }
)
print(response.json())
```

### JavaScript

```javascript
const response = await fetch(
  "https://api.example.com/api/v1/users/123",
  {
    headers: {
      "Authorization": "Bearer <token>",
      "Content-Type": "application/json"
    }
  }
);
const data = await response.json();
console.log(data);
```

### Java

```java
import java.net.http.*;
import java.net.URI;

HttpClient client = HttpClient.newHttpClient();
HttpRequest request = HttpRequest.newBuilder()
    .uri(URI.create("https://api.example.com/api/v1/users/123"))
    .header("Authorization", "Bearer <token>")
    .header("Content-Type", "application/json")
    .GET()
    .build();

HttpResponse<String> response = client.send(
    request,
    HttpResponse.BodyHandlers.ofString()
);
System.out.println(response.body());
```

### Go

```go
import (
    "net/http"
    "io"
)

req, _ := http.NewRequest("GET",
    "https://api.example.com/api/v1/users/123", nil)
req.Header.Set("Authorization", "Bearer <token>")
req.Header.Set("Content-Type", "application/json")

client := &http.Client{}
resp, _ := client.Do(req)
defer resp.Body.Close()
body, _ := io.ReadAll(resp.Body)
fmt.Println(string(body))
```

## 注意事项格式

```markdown
// ❌ 错误示例
// 说明错误原因

// ✅ 正确示例
// 说明正确做法
```

## API 版本控制

### URL 路径版本

```
/api/v1/users
/api/v2/users
```

### 请求头版本

```http
Accept: application/vnd.myapi.v1+json
```

### 版本废弃通知

```http
Deprecation: true
Sunset: Sat, 01 Jul 2024 00:00:00 GMT
Link: </api/v2/users>; rel="successor-version"
```

## 性能注意事项

| 场景 | 建议 |
|------|------|
| 大数据量列表 | 使用分页，避免一次性返回 |
| 频繁请求 | 实现客户端缓存 |
| 限流场景 | 实现指数退避重试 |
| 超时处理 | 设置合理的超时时间 |
| 并发请求 | 使用连接池复用连接 |

## 工具与调试

### API 测试工具

| 工具 | 说明 |
|------|------|
| Postman | GUI API 测试工具 |
| curl | 命令行 HTTP 客户端 |
| HTTPie | 更友好的 curl 替代品 |
| Insomnia | REST API 客户端 |
| Paw | macOS API 测试工具 |

### 调试技巧

```bash
# curl 详细输出
curl -v https://api.example.com/users

# 仅显示响应头
curl -I https://api.example.com/users

# 跟踪重定向
curl -L https://api.example.com/users

# 显示耗时
curl -w "Time: %{time_total}s\n" https://api.example.com/users

# 保存响应到文件
curl -o response.json https://api.example.com/users

# 携带证书调试
curl --cert client.crt --key client.key https://api.example.com
```

### Postman 使用

```javascript
// Pre-request Script（请求前脚本）
pm.environment.set('timestamp', Date.now());
pm.variables.set('token', pm.environment.get('api_token'));

// Tests Script（测试脚本）
pm.test('Status code is 200', function() {
    pm.response.to.have.status(200);
});

pm.test('Response has data', function() {
    var jsonData = pm.response.json();
    pm.expect(jsonData.data).to.exist;
});

// 环境变量管理
// 点击右上角 "Environment" → "Manage Environments" → "Add"
// 设置变量：base_url, api_token, user_id 等
```

### HTTPie 使用

```bash
# 安装
pip install httpie

# GET 请求
http GET https://api.example.com/users Authorization:"Bearer token"

# POST JSON
http POST https://api.example.com/users name="Alice" email="alice@example.com"

# 表单数据
http --form POST https://api.example.com/login username="alice" password="secret"

# 下载文件
http GET https://api.example.com/files/report.pdf --download

# 查看请求详情
http --print=hHbB GET https://api.example.com/users
# h: 请求头，H: 响应头，b: 请求体，B: 响应体
```

### 网络抓包

```bash
# Charles（macOS GUI）
# 设置代理：127.0.0.1:8888
# SSL Proxying：Enable，添加 *.example.com

# Wireshark（跨平台）
# 过滤 HTTP：http
# 过滤特定主机：http.host == "api.example.com"

# tcpdump（命令行）
tcpdump -i any -A 'host api.example.com and port 80'
tcpdump -i any -w capture.pcap 'tcp port 443'  # 保存到文件

# mitmproxy（Python 工具）
mitmproxy --mode reverse:https://api.example.com -p 8080
mitmdump -s script.py  # 使用脚本处理流量
```

### 日志分析

```bash
# Nginx 日志分析
grep "POST /api/v1/users" /var/log/nginx/access.log
awk '$9 == "500" {print $0}' access.log  # 500 错误

# 实时监控
tail -f /var/log/nginx/access.log | grep "api"

# 统计响应时间分布
awk '{print $NF}' access.log | sort -n | uniq -c
```

### Mock 服务器

```javascript
// JSON Server（快速 Mock）
// 安装：npm install -g json-server
// db.json
{
  "users": [
    { "id": 1, "name": "Alice" },
    { "id": 2, "name": "Bob" }
  ]
}

// 启动：json-server --watch db.json --port 3000
// 自动生成 REST API：
// GET    /users
// GET    /users/1
// POST   /users
// PUT    /users/1
// PATCH  /users/1
// DELETE /users/1
```

```python
# Flask Mock（Python）
from flask import Flask, jsonify, request

app = Flask(__name__)

@app.route('/api/users', methods=['GET'])
def get_users():
    return jsonify({'data': [{'id': 1, 'name': 'Mock User'}]})

@app.route('/api/users', methods=['POST'])
def create_user():
    data = request.json
    return jsonify({'id': 100, **data}), 201

if __name__ == '__main__':
    app.run(port=3000)
```