# CompletableFuture - Java 异步编程

## 1. 概述

`CompletableFuture` 是 Java 8 引入的异步编程工具，实现了 `Future` 和 `CompletionStage` 接口。它支持链式调用、异常处理和组合多个异步任务，是 Java 响应式编程的基础。

定义在 `java.util.concurrent` 包中，无需额外依赖。

与 `Future` 的关键区别：
- `Future` 只能阻塞等待结果
- `CompletableFuture` 支持回调、链式调用和组合操作

## 2. 来源与演变

### Java 8 引入

`CompletableFuture` 在 **Java 8**（2014 年）中首次引入，填补了 Java 异步编程的空白。

### 设计动机

- `Future` 的 `get()` 方法阻塞，无法实现真正的异步回调
- 需要链式调用和组合多个异步任务
- 响应式编程模式的需求

### 主要版本变更

| 版本 | 变更 |
|------|------|
| Java 8 | 首次引入 |
| Java 9 | 新增 `orTimeout()`、`completeOnTimeout()` |
| Java 12 | 新增 `exceptionallyAsync()` |

## 3. 语法与参数

### 创建方式

```java
import java.util.concurrent.CompletableFuture;

// 已知结果
CompletableFuture<String> completed = CompletableFuture.completedFuture("result");

// 异步执行（使用 ForkJoinPool）
CompletableFuture<Void> runAsync = CompletableFuture.runAsync(() -> {
    // 无返回值的异步任务
});

CompletableFuture<String> supplyAsync = CompletableFuture.supplyAsync(() -> {
    return "result";
});

// 指定执行器
CompletableFuture<String> withExecutor = CompletableFuture.supplyAsync(
    () -> "result",
    executor
);
```

### 链式调用方法

| 方法 | 说明 |
|------|------|
| `thenApply(fn)` | 同步转换结果 |
| `thenApplyAsync(fn)` | 异步转换结果 |
| `thenAccept(consumer)` | 消费结果（无返回） |
| `thenRun(runnable)` | 执行动作（无输入无输出） |
| `thenCompose(fn)` | 扁平化（返回 CompletableFuture） |
| `thenCombine(other, fn)` | 组合两个 Future |
| `whenComplete(fn)` | 完成时回调 |
| `handle(fn)` | 处理结果或异常 |
| `exceptionally(fn)` | 异常处理 |

### 组合方法

| 方法 | 说明 |
|------|------|
| `allOf(futures...)` | 所有完成 |
| `anyOf(futures...)` | 任一完成 |

## 4. 底层原理

### 状态管理

```
CompletableFuture 状态转换：

NEW ──────> COMPLETED
  │              │
  │              └── 结果可用
  │
  └──────────> EXCEPTIONALLY
                    │
                    └── 异常可用
```

### 链式调用机制

```
supplyAsync()
    │
    ▼
thenApply(fn)
    │
    ▼
thenAccept(consumer)

每个 stage 保存依赖链，当前 stage 完成后触发下一个 stage
```

### 线程池

- 默认使用 `ForkJoinPool.commonPool()`
- 生产环境建议指定自定义 `Executor`
- 异步方法 `*Async` 在新线程执行，非异步方法在调用线程执行

## 5. 使用场景

### 适合使用

- 并行调用多个独立服务
- 异步 I/O 操作
- 构建响应式流水线
- 非阻塞等待结果

### 不适合使用

- 简单同步任务 → 直接调用
- 需要严格顺序 → 同步代码
- 高吞吐消息处理 → 考虑响应式框架（Reactor/RxJava）

### 最佳实践

```java
// ✅ 正确：指定线程池
ExecutorService executor = Executors.newFixedThreadPool(10);
CompletableFuture<String> future = CompletableFuture.supplyAsync(
    () -> fetchData(),
    executor
);

// ✅ 正确：处理异常
future
    .thenApply(this::process)
    .exceptionally(ex -> "fallback");

// ✅ 正确：超时处理
future.orTimeout(5, TimeUnit.SECONDS);

// ❌ 错误：阻塞等待
String result = future.get();  // 阻塞当前线程
```

## 6. 代码示例

### 基础用法

```java
import java.util.concurrent.CompletableFuture;

public class BasicExample {
    public static void main(String[] args) throws Exception {
        // 异步计算
        CompletableFuture<String> future = CompletableFuture.supplyAsync(() -> {
            return "Hello";
        });

        // 链式调用
        CompletableFuture<String> result = future
            .thenApply(s -> s + " World")
            .thenApply(String::toUpperCase);

        System.out.println(result.get());  // HELLO WORLD
    }
}
```

### 异常处理

