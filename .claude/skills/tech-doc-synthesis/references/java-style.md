# Java 文档风格指南

## 术语规范

### 基础概念

| 中文 | 英文 | 说明 |
|------|------|------|
| 类 | class | 类型定义 |
| 接口 | interface | 抽象类型定义 |
| 抽象类 | abstract class | 不能实例化的类 |
| 枚举 | enum | 枚举类型 |
| 注解 | annotation | 元数据标记 |
| 记录 | record | Java 16+ 简洁数据载体 |
| 密封类 | sealed class | Java 17+ 限制继承 |

### 类型系统

| 中文 | 英文 | 说明 |
|------|------|------|
| 泛型 | generics | 类型参数化 |
| 类型参数 | type parameter | 泛型参数 `<T>` |
| 通配符 | wildcard | `?` 未知类型 |
| 上界 | upper bound | `? extends T` |
| 下界 | lower bound | `? super T` |
| 类型擦除 | type erasure | 泛型运行时擦除 |
| 原始类型 | raw type | 无泛型参数类型 |
| 协变 | covariance | `? extends T` |
| 逆变 | contravariance | `? super T` |

### 面向对象

| 中文 | 英文 | 说明 |
|------|------|------|
| 继承 | inheritance | 类继承关系 |
| 实现 | implements | 接口实现 |
| 多态 | polymorphism | 同一接口不同实现 |
| 重写 | override | 子类重写父类方法 |
| 重载 | overload | 同名不同参数方法 |
| 封装 | encapsulation | 访问控制 |
| 内部类 | inner class | 嵌套类 |
| 静态内部类 | static nested class | 静态嵌套类 |
| 匿名类 | anonymous class | 匿名内部类 |

### 访问修饰符

| 中文 | 英文 | 说明 |
|------|------|------|
| 公有 | public | 所有类可访问 |
| 保护 | protected | 同包和子类可访问 |
| 默认 | package-private | 同包可访问 |
| 私有 | private | 仅本类可访问 |
| 最终 | final | 不可继承/重写/修改 |
| 静态 | static | 类级别成员 |

### 异常处理

| 中文 | 英文 | 说明 |
|------|------|------|
| 异常 | exception | 错误情况表示 |
| 受检异常 | checked exception | 必须处理的异常 |
| 非受检异常 | unchecked exception | RuntimeException |
| 错误 | Error | JVM 严重错误 |
| 抛出 | throw | 抛出异常 |
| 捕获 | catch | 捕获异常 |
| 声明抛出 | throws | 方法声明异常 |
| try-with-resources | try-with-resources | 自动资源管理 |

### 集合框架

| 中文 | 英文 | 说明 |
|------|------|------|
| 列表 | List | 有序集合 |
| 集合 | Set | 无序唯一元素集合 |
| 映射 | Map | 键值对集合 |
| 队列 | Queue | 先进先出队列 |
| 双端队列 | Deque | 双端队列 |
| 栈 | Stack | 后进先出栈 |
| 迭代器 | Iterator | 遍历集合 |
| 比较器 | Comparator | 比较规则 |

### 流式 API

| 中文 | 英文 | 说明 |
|------|------|------|
| 流 | stream | Stream API 数据流 |
| 中间操作 | intermediate operation | 返回新流的操作 |
| 终端操作 | terminal operation | 产生结果的操作 |
| 映射 | map | 元素转换 |
| 过滤 | filter | 元素过滤 |
| 归约 | reduce | 元素聚合 |
| 收集 | collect | 收集为集合 |
| 并行流 | parallel stream | 并行处理的流 |

### 并发编程

| 中文 | 英文 | 说明 |
|------|------|------|
| 线程 | Thread | 执行线程 |
| 线程池 | ExecutorService | 线程池服务 |
| 可完成 Future | CompletableFuture | 异步计算 |
| 锁 | Lock | 显式锁 |
| 读写锁 | ReadWriteLock | 读写分离锁 |
| 信号量 | Semaphore | 并发数控制 |
| 倒计时锁 | CountDownLatch | 等待多个线程 |
| 屏障 | CyclicBarrier | 线程集合点 |
| 原子类 | AtomicInteger | 原子操作类 |
| 可变对象 | volatile | 可见性保证 |

### JVM 相关

| 中文 | 英文 | 说明 |
|------|------|------|
| JVM | JVM | Java 虚拟机 |
| 堆 | heap | 对象存储区域 |
| 栈 | stack | 方法调用栈 |
| 方法区 | method area | 类信息存储 |
| 垃圾回收 | garbage collection | GC 自动内存回收 |
| 类加载器 | ClassLoader | 加载类文件 |
| 字节码 | bytecode | JVM 指令 |
| JVM 参数 | JVM options | `-Xmx`、`-Xms` 等 |

### 常用标准库

| 中文 | 英文 | 说明 |
|------|------|------|
| 字符串 | String | 不可变字符串 |
| 字符串构建器 | StringBuilder | 可变字符串 |
| 数组列表 | ArrayList | 动态数组 |
| 哈希映射 | HashMap | 哈希表实现 |
| 数组工具 | Arrays | 数组操作工具 |
| 集合工具 | Collections | 集合操作工具 |
| 对象 | Objects | 对象操作工具 |
| 可选 | Optional | 可能存在或不存在 |
| 文件 | Files | 文件操作 |
| 路径 | Path | 文件系统路径 |
| 时间 | LocalDateTime | 本地日期时间 |
| 日期 | LocalDate | 本地日期 |

