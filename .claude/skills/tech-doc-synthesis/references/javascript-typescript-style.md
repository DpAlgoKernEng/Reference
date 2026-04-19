# JavaScript/TypeScript 文档风格指南

## 术语规范

### 基础概念

| 中文 | 英文 | 说明 |
|------|------|------|
| 变量 | variable | 数据存储容器 |
| 常量 | constant | 不可变变量 |
| 作用域 | scope | 变量有效范围 |
| 闭包 | closure | 捕获外部变量的函数 |
| 提升 | hoisting | 声明提升到作用域顶部 |
| 严格模式 | strict mode | 严格执行模式 |

### 数据类型

| 中文 | 英文 | 说明 |
|------|------|------|
| 原始类型 | primitive type | 基础数据类型 |
| 对象 | object | 键值对集合 |
| 数组 | array | 有序集合 |
| 函数 | function | 代码块执行单元 |
| 符号 | Symbol | ES6 唯一标识符 |
| 大整数 | BigInt | 任意精度整数 |
| null | null | 空值 |
| undefined | undefined | 未定义值 |

### 函数特性

| 中文 | 英文 | 说明 |
|------|------|------|
| 函数 | function | 代码块执行单元 |
| 箭头函数 | arrow function | `=>` 箭头函数 |
| 回调函数 | callback function | 作为参数传递的函数 |
| 高阶函数 | higher-order function | 接收/返回函数的函数 |
| 立即执行函数 | IIFE | 立即调用函数表达式 |
| 剩余参数 | rest parameters | `...args` 剩余参数 |
| 默认参数 | default parameter | 参数默认值 |
| 解构 | destructuring | 从对象/数组提取值 |

### 异步编程

| 中文 | 英文 | 说明 |
|------|------|------|
| Promise | Promise | 异步操作表示 |
| async | async | 异步函数声明 |
| await | await | 等待 Promise |
| 回调 | callback | 异步回调函数 |
| 微任务 | microtask | Promise 回调队列 |
| 宏任务 | macrotask | setTimeout 等队列 |
| 事件循环 | event loop | 异步执行机制 |

### 面向对象

| 中文 | 英文 | 说明 |
|------|------|------|
| 类 | class | ES6 类语法 |
| 构造函数 | constructor | 类构造方法 |
| 继承 | extends | 类继承 |
| 超类 | super | 父类引用 |
| 静态方法 | static method | 类级别方法 |
| 私有字段 | private field | `#` 私有属性 |
| Getter | getter | 取值函数 |
| Setter | setter | 设值函数 |

### 模块系统

| 中文 | 英文 | 说明 |
|------|------|------|
| 模块 | module | ES6+ 代码组织单元 |
| 导入 | import | 引入模块 |
| 导出 | export | 导出模块成员 |
| 默认导出 | default export | 默认导出成员 |
| 命名导出 | named export | 命名导出成员 |
| 动态导入 | dynamic import | 运行时导入 |
| 重新导出 | re-export | 转发导出 |

### TypeScript 类型

| 中文 | 英文 | 说明 |
|------|------|------|
| 类型注解 | type annotation | 类型声明 |
| 类型推断 | type inference | 自动类型推导 |
| 接口 | interface | TypeScript 类型定义 |
| 类型别名 | type alias | TypeScript 类型命名 |
| 泛型 | generics | TypeScript 类型参数化 |
| 联合类型 | union type | `A \| B` 或类型 |
| 交叉类型 | intersection type | `A & B` 与类型 |
| 字面量类型 | literal type | 具体值类型 |
| 可选属性 | optional property | `?` 可选字段 |
| 只读属性 | readonly | 只读属性 |
| 类型守卫 | type guard | 运行时类型检查 |
| 类型断言 | type assertion | `as` 类型断言 |

### TypeScript 工具类型

