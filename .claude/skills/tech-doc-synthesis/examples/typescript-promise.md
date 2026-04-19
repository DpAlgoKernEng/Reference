# Promise - JavaScript/TypeScript 异步编程

## 1. 概述

`Promise` 是 JavaScript 中处理异步操作的核心对象，代表一个异步操作的最终完成（或失败）及其结果值。它是 ES6 标准的一部分，是现代 JavaScript 异步编程的基础。

TypeScript 提供完整的类型支持，`Promise<T>` 表示异步操作结果的类型。

与回调函数的关键区别：
- 回调函数容易形成"回调地狱"
- Promise 提供链式调用，代码更清晰
- Promise 内置错误处理机制

## 2. 来源与演变

### ES6 (ES2015) 引入

`Promise` 在 **ES6**（2015 年）中正式成为标准，统一了 JavaScript 异步编程模式。

### 设计动机

- 回调函数模式导致代码嵌套过深（回调地狱）
- 缺乏统一的异步错误处理
- 需要标准化的异步操作表示

### 主要版本变更

| 版本 | 变更 |
|------|------|
| ES6 | Promise 首次引入 |
| ES2017 | `async/await` 语法糖 |
| ES2020 | `Promise.allSettled()` |
| ES2020 | `Promise.any()` |

## 3. 语法与参数

### 创建 Promise

```typescript
// 基本创建
const promise = new Promise<string>((resolve, reject) => {
    // 异步操作
    if (success) {
        resolve('result');
    } else {
        reject(new Error('failed'));
    }
});

// 静态方法
Promise.resolve(value);        // 已完成的 Promise
Promise.reject(reason);        // 已拒绝的 Promise
```

### Promise 状态

```typescript
type PromiseState = 'pending' | 'fulfilled' | 'rejected';

// 状态一旦改变就不可逆
// pending -> fulfilled
// pending -> rejected
```

### 实例方法

| 方法 | 说明 |
|------|------|
| `then(onFulfilled, onRejected)` | 处理成功/失败 |
| `catch(onRejected)` | 捕获错误 |
| `finally(onFinally)` | 无论结果都执行 |

### 静态方法

| 方法 | 说明 |
|------|------|
| `Promise.all(promises)` | 所有成功才成功 |
| `Promise.allSettled(promises)` | 等待所有完成 |
| `Promise.race(promises)` | 第一个完成 |
| `Promise.any(promises)` | 第一个成功 |

### TypeScript 类型定义

```typescript
// Promise<T> 泛型
const fetchUser = (): Promise<User> => {
    return api.get<User>('/user').then(res => res.data);
};

// async 函数返回 Promise
async function getUser(): Promise<User> {
    return await fetchUser();
}
```

## 4. 底层原理

### 状态机模型

```
Promise 状态转换：

    ┌─────────────────────────────────────┐
    │             pending                  │
    │           (初始状态)                  │
    └───────────┬───────────┬─────────────┘
                │           │
        resolve │           │ reject
                ▼           ▼
    ┌───────────────┐  ┌───────────────┐
    │   fulfilled   │  │   rejected    │
    │   (已完成)     │  │   (已拒绝)     │
    └───────────────┘  └───────────────┘
```

### 微任务队列

```javascript
// Promise 回调在微任务队列执行
console.log('1');                          // 同步
Promise.resolve().then(() => console.log('3'));  // 微任务
console.log('2');                          // 同步
// 输出：1, 2, 3
```

### 链式调用机制

```javascript
// 每个 then 返回新的 Promise
Promise.resolve(1)
    .then(x => x + 1)    // 返回 Promise<2>
    .then(x => x * 2)    // 返回 Promise<4>
    .then(console.log);  // 4
```

### 错误冒泡

```javascript
// 错误会沿着链向下传递
Promise.resolve()
    .then(() => { throw new Error('fail'); })
    .then(() => console.log('skip'))  // 跳过
    .catch(e => console.log(e.message));  // 'fail'
```

## 5. 使用场景

### 适合使用

- API 请求
- 文件读写（Node.js）
- 定时器封装
- 异步计算

### 最佳实践

```typescript
// ✅ 正确：使用 async/await
async function fetchData(): Promise<Data> {
    const response = await fetch('/api/data');
    return response.json();
}

// ✅ 正确：并行执行
const [users, orders] = await Promise.all([
    fetchUsers(),
    fetchOrders()
]);

// ✅ 正确：错误处理
try {
    const data = await fetchData();
} catch (error) {
    console.error(error);
}
```

## 6. 代码示例

### 基础用法