## 代码示例规范

### 包声明与导入

```java
package com.example.myproject.service;

import java.util.List;           // 标准库
import java.util.ArrayList;

import org.slf4j.Logger;         // 第三方库

import com.example.model.User;   // 项目类
```

### 命名规范

```java
// 类/接口：PascalCase
public class UserManager {}
public interface UserRepository {}

// 方法/变量：camelCase
public void processData() {}
private int userCount = 0;

// 常量：全大写+下划线
private static final int MAX_SIZE = 100;

// 包名：小写
package com.example.myproject;
```

### 类定义规范

```java
public class UserService {

    // 常量
    private static final Logger logger = LoggerFactory.getLogger(UserService.class);

    // 字段
    private final UserRepository repository;

    // 构造函数
    public UserService(UserRepository repository) {
        this.repository = repository;
    }

    // 方法
    public User findById(Long id) {
        return repository.findById(id);
    }
}
```

### 泛型规范

```java
// 泛型类
public class Container<T> {
    private T value;
    public T getValue() { return value; }
}

// 泛型方法
public <T> List<T> createList(T... elements) {
    return Arrays.asList(elements);
}

// 泛型约束
public <T extends Comparable<T>> T max(T a, T b) {
    return a.compareTo(b) >= 0 ? a : b;
}

// 通配符
public void process(List<?> list);
public List<? extends Number> getNumbers();
```

## 版本标注

### Java 8+

```java
// Lambda 和 Stream
List<User> active = users.stream()
    .filter(u -> u.isActive())
    .toList();

// Optional
Optional<User> user = repository.findById(id);
```

### Java 9+

```java
// 集合工厂方法
List<String> list = List.of("a", "b", "c");
Map<String, Integer> map = Map.of("a", 1, "b", 2);
```

### Java 10+

```java
// 局部变量类型推断
var users = new ArrayList<User>();
var stream = users.stream();
```

### Java 14+

```java
// switch 表达式
String result = switch (status) {
    case 200 -> "OK";
    case 404 -> "Not Found";
    default -> "Unknown";
};

// Record
public record User(Long id, String name, String email) {}
```

### Java 17+

```java
// Sealed Classes
public sealed interface Shape permits Circle, Rectangle {}
```

## 异常处理

```java
// ✅ 正确：try-with-resources
public void processData(String path) {
    try (BufferedReader reader = Files.newBufferedReader(Paths.get(path))) {
        String line;
        while ((line = reader.readLine()) != null) {
            // 处理
        }
    } catch (IOException e) {
        logger.error("Read failed", e);
    }
}

// ✅ 正确：自定义异常
public class BusinessException extends RuntimeException {
    private final String code;
    public BusinessException(String code, String message) {
        super(message);
        this.code = code;
    }
}

// ❌ 错误：空 catch 块
try { doSomething(); } catch (Exception e) { }
```

## Stream API

```java
// 常用操作
List<User> activeAdults = users.stream()
    .filter(u -> u.getAge() >= 18)
    .filter(User::isActive)
    .sorted(Comparator.comparing(User::getName))
    .map(User::getEmail)
    .distinct()
    .toList();

// 分组
Map<String, List<User>> byDept = users.stream()
    .collect(Collectors.groupingBy(User::getDepartment));

// Optional 与 Stream
Optional<User> first = users.stream()
    .filter(User::isActive)
    .findFirst();
```

## 并发编程

### CompletableFuture

```java
ExecutorService executor = Executors.newFixedThreadPool(10);

CompletableFuture<String> future = CompletableFuture.supplyAsync(() -> {
    return fetchData();
}, executor);

CompletableFuture<String> result = future
    .thenApply(this::processData)
    .exceptionally(ex -> "Fallback: " + ex.getMessage());

// 组合多个 Future
CompletableFuture<Void> all = CompletableFuture.allOf(future1, future2);
```

### 线程安全

```java
// ReentrantLock
private final ReentrantLock lock = new ReentrantLock();

public void increment() {
    lock.lock();
    try {
        count++;
    } finally {
        lock.unlock();
    }
}

// ReadWriteLock
private final ReadWriteLock rwLock = new ReentrantReadWriteLock();

public String read() {
    rwLock.readLock().lock();
    try { return data; }
    finally { rwLock.readLock().unlock(); }
}
```

## 注意事项格式

```java
// ❌ 错误示例
// 说明错误原因

// ✅ 正确示例
// 说明正确做法
```

## 性能注意事项

| 场景 | 建议 |
|------|------|
| 大字符串拼接 | 使用 StringBuilder |
| 频繁对象创建 | 减少创建或使用对象池 |
| 并发场景 | 合适的线程池大小 |
| JVM 调优 | 根据场景选择 GC |

## 工具与构建

```bash
# Maven
mvn clean install
mvn test

# Gradle
gradle build
gradle test

# 代码质量
mvn spotbugs:check
mvn checkstyle:check
```