| 中文 | 英文 | 说明 |
|------|------|------|
| 部分 | Partial<T> | 所有属性可选 |
| 必需 | Required<T> | 所有属性必需 |
| 只读 | Readonly<T> | 所有属性只读 |
| 选取 | Pick<T, K> | 选取部分属性 |
| 忽略 | Omit<T, K> | 忽略部分属性 |
| 记录 | Record<K, T> | 键值对类型 |
| 排除 | Exclude<T, U> | 排除部分类型 |
| 提取 | Extract<T, U> | 提取部分类型 |
| 不可为空 | NonNullable<T> | 排除 null/undefined |
| 返回类型 | ReturnType<T> | 函数返回类型 |
| 参数类型 | Parameters<T> | 函数参数类型 |
| 实例类型 | InstanceType<T> | 构造函数实例类型 |

### 常用 API

| 中文 | 英文 | 说明 |
|------|------|------|
| 解析 JSON | JSON.parse | 解析 JSON 字符串 |
| 序列化 JSON | JSON.stringify | 转换为 JSON 字符串 |
| 映射数组 | map | 数组元素映射 |
| 过滤数组 | filter | 数组元素过滤 |
| 归约数组 | reduce | 数组元素聚合 |
| 查找元素 | find | 查找第一个匹配元素 |
| 查找索引 | findIndex | 查找第一个匹配索引 |
| 包含检测 | includes | 检测是否包含 |
| 对象键 | Object.keys | 获取对象所有键 |
| 对象值 | Object.values | 获取对象所有值 |
| 对象条目 | Object.entries | 获取键值对数组 |
| 展开运算符 | spread operator | `...` 展开运算 |
| 剩余运算符 | rest operator | `...` 剩余参数 |

### 运行环境

| 中文 | 英文 | 说明 |
|------|------|------|
| Node.js | Node.js | JavaScript 运行时 |
| npm | npm | Node 包管理器 |
| yarn | yarn | 替代包管理器 |
| pnpm | pnpm | 高效包管理器 |
| 打包工具 | bundler | Webpack/Vite 等 |
| 转译器 | transpiler | TypeScript/Babel |

## 代码示例规范

### 导入规范

```typescript
// 命名导入
import { useState, useEffect } from 'react';

// 默认导入
import React from 'react';

// 类型导入
import type { User } from './types';

// 混合导入
import { User, type UserProps } from './user';
```

### 命名规范

```typescript
// 类/接口/类型：PascalCase
class UserService {}
interface UserRepository {}
type UserProps = {};

// 函数/变量：camelCase
function processData() {}
let userCount = 0;

// 常量：全大写+下划线
const MAX_SIZE = 100;

// 私有属性：_ 前缀
class User {
    private _internal: {};
}
```

### 类型定义

```typescript
// 接口
interface User {
    id: number;
    name: string;
    role?: 'admin' | 'user';  // 可选 + 字面量类型
}

// 类型别名
type Status = 'pending' | 'active' | 'inactive';
type ID = number | string;

// 泛型
type Container<T> = {
    value: T;
    metadata: Record<string, unknown>;
};

// Utility Types
type PartialUser = Partial<User>;
type UserKeys = keyof User;
type UserPreview = Pick<User, 'id' | 'name'>;
```

### 函数定义

```typescript
// 基本函数
function greet(name: string): string {
    return `Hello, ${name}`;
}

// 箭头函数
const greet = (name: string): string => `Hello, ${name}`;

// 泛型函数
function identity<T>(value: T): T {
    return value;
}

// 函数重载
function format(value: string): string;
function format(value: number): string;
function format(value: string | number): string {
    return String(value);
}
```

## 版本标注

### ES6+ (ES2015+)

```javascript
// ES6
const [a, b] = arr;              // 解构
const obj = { a, b };            // 属性简写
const fn = (x) => x * 2;         // 箭头函数
import { x } from './module';    // 模块

// ES2020
const val = obj?.prop?.nested;   // 可选链
const result = val ?? 'default'; // nullish coalescing

// ES2022
class MyClass {
    #privateField = 0;           // 私有字段
}
arr.at(-1);                      // at() 方法
```

### TypeScript 版本

```typescript
// TS 3.7+
const val = obj?.prop;

// TS 4.0+
type Arr = [...string[], number];  // 变参元组

// TS 4.5+
import { type User } from './user'; // 类型导入

// TS 5.0+
function createArray<const T>(items: T[]) {
    return items;
}
```

## 异步编程

### Promise

