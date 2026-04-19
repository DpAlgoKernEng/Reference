# C++日期和时间库（Date and Time Library）

来源：cppreference.com

## 日期时间库概述

C++包含两种类型的时间操作支持：

1. **chrono库** - 灵活的类型集合，以不同程度的精度跟踪时间（如`std::chrono::time_point`）
2. **C风格日期时间库** - 传统C风格的时间操作函数（如`std::time`）

## 主要组件

### 一、时间点（Time Point）

时间点表示从特定时钟的纪元开始经过的时间段。

| 组件 | 说明 |
|------|------|
| `time_point`(C++11) | 时间点（类模板） |
| `clock_time_conversion`(C++20) | 定义如何将一个时钟的时间点转换为另一个时钟的特征类（类模板） |
| `clock_cast`(C++20) | 将一个时钟的时间点转换为另一个时钟（函数模板） |

### 二、持续时间（Duration）

持续时间由时间跨度组成，定义为某时间单位的若干个滴答。例如，"42秒"可表示为由42个1秒时间单位组成的持续时间。

| 组件 | 说明 |
|------|------|
| `duration`(C++11) | 时间间隔（类模板） |

### 三、时钟（Clocks）

时钟由起始点（或纪元）和滴答率组成。例如，时钟可能以1970年1月1日为纪元，每秒滴答一次。

| 组件 | 说明 |
|------|------|
| `system_clock`(C++11) | 系统范围实时时钟的挂钟时间（类） |
| `steady_clock`(C++11) | 永远不会被调整的单调时钟（类） |
| `high_resolution_clock`(C++11) | 具有最短滴答周期的时钟（类） |
| `is_clock`/`is_clock_v`(C++20) | 确定类型是否为时钟（类模板）/（变量模板） |
| `utc_clock`(C++20) | 协调世界时(UTC)时钟（类） |
| `tai_clock`(C++20) | 国际原子时(TAI)时钟（类） |
| `gps_clock`(C++20) | GPS时间时钟（类） |
| `file_clock`(C++20) | 用于文件时间的时钟（typedef） |
| `local_t`(C++20) | 表示本地时间的伪时钟（类） |

### 四、一天中的时间（C++20起）

| 组件 | 说明 |
|------|------|
| `hh_mm_ss`(C++20) | 表示一天中的时间（类模板） |
| `is_am`/`is_pm`/`make12`/`make24`(C++20) | 在12小时制/24小时制时间之间转换（函数） |

### 五、日历（C++20起）

| 组件 | 说明 |
|------|------|
| `last_spec`(C++20) | 标记类，表示月份的最后一天或工作日（类） |
| `day`(C++20) | 表示月份中的一天（类） |
| `month`(C++20) | 表示年份中的月份（类） |
| `year`(C++20) | 表示公历中的年份（类） |
| `weekday`(C++20) | 表示公历中的星期几（类） |
| `operator/`(C++20) | 公历日期创建的常规语法（函数） |

### 六、时区（C++20起）

| 组件 | 说明 |
|------|------|
| `tzdb`(C++20) | 描述IANA时区数据库副本（类） |
| `time_zone`(C++20) | 表示时区（类） |
| `zoned_time`(C++20) | 表示时区和时间点（类） |
| `locate_zone`(C++20) | 根据名称定位时区（函数） |

### 七、字面量（C++14起）

| 组件 | 说明 |
|------|------|
| `operator""y`(C++20) | 表示特定年份的`std::chrono::year`字面量（函数） |
| `operator""h`(C++14) | 表示小时的`std::chrono::duration`字面量（函数） |
| `operator""min`(C++14) | 表示分钟的`std::chrono::duration`字面量（函数） |
| `operator""s`(C++14) | 表示秒的`std::chrono::duration`字面量（函数） |
| `operator""ms`(C++14) | 表示毫秒的`std::chrono::duration`字面量（函数） |
| `operator""us`(C++14) | 表示微秒的`std::chrono::duration`字面量（函数） |
| `operator""ns`(C++14) | 表示纳秒的`std::chrono::duration`字面量（函数） |