```java
import java.util.concurrent.CompletableFuture;

public class ExceptionExample {
    public static void main(String[] args) {
        CompletableFuture<String> future = CompletableFuture
            .supplyAsync(() -> {
                if (Math.random() > 0.5) {
                    throw new RuntimeException("Random error");
                }
                return "success";
            })
            .handle((result, error) -> {
                if (error != null) {
                    return "Recovered from: " + error.getMessage();
                }
                return result;
            });

        future.thenAccept(System.out::println);
    }
}
```

### 组合多个 Future

```java
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class CombineExample {
    private static final ExecutorService executor = Executors.newFixedThreadPool(4);

    public static void main(String[] args) {
        // 并行调用两个服务
        CompletableFuture<String> userFuture = CompletableFuture.supplyAsync(
            () -> fetchUser(1L),
            executor
        );

        CompletableFuture<String> orderFuture = CompletableFuture.supplyAsync(
            () -> fetchOrders(1L),
            executor
        );

        // 组合结果
        CompletableFuture<String> combined = userFuture.thenCombine(
            orderFuture,
            (user, orders) -> user + " has " + orders
        );

        combined.thenAccept(System.out::println);
    }

    static String fetchUser(Long id) { return "User-" + id; }
    static String fetchOrders(Long id) { return "3 orders"; }
}
```

### 并行执行多个任务

```java
import java.util.concurrent.CompletableFuture;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class ParallelExample {
    public static void main(String[] args) {
        // 创建多个 Future
        List<CompletableFuture<String>> futures = IntStream.range(0, 5)
            .mapToObj(i -> CompletableFuture.supplyAsync(() -> "Task-" + i))
            .collect(Collectors.toList());

        // 等待所有完成
        CompletableFuture<Void> allOf = CompletableFuture.allOf(
            futures.toArray(new CompletableFuture[0])
        );

        allOf.thenRun(() -> {
            List<String> results = futures.stream()
                .map(CompletableFuture::join)
                .collect(Collectors.toList());
            System.out.println(results);
        }).join();

        // 任一完成
        CompletableFuture<Object> anyOf = CompletableFuture.anyOf(
            futures.toArray(new CompletableFuture[0])
        );
        System.out.println("First: " + anyOf.join());
    }
}
```

### 超时处理

```java
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.TimeUnit;

public class TimeoutExample {
    public static void main(String[] args) {
        CompletableFuture<String> future = CompletableFuture.supplyAsync(() -> {
            try {
                Thread.sleep(3000);  // 模拟耗时操作
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            return "result";
        });

        // Java 9+ 超时
        future
            .orTimeout(2, TimeUnit.SECONDS)
            .exceptionally(ex -> "Timeout fallback")
            .thenAccept(System.out::println);

        // 或使用 completeOnTimeout
        future
            .completeOnTimeout("default value", 2, TimeUnit.SECONDS)
            .thenAccept(System.out::println);
    }
}
```

### 实际应用：服务聚合

```java
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.List;

public class ServiceAggregator {
    private final ExecutorService executor = Executors.newFixedThreadPool(10);

    public CompletableFuture<UserDashboard> getDashboard(Long userId) {
        // 并行获取用户、订单、支付信息
        CompletableFuture<User> userFuture = CompletableFuture.supplyAsync(
            () -> userService.getUser(userId), executor
        );

        CompletableFuture<List<Order>> ordersFuture = CompletableFuture.supplyAsync(
            () -> orderService.getOrders(userId), executor
        );

        CompletableFuture<Payment> paymentFuture = CompletableFuture.supplyAsync(
            () -> paymentService.getLastPayment(userId), executor
        );

        // 组合结果
        return CompletableFuture.allOf(userFuture, ordersFuture, paymentFuture)
            .thenApply(v -> new UserDashboard(
                userFuture.join(),
                ordersFuture.join(),
                paymentFuture.join()
            ))
            .exceptionally(ex -> UserDashboard.empty());
    }
}

// 数据类
record User(Long id, String name) {}
record Order(Long id, String product) {}
record Payment(Long id, Double amount) {}
record UserDashboard(User user, List<Order> orders, Payment payment) {
    static UserDashboard empty() { return new UserDashboard(null, List.of(), null); }
}
```

## 7. 总结

`CompletableFuture` 提供强大的异步编程能力：

| 特性 | Future | CompletableFuture |
|------|--------|-------------------|
| 链式调用 | ❌ | ✅ |
| 异常处理 | ❌ | ✅ |
| 组合操作 | ❌ | ✅ |
| 回调机制 | ❌ | ✅ |
| 超时控制 | ⚠️ 有限 | ✅ 完善 |

选择建议：
- 简单异步任务 → `CompletableFuture`
- 复杂响应式流 → Reactor/RxJava
- 需要背压支持 → 响应式框架
- 阻塞 I/O → 考虑虚拟线程（Java 21+）