```typescript
// TypeScript
interface User {
    id: number;
    name: string;
}

// 创建 Promise
const fetchUser = (id: number): Promise<User> => {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            if (id > 0) {
                resolve({ id, name: `User-${id}` });
            } else {
                reject(new Error('Invalid id'));
            }
        }, 1000);
    });
};

// 使用 then/catch
fetchUser(1)
    .then(user => {
        console.log(user.name);
        return user.id;
    })
    .then(id => {
        console.log(`ID: ${id}`);
    })
    .catch(error => {
        console.error(error.message);
    })
    .finally(() => {
        console.log('Done');
    });
```

### async/await 语法

```typescript
// async 函数返回 Promise
async function getUserData(id: number): Promise<User> {
    try {
        const user = await fetchUser(id);
        console.log(`Found: ${user.name}`);
        return user;
    } catch (error) {
        console.error('Failed to fetch user');
        throw error;
    }
}

// 并行执行
async function fetchAll(): Promise<void> {
    // ✅ 正确：并行
    const [user, orders] = await Promise.all([
        fetchUser(1),
        fetchOrders(1)
    ]);
    console.log(user, orders);
}

async function fetchOrders(userId: number): Promise<Order[]> {
    return [{ id: 1, product: 'Book' }];
}

interface Order {
    id: number;
    product: string;
}
```

### Promise.all 并行

```typescript
// 并行请求多个资源
async function fetchDashboard(userId: number): Promise<Dashboard> {
    const [user, orders, payments] = await Promise.all([
        fetchApi<User>(`/users/${userId}`),
        fetchApi<Order[]>(`/orders?userId=${userId}`),
        fetchApi<Payment[]>(`/payments?userId=${userId}`)
    ]);

    return { user, orders, payments };
}

// 泛型 fetch 封装
async function fetchApi<T>(url: string): Promise<T> {
    const response = await fetch(url);
    if (!response.ok) {
        throw new Error(`HTTP ${response.status}`);
    }
    return response.json();
}

// 类型定义
interface Dashboard {
    user: User;
    orders: Order[];
    payments: Payment[];
}

interface Payment {
    id: number;
    amount: number;
}
```

### Promise.allSettled 错误隔离

```typescript
// 部分失败不影响其他请求
async function fetchMultiple(): Promise<void> {
    const results = await Promise.allSettled([
        fetchUser(1),
        fetchUser(2),
        fetchUser(-1),  // 会失败
    ]);

    results.forEach((result, index) => {
        if (result.status === 'fulfilled') {
            console.log(`User ${index + 1}:`, result.value.name);
        } else {
            console.error(`User ${index + 1} failed:`, result.reason.message);
        }
    });
}
```

### Promise.race 超时控制

```typescript
// 超时控制
function fetchWithTimeout<T>(
    promise: Promise<T>,
    timeout: number
): Promise<T> {
    const timeoutPromise = new Promise<never>((_, reject) => {
        setTimeout(() => {
            reject(new Error(`Timeout after ${timeout}ms`));
        }, timeout);
    });

    return Promise.race([promise, timeoutPromise]);
}

// 使用
const user = await fetchWithTimeout(
    fetchUser(1),
    5000  // 5秒超时
);
```

### 实际应用：重试机制

```typescript
// 带重试的请求
async function fetchWithRetry<T>(
    fn: () => Promise<T>,
    retries: number = 3,
    delay: number = 1000
): Promise<T> {
    for (let i = 0; i < retries; i++) {
        try {
            return await fn();
        } catch (error) {
            if (i === retries - 1) {
                throw error;
            }
            await new Promise(resolve => setTimeout(resolve, delay));
            delay *= 2;  // 指数退避
        }
    }
    throw new Error('Max retries reached');
}

// 使用
const user = await fetchWithRetry(
    () => fetchUser(1),
    3,
    1000
);
```

## 7. 总结

`Promise` 提供强大的异步编程能力：

| 特性 | 回调函数 | Promise | async/await |
|------|----------|---------|-------------|
| 链式调用 | ❌ 嵌套 | ✅ | ✅ |
| 错误处理 | ⚠️ 手动 | ✅ catch | ✅ try/catch |
| 并行控制 | ❌ | ✅ all/allSettled | ✅ |
| 可读性 | ❌ 低 | ⚠️ 中 | ✅ 高 |
| 调试 | ❌ 困难 | ⚠️ 中等 | ✅ 容易 |

选择建议：
- 新代码 → `async/await`
- 并行任务 → `Promise.all`
- 错误隔离 → `Promise.allSettled`
- 超时控制 → `Promise.race`
- 容错获取 → `Promise.any`