---

## 扩展知识详解

### 一、现代chrono库演进

#### 1. 基础持续时间使用（C++11）
```cpp
#include <chrono>
#include <iostream>

void basic_duration_usage() {
    // 基本时间单位
    using namespace std::chrono;
    
    // 创建不同单位的持续时间
    auto seconds = 5s;           // 5秒
    auto milliseconds = 100ms;   // 100毫秒
    auto microseconds = 500us;   // 500微秒
    auto nanoseconds = 1000ns;   // 1000纳秒
    auto minutes = 2min;         // 2分钟
    auto hours = 1h;             // 1小时
    
    // 持续时间转换
    auto total_ms = duration_cast<milliseconds>(seconds + milliseconds);
    std::cout << "Total milliseconds: " << total_ms.count() << "ms\n";
    
    // 持续时间运算
    auto result = 2h + 30min + 45s;
    auto seconds_only = duration_cast<seconds>(result);
    std::cout << "Total seconds: " << seconds_only.count() << "s\n";
}
```

#### 2. 时间点操作（C++11）
```cpp
#include <chrono>
#include <iostream>

void time_point_operations() {
    using namespace std::chrono;
    
    // 获取当前时间点
    auto now = system_clock::now();
    auto steady_now = steady_clock::now();
    
    // 时间点运算
    auto later = now + 5min;
    auto earlier = now - 30s;
    
    // 计算时间差
    auto diff = later - now;
    std::cout << "Time difference: " << duration_cast<seconds>(diff).count() << " seconds\n";
    
    // 转换到time_t（C风格）
    std::time_t time_c = system_clock::to_time_t(now);
    std::cout << "Current time: " << std::ctime(&time_c);
}
```

#### 3. 高精度计时（C++11）
```cpp
#include <chrono>
#include <iostream>
#include <cmath>

void high_precision_timing() {
    auto start = std::chrono::high_resolution_clock::now();
    
    // 执行一些计算
    volatile double result = 0.0;
    for (int i = 0; i < 1000000; ++i) {
        result += std::sin(i * 0.001);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    
    // 计算执行时间
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " microseconds\n";
}
```

### 二、现代日历和时区（C++20）

#### 1. 日历日期操作
```cpp
#include <chrono>
#include <iostream>

void calendar_operations() {
    using namespace std::chrono;
    
    // 创建日期
    year_month_day ymd{2025y, January, 15d};
    std::cout << "Date: " << static_cast<int>(ymd.year()) << "-"
              << static_cast<unsigned>(ymd.month()) << "-"
              << static_cast<unsigned>(ymd.day()) << "\n";
    
    // 日期运算
    auto next_month = ymd + months{1};
    std::cout << "Next month: " << static_cast<int>(next_month.year()) << "-"
              << static_cast<unsigned>(next_month.month()) << "-"
              << static_cast<unsigned>(next_month.day()) << "\n";
    
    // 星期几计算
    weekday wd{ymd};
    std::cout << "Weekday: " << wd.c_encoding() << "\n";
    
    // 最后一天
    year_month_day_last ymdl{2025y / February / last};
    std::cout << "Last day of Feb 2025: " 
              << static_cast<unsigned>(ymdl.day()) << "\n";
}
```

#### 2. 一天中的时间
```cpp
#include <chrono>
#include <iostream>

void time_of_day_operations() {
    using namespace std::chrono;
    
    // 创建持续时间
    auto duration_since_midnight = 14h + 30min + 45s + 123ms;
    
    // 转换为hh_mm_ss格式
    hh_mm_ss time{duration_since_midnight};
    
    std::cout << "Hours: " << time.hours().count() << "\n";
    std::cout << "Minutes: " << time.minutes().count() << "\n";
    std::cout << "Seconds: " << time.seconds().count() << "\n";
    std::cout << "Subseconds: " << time.subseconds().count() << "\n";
    
    // 12小时制转换
    auto pm_time = make12(duration_since_midnight);
    std::cout << "12-hour format: " << (is_pm(duration_since_midnight) ? "PM" : "AM") << "\n";
}
```

