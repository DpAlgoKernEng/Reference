# 用户 API - REST 接口文档

## 1. 接口概述

用户管理 API，提供用户的创建、查询、更新和删除功能。

| 项目 | 说明 |
|------|------|
| 基础 URL | `https://api.example.com/v1` |
| 数据格式 | JSON |
| 字符编码 | UTF-8 |

### 端点列表

| 方法 | 端点 | 说明 |
|------|------|------|
| GET | `/users` | 获取用户列表 |
| GET | `/users/{id}` | 获取单个用户 |
| POST | `/users` | 创建用户 |
| PUT | `/users/{id}` | 更新用户 |
| DELETE | `/users/{id}` | 删除用户 |

## 2. 认证与权限

### 认证方式

使用 Bearer Token 认证：

```
Authorization: Bearer <access_token>
```

### 获取 Token

```bash
POST /auth/token
Content-Type: application/json

{
  "client_id": "your_client_id",
  "client_secret": "your_client_secret"
}
```

### 权限要求

| 端点 | 所需权限 |
|------|----------|
| GET /users | `users:read` |
| POST /users | `users:write` |
| PUT /users/{id} | `users:write` |
| DELETE /users/{id} | `users:admin` |

## 3. 请求参数

### GET /users - 查询参数

| 参数 | 类型 | 必填 | 默认值 | 说明 |
|------|------|------|--------|------|
| page | integer | 否 | 1 | 页码 |
| limit | integer | 否 | 20 | 每页数量（最大 100） |
| status | string | 否 | - | 状态筛选：active/inactive |
| sort | string | 否 | created_at | 排序字段 |
| order | string | 否 | desc | 排序方向：asc/desc |

### POST /users - 请求体

```json
{
  "name": "string",       // 必填，2-50 字符
  "email": "string",      // 必填，有效邮箱格式
  "password": "string",   // 必填，至少 8 字符
  "role": "string",       // 可选，默认 "user"
  "avatar": "string"      // 可选，头像 URL
}
```

### PUT /users/{id} - 请求体

```json
{
  "name": "string",       // 可选
  "email": "string",      // 可选
  "role": "string",       // 可选
  "status": "string"      // 可选
}
```

## 4. 响应格式

### 成功响应

```json
{
  "code": 200,
  "message": "success",
  "data": {
    "id": "usr_abc123",
    "name": "John Doe",
    "email": "john@example.com",
    "role": "user",
    "status": "active",
    "created_at": "2024-01-15T10:30:00Z",
    "updated_at": "2024-01-15T10:30:00Z"
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
      "total": 100,
      "total_pages": 5
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
    }
  ]
}
```

### HTTP 状态码

| 状态码 | 说明 |
|--------|------|
| 200 | 成功 |
| 201 | 创建成功 |
| 400 | 请求参数错误 |
| 401 | 未认证 |
| 403 | 无权限 |
| 404 | 资源不存在 |
| 422 | 验证失败 |
| 429 | 请求频率超限 |
| 500 | 服务器错误 |

## 5. 请求示例

### cURL

```bash
# 获取用户列表
curl -X GET "https://api.example.com/v1/users?page=1&limit=10" \
  -H "Authorization: Bearer <token>" \
  -H "Content-Type: application/json"

# 创建用户
curl -X POST "https://api.example.com/v1/users" \
  -H "Authorization: Bearer <token>" \
  -H "Content-Type: application/json" \
  -d '{"name": "John", "email": "john@example.com", "password": "secret123"}'

# 更新用户
curl -X PUT "https://api.example.com/v1/users/usr_abc123" \
  -H "Authorization: Bearer <token>" \
  -H "Content-Type: application/json" \
  -d '{"name": "John Doe"}'

# 删除用户
curl -X DELETE "https://api.example.com/v1/users/usr_abc123" \
  -H "Authorization: Bearer <token>"
```

### Python

```python
import requests

base_url = "https://api.example.com/v1"
headers = {
    "Authorization": "Bearer <token>",
    "Content-Type": "application/json"
}

# 获取用户列表
response = requests.get(f"{base_url}/users", headers=headers, params={"page": 1})
users = response.json()

# 创建用户
new_user = {
    "name": "John",
    "email": "john@example.com",
    "password": "secret123"
}
response = requests.post(f"{base_url}/users", headers=headers, json=new_user)

# 更新用户
response = requests.put(
    f"{base_url}/users/usr_abc123",
    headers=headers,
    json={"name": "John Doe"}
)

# 删除用户
response = requests.delete(f"{base_url}/users/usr_abc123", headers=headers)
```

### JavaScript

```javascript
const baseUrl = 'https://api.example.com/v1';
const headers = {
  'Authorization': 'Bearer <token>',
  'Content-Type': 'application/json'
};

// 获取用户列表
const response = await fetch(`${baseUrl}/users?page=1`, { headers });
const users = await response.json();

// 创建用户
const createResponse = await fetch(`${baseUrl}/users`, {
  method: 'POST',
  headers,
  body: JSON.stringify({
    name: 'John',
    email: 'john@example.com',
    password: 'secret123'
  })
});

// 更新用户
await fetch(`${baseUrl}/users/usr_abc123`, {
  method: 'PUT',
  headers,
  body: JSON.stringify({ name: 'John Doe' })
});

// 删除用户
await fetch(`${baseUrl}/users/usr_abc123`, {
  method: 'DELETE',
  headers
});
```

## 6. 错误处理

### 错误码列表

| 错误码 | 说明 | 处理建议 |
|--------|------|----------|
| `INVALID_TOKEN` | Token 无效或过期 | 重新获取 Token |
| `PERMISSION_DENIED` | 权限不足 | 检查用户权限 |
| `VALIDATION_ERROR` | 参数验证失败 | 检查请求参数 |
| `RESOURCE_NOT_FOUND` | 资源不存在 | 检查资源 ID |
| `RATE_LIMIT_EXCEEDED` | 请求频率超限 | 实现退避重试 |

### 错误处理示例

```python
import requests
from time import sleep

def get_user(user_id, retry=3):
    for attempt in range(retry):
        response = requests.get(
            f"https://api.example.com/v1/users/{user_id}",
            headers={"Authorization": f"Bearer {token}"}
        )

        if response.status_code == 200:
            return response.json()

        elif response.status_code == 429:
            # 限流，等待后重试
            wait = int(response.headers.get('Retry-After', 60))
            sleep(wait)

        elif response.status_code == 401:
            # Token 过期，刷新后重试
            refresh_token()
            continue

        else:
            response.raise_for_status()

    raise Exception("Max retries exceeded")
```

## 7. 注意事项

### 限流策略

| 限制类型 | 阈值 | 说明 |
|----------|------|------|
| 每秒请求 | 10 | 超过返回 429 |
| 每小时请求 | 10000 | 超过返回 429 |

响应头包含限流信息：
```
X-RateLimit-Limit: 10000
X-RateLimit-Remaining: 9995
X-RateLimit-Reset: 1705312800
```

### 缓存

- GET 请求可缓存，响应头包含 `Cache-Control`
- 建议客户端实现本地缓存

### 版本兼容

- URL 中包含版本号 `/v1/`
- 版本升级时会在响应头标注 `X-API-Version`