```typescript
// 创建 Promise
const prom = new Promise<string>((resolve, reject) => {
    resolve('done');
});

// 链式调用
fetchData()
    .then(data => processData(data))
    .catch(error => console.error(error))
    .finally(() => cleanup());

// 静态方法
Promise.all([p1, p2, p3]);       // 全部完成
Promise.allSettled([p1, p2]);    // 全部结束
Promise.race([p1, p2]);          // 第一个完成
Promise.any([p1, p2]);           // 第一个成功
```

### async/await

```typescript
async function fetchAndProcess(): Promise<string> {
    const data = await fetchData();
    return processData(data);
}

// 错误处理
async function safeFetch(): Promise<User | null> {
    try {
        return await fetchUser();
    } catch (error) {
        console.error('Fetch failed:', error);
        return null;
    }
}

// 并行执行
async function fetchAll(): Promise<[User, Order[]]> {
    const [user, orders] = await Promise.all([
        fetchUser(),
        fetchOrders()
    ]);
    return [user, orders];
}
```

## 解构与展开

```typescript
// 对象解构
const { name, age } = user;
const { name: userName } = user;  // 重命名
const { role = 'guest' } = user;  // 默认值
const { id, ...rest } = user;     // rest

// 数组解构
const [first, second] = arr;
const [first, ...rest] = arr;

// Spread
const merged = { ...obj1, ...obj2 };
const merged = [...arr1, ...arr2];
```

## 类型安全

### 类型守卫

```typescript
// typeof
function process(value: string | number): string {
    if (typeof value === 'string') {
        return value.toUpperCase();
    }
    return String(value);
}

// in operator
function process(obj: User | Admin): string {
    if ('permissions' in obj) {
        return obj.permissions.join(',');  // Admin
    }
    return obj.name;                       // User
}

// 自定义类型守卫
function isUser(value: unknown): value is User {
    return typeof value === 'object' && value !== null && 'id' in value;
}
```

### 类型断言

```typescript
// as 断言
const user = data as User;

// const 断言
const config = { url: 'https://api.example.com' } as const;

// 非空断言
const element = document.getElementById('app')!;
```

## 错误处理

### 同步错误处理

```typescript
// ✅ 正确：具体错误类型
try {
    const data = JSON.parse(input);
} catch (error) {
    if (error instanceof SyntaxError) {
        console.error('JSON 解析错误:', error.message);
    } else {
        console.error('未知错误:', error);
    }
}

// ✅ 正确：自定义错误类型
class ValidationError extends Error {
    constructor(public field: string, message: string) {
        super(message);
        this.name = 'ValidationError';
    }
}

throw new ValidationError('email', 'Invalid email format');
```

### 异步错误处理

```typescript
// ✅ 正确：async/await + try/catch
async function fetchData(): Promise<Data> {
    try {
        const response = await fetch('/api/data');
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }
        return response.json();
    } catch (error) {
        console.error('请求失败:', error);
        throw error;
    }
}

// ✅ 正确：Promise.catch
fetchData()
    .then(data => processData(data))
    .catch(error => {
        console.error('处理失败:', error);
        return defaultValue;
    });

// ❌ 错误：未处理 Promise 错误
fetchData().then(data => console.log(data));  // 缺少 .catch
```

### Result 模式（函数式错误处理）

```typescript
// 使用 Result 类型替代异常
type Result<T, E = Error> =
    | { success: true; value: T }
    | { success: false; error: E };

function divide(a: number, b: number): Result<number> {
    if (b === 0) {
        return { success: false, error: new Error('Division by zero') };
    }
    return { success: true, value: a / b };
}

const result = divide(10, 2);
if (result.success) {
    console.log(result.value);  // 5
} else {
    console.error(result.error);
}
```

## 注意事项格式

```typescript
// ❌ 错误示例
// 说明错误原因

// ✅ 正确示例
// 说明正确做法
```

## 性能注意事项

| 场景 | 建议 |
|------|------|
| 大数组操作 | 使用 TypedArray |
| React 渲染 | useMemo/useCallback |
| 异步操作 | 并行而非串行 |
| 打包优化 | Tree shaking、代码分割 |

## 工具与构建

```bash
# TypeScript
tsc --watch

# npm/yarn
npm run build
npm test

# ESLint/Prettier
eslint src/
prettier --write src/
```