#### 3. 时区处理
```cpp
#include <chrono>
#include <iostream>

void timezone_operations() {
    using namespace std::chrono;
    
    try {
        // 获取时区
        auto utc_zone = locate_zone("UTC");
        auto local_zone = current_zone();
        
        // 创建带时区的时间
        auto sys_time = system_clock::now();
        zoned_time utc_time{utc_zone, sys_time};
        zoned_time local_time{local_zone, sys_time};
        
        std::cout << "UTC time: " << utc_time << "\n";
        std::cout << "Local time: " << local_time << "\n";
        
        // 时区转换
        zoned_time converted_time{"America/New_York", sys_time};
        std::cout << "New York time: " << converted_time << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "Timezone error: " << e.what() << "\n";
    }
}
```

### 三、性能计时和基准测试

#### 1. 通用计时器类
```cpp
#include <chrono>
#include <iostream>
#include <string>
#include <functional>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::string name_;
    
public:
    explicit Timer(const std::string& name = "Timer") : name_(name) {
        start_time_ = std::chrono::high_resolution_clock::now();
    }
    
    ~Timer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time_);
        std::cout << name_ << " took " << duration.count() << " microseconds\n";
    }
    
    template<typename Func, typename... Args>
    static auto benchmark(Func&& func, Args&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Function took " << duration.count() << " microseconds\n";
        
        return result;
    }
};

// 使用示例
void timing_example() {
    {
        Timer timer("Vector creation");
        std::vector<int> vec(1000000, 42);
    }
    
    auto result = Timer::benchmark([](int n) {
        return n * n;
    }, 42);
    
    std::cout << "Result: " << result << "\n";
}
```

#### 2. 性能比较框架
```cpp
#include <chrono>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

template<typename TimeUnit = std::chrono::milliseconds>
class PerformanceTester {
private:
    std::vector<std::chrono::nanoseconds> measurements_;
    
public:
    template<typename Func>
    void measure(const std::string& name, Func&& func, int iterations = 1) {
        std::vector<std::chrono::nanoseconds> times;
        times.reserve(iterations);
        
        for (int i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            std::invoke(func);
            auto end = std::chrono::high_resolution_clock::now();
            times.push_back(end - start);
        }
        
        // 计算统计信息
        auto min_time = *std::min_element(times.begin(), times.end());
        auto max_time = *std::max_element(times.begin(), times.end());
        auto avg_time = std::accumulate(times.begin(), times.end(), 
                                       std::chrono::nanoseconds{0}) / times.size();
        
        std::cout << "Performance test: " << name << "\n";
        std::cout << "  Min: " << std::chrono::duration_cast<TimeUnit>(min_time).count() 
                  << " " << TimeUnit::period::num << "/" << TimeUnit::period::den << "\n";
        std::cout << "  Max: " << std::chrono::duration_cast<TimeUnit>(max_time).count() 
                  << " " << TimeUnit::period::num << "/" << TimeUnit::period::den << "\n";
        std::cout << "  Avg: " << std::chrono::duration_cast<TimeUnit>(avg_time).count() 
                  << " " << TimeUnit::period::num << "/" << TimeUnit::period::den << "\n\n";
    }
};

// 性能比较示例
void performance_comparison() {
    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1);
    
    PerformanceTester<std::chrono::milliseconds> tester;
    
    tester.measure("std::sort", [&data]() {
        auto copy = data;
        std::sort(copy.begin(), copy.end());
    });
    
    tester.measure("std::stable_sort", [&data]() {
        auto copy = data;
        std::stable_sort(copy.begin(), copy.end());
    }, 5); // 运行5次取平均
}
```

### 四、实际应用示例

#### 1. 超时和定时器
```cpp
#include <chrono>
#include <thread>
#include <future>
#include <iostream>

class TimeoutManager {
public:
    template<typename Func, typename Rep, typename Period>
    static bool execute_with_timeout(Func&& func, 
                                   const std::chrono::duration<Rep, Period>& timeout) {
        auto future = std::async(std::launch::async, std::forward<Func>(func));
        return future.wait_for(timeout) == std::future_status::ready;
    }
    
    static void sleep_for(const std::chrono::milliseconds& duration) {
        std::this_thread::sleep_for(duration);
    }
    
    static void sleep_until(const std::chrono::system_clock::time_point& time_point) {
        std::this_thread::sleep_until(time_point);
    }
};

// 使用示例
void timeout_example() {
    using namespace std::chrono_literals;
    
    // 带超时的执行
    bool completed = TimeoutManager::execute_with_timeout([]() {
        std::this_thread::sleep_for(2s);
        return 42;
    }, 1s);
    
    std::cout << "Task completed: " << (completed ? "Yes" : "No (timeout)") << "\n";
    
    // 定时睡眠
    auto wakeup_time = std::chrono::system_clock::now() + 500ms;
    TimeoutManager::sleep_until(wakeup_time);
    std::cout << "Woke up!\n";
}
```

#### 2. 日期时间格式化和解析
```cpp
#include <chrono>
#include <iostream>
#include <sstream>

class DateTimeFormatter {
public:
    // 格式化系统时间
    static std::string format_system_time(const std::chrono::system_clock::time_point& tp) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    // 格式化持续时间
    template<typename Rep, typename Period>
    static std::string format_duration(const std::chrono::duration<Rep, Period>& duration) {
        using namespace std::chrono;
        
        auto hours = duration_cast<std::chrono::hours>(duration);
        auto minutes = duration_cast<std::chrono::minutes>(duration - hours);
        auto seconds = duration_cast<std::chrono::seconds>(duration - hours - minutes);
        auto milliseconds = duration_cast<std::chrono::milliseconds>(
            duration - hours - minutes - seconds);
        
        std::stringstream ss;
        ss << hours.count() << "h " << minutes.count() << "m " 
           << seconds.count() << "s " << milliseconds.count() << "ms";
        return ss.str();
    }
    
    // 创建特定时间点
    static std::chrono::system_clock::time_point create_time_point(
        int year, unsigned month, unsigned day, 
        unsigned hour = 0, unsigned minute = 0, unsigned second = 0) {
        
        using namespace std::chrono;
        
        // 使用C++20日历（如果可用）
        #if __cplusplus >= 202002L
        year_month_day ymd{year, month, day};
        auto sys_days = std::chrono::sys_days{ymd};
        return sys_days + hours{hour} + minutes{minute} + seconds{second};
        #else
        // 回退到传统方法
        std::tm tm{};
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;
        return system_clock::from_time_t(std::mktime(&tm));
        #endif
    }
};

// 使用示例
void formatting_example() {
    using namespace std::chrono;
    using namespace std::chrono_literals;
    
    auto now = system_clock::now();
    std::cout << "Current time: " << DateTimeFormatter::format_system_time(now) << "\n";
    
    auto duration = 2h + 30min + 45s + 123ms;
    std::cout << "Duration: " << DateTimeFormatter::format_duration(duration) << "\n";
    
    auto specific_time = DateTimeFormatter::create_time_point(2025, 12, 25, 10, 30, 0);
    std::cout << "Christmas 2025: " << DateTimeFormatter::format_system_time(specific_time) << "\n";
}
```

#### 3. 定时任务调度器
```cpp
#include <chrono>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class TaskScheduler {
private:
    struct TimedTask {
        std::chrono::steady_clock::time_point execute_time;
        std::function<void()> task;
        int id;
        
        bool operator>(const TimedTask& other) const {
            return execute_time > other.execute_time;
        }
    };
    
    std::priority_queue<TimedTask, std::vector<TimedTask>, std::greater<TimedTask>> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> running_{false};
    std::thread worker_thread_;
    int next_id_ = 0;
    
public:
    TaskScheduler() = default;
    
    ~TaskScheduler() {
        stop();
    }
    
    void start() {
        running_ = true;
        worker_thread_ = std::thread(&TaskScheduler::worker_loop, this);
    }
    
    void stop() {
        running_ = false;
        condition_.notify_all();
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
    }
    
    template<typename Rep, typename Period>
    int schedule_task(std::function<void()> task, 
                     const std::chrono::duration<Rep, Period>& delay) {
        auto execute_time = std::chrono::steady_clock::now() + delay;
        
        std::lock_guard<std::mutex> lock(queue_mutex_);
        int id = next_id_++;
        task_queue_.push({execute_time, std::move(task), id});
        condition_.notify_one();
        return id;
    }
    
    void schedule_at(std::function<void()> task, 
                    const std::chrono::steady_clock::time_point& time_point) {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        int id = next_id_++;
        task_queue_.push({time_point, std::move(task), id});
        condition_.notify_one();
    }
    
private:
    void worker_loop() {
        while (running_) {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            
            if (task_queue_.empty()) {
                condition_.wait(lock, [this] { return !task_queue_.empty() || !running_; });
                continue;
            }
            
            auto now = std::chrono::steady_clock::now();
            auto& next_task = task_queue_.top();
            
            if (next_task.execute_time <= now) {
                auto task = next_task.task;
                task_queue_.pop();
                lock.unlock();
                task();
            } else {
                condition_.wait_until(lock, next_task.execute_time);
            }
        }
    }
};

// 使用示例
void scheduler_example() {
    TaskScheduler scheduler;
    scheduler.start();
    
    using namespace std::chrono_literals;
    
    // 调度任务
    scheduler.schedule_task([]() {
        std::cout << "Task 1 executed at " 
                  << std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now().time_since_epoch()).count() 
                  << "s\n";
    }, 2s);
    
    scheduler.schedule_task([]() {
        std::cout << "Task 2 executed\n";
    }, 1s);
    
    // 等待任务执行
    std::this_thread::sleep_for(3s);
    scheduler.stop();
}
```

## 示例代码详解

### 原始示例分析
```cpp
#include <chrono>
#include <iostream>

long Fibonacci(unsigned n) {
    return n < 2 ? n : Fibonacci(n - 1) + Fibonacci(n - 2);
}

int main() {
    const auto start{std::chrono::steady_clock::now()};
    const auto fb{Fibonacci(42)};
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};

    std::cout << "Fibonacci(42): " << fb << "\nElapsed time: ";
    std::cout << elapsed_seconds << '\n'; // C++20's chrono::duration operator<<
}
```

### 输出结果
```
Fibonacci(42): 267914296
Elapsed time: 0.791429s
```

### 代码要点解析

1. **选择合适的时钟**：使用`steady_clock`进行性能测量，因为它不会被系统时间调整影响
2. **高精度持续时间**：使用`duration<double>`获得浮点数秒数表示
3. **C++20特性**：直接使用`operator<<`输出持续时间
4. **类型推导**：使用`auto`简化类型声明

## 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|------------|----|------|------|
| `__cpp_lib_chrono` | `201510L` | (C++17) | `std::chrono::duration`和`std::chrono::time_point`的舍入函数 |
| `201611L` | (C++17) | `std::chrono::duration`和`std::chrono::time_point`所有成员函数的constexpr |
| `201907L` | (C++20) | 日历和时区 |
| `202306L` | (C++26) | `std::chrono`值类的哈希支持 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/chrono&oldid=179908>
- 最后修改时间：2025年1月28日 11:46
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++日期时间库为现代C++程序提供了强大而灵活的时间处理能力。从基础的`duration`和`time_point`到现代的`calendar`和`time_zone`，chrono库不断演进以满足日益复杂的时区和日期处理需求。正确理解各种时钟的特性，选择合适的时钟进行性能测量和时间处理至关重要。C++20引入的日历和时区功能使得国际化的日期时间处理变得更加简单和安全。掌握chrono库的各种特性和最佳实践，能够帮助开发者编写更加健壮和高效的C++应用程序。持续学习和实践现代chrono库的新特性，是跟上C++发展步伐的重要途径。