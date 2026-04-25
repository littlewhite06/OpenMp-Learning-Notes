# Lesson 01：OpenMP 基础总结  

## 一、学习目标 (Learning Goals)  

本节主要学习 OpenMP 在 C++ 中的基本并行编程模型，包括对并行循环的加速，#pragma omp的语法逻辑（critical，atomic, reduction）,以及不同的调度策略（static,dynamic）

---

## 二、核心内容 (Core Concepts)  

### 1. 并行循环 (Parallel For)  

使用：  

    #pragma omp parallel for  

将 for 循环的迭代分配给多个线程执行。  

前提：循环之间必须是 **independent（无依赖）**  

---

### 2. 调度策略 (Scheduling)  

OpenMP 提供不同的任务分配方式：  

- `schedule(static)`  
  - 编译时就分配好任务  
  - 适合 workload 均匀  

- `schedule(dynamic)`  
  - 运行时动态分配（负载小的程序可以接管大的）  
  - 适合 workload 不均匀  

本质区别：  
**load balancing（负载均衡）能力不同**  

---

### 3. 共享变量 vs 私有变量  

- `shared`  
  - 所有线程共享同一份变量  
  - 容易产生 race condition  

- `private`  
  - 每个线程都有独立副本  
  - 不会互相影响  
  - 但是注意入果变量定义在外部则每个线程循环内该变量的初始值不一定相同，最好在线程内重新赋值确保统一


---

### 4. 竞争条件 (Race Condition)  

多个线程同时修改同一个变量时，结果不确定。  

典型例子：  

    sum += a[i];  

---

### 5. 临界区 (Critical Section)  

    #pragma omp critical  

👉 特点：  

- 相当于加锁（lock）  
- 同一时间只能一个线程进入  
- 性能很差（严重串行化）  

---

### 6. 原子操作 (Atomic)  

    #pragma omp atomic  

👉 特点：  

- 只保护单个操作，例如普通加减乘除
- 比 critical 更轻量  
- 只能用于简单语句（如 `+=`）  

❌ 错误写法（你踩过的坑）：  

    #pragma omp atomic
    {
        if (a[i] > max_val)   // ❌
            max_val = a[i];
    }
（原因是原子操作是轻量cpu操作，不能加{}，只能有单行语句）


但也可以用于简单读写语句
     #pragma omp atomic read
     local = x;

     #pragma omp atomic write
     x = value;

---

### 7. 归约 (Reduction)  

    #pragma omp parallel for reduction(+:sum)  

👉 原理：  

- 每个线程有自己的局部变量（local copy）  
- 最后统一合并（combine）  

👉 支持：  

- `+`（sum）  
- `max`（最大值）  
- `min`（最小值）  

👉 示例：  

    #pragma omp parallel for reduction(+:sum, even_count) \
        reduction(max:max_val) \
        reduction(min:min_val)

(第一个为混合输出并且后方无空格，或者为无\空格但不换行)

#pragma omp parallel for reduction(+:sum, even_count) reduction(max:max_val) reduction(min:min_val)

---
### 8. firstprivate / lastprivate  

---

#### firstprivate  

    #pragma omp parallel for firstprivate(x)

👉 含义：  

每个线程都有一份 `x` 的私有副本，并且初始值等于原始变量的值。  

👉 示例：  

    int x = 10;

    #pragma omp parallel for firstprivate(x)
    for (int i = 0; i < 4; i++) {
        x += i;
        cout << x << endl;
    }

👉 结果说明：  

每个线程都会从 `x = 10` 开始计算，线程之间互不影响。  

---

👉 对比总结：  

| 类型 | 初值 |
|------|------|
| private | 未定义（垃圾值） |
| firstprivate | 拷贝原始值 |

---

#### lastprivate  

    #pragma omp parallel for lastprivate(x)

👉 含义：  

循环结束后，将“最后一次迭代”的变量值写回到原始变量中。  

👉 示例：  

    int x = 0;

    #pragma omp parallel for lastprivate(x)
    for (int i = 0; i < 4; i++) {
        x = i;
    }

👉 结果说明：  

最终 `x = 3`（即最后一次循环 i=3 的结果）。  

---

#### ⚠️ 注意事项（重要）  

1️⃣ lastprivate 的“最后”不是指线程执行顺序，而是：  

逻辑上的最后一次迭代（largest i）。  

---

2️⃣ firstprivate 和 lastprivate 可以同时使用：  

    #pragma omp parallel for firstprivate(x) lastprivate(x)

👉 含义：  

开始时复制变量 → 结束时写回最终结果  

---

3️⃣ 常见误区（你踩过的坑）  

❌ 认为 private 会继承初始值  

✔ 实际情况：  

- private → 不初始化（随机值）  
- firstprivate → 才会复制原始值  

---

4️⃣ 使用场景  

- firstprivate → 每个线程需要相同初始状态  
- lastprivate → 需要最终结果  

---

#### 🧠 底层理解（Engineering Insight）  

firstprivate 本质：  

每个线程创建一个局部变量（local copy），并复制初始值  

lastprivate 本质：  

在并行结束后，将最后一次迭代的结果写回共享变量  

---

## 三、重要对比 (Key Comparisons)  

| 方法 | 是否加锁 | 性能 | 使用场景 |
|------|--------|------|--------|
| critical | 是 | 慢 | 任意复杂代码 |
| atomic | 是（轻量） | 中 | 简单操作 |
| reduction | 否 | 快 | 累加、统计 |

---

## 四、你写过的程序 (Programs)  

### 01_ArrayDouble.cpp  
对数组每个元素 ×2（基础 parallel for）

---

### 02_ParallelSum_RaceReduction.cpp  
计算 sum，对比：  
- 无锁（错误）  
- critical（慢）  
- reduction（最快）  

---

### 03_Schedule_StaticDynamic.cpp  
对比 static vs dynamic 调度策略  

---

### 04_SharedPrivate_RaceDemo.cpp  
演示 shared / private 变量区别  

---

### 05_FirstPrivate_LastPrivate.cpp  
变量初始化与回写控制  

---

### 06_Reduction_vs_Critical.cpp  
验证 reduction 比 critical 快  

---

### 07_Atomic_vs_Critical.cpp  
验证 atomic > critical  

---

## 五、Project 部分 (Projects)  

### Project 1：ParallelStatsEngine  

计算：  

- sum  
- max  
- min  
- even_count  

对比三种方法：  

- critical → 最慢  
- atomic → 中等  
- reduction → 最快  

---

### Project 2：ParallelWorkloadScheduler  

模拟不同 workload：  

👉 实验现象：  

- static 有时更快（反直觉），如果编译器进行优化且比dynamic的调度成本小的话
- dynamic 有时更快  

👉 原因：  

dynamic 具备更强 load balancing  

---

## 六、常见错误 (Common Mistakes)  

### ❌ 1. atomic 写法错误  

错误：  

    #pragma omp atomic {
        ...
    }

正确：  

    #pragma omp atomic
    sum += a[i];

---

### ❌ 2. max/min 初始化错误  

错误：  

    int max_val = INT_MAX;
    int min_val = INT_MIN;

正确：  

    int max_val = INT_MIN;
    int min_val = INT_MAX;

---

### ❌ 3. reduction 写法错误  

错误：  

    reduction(+:sum) \ reduction(+:even_count)

正确：  

    reduction(+:sum, even_count)

---

### ❌ 4. reduction "\" 后有空格  

必须写成：  

    \

后面不能有空格  

---


## 七、底层逻辑 (Engineering Insight)  

### 1. OpenMP 执行模型  

    Fork → Parallel → Join  

---

### 2. 性能本质  

性能 = 计算时间 + 同步开销 + 调度开销  

---

### 3. 三种同步机制本质  

- critical → 全局锁 → 串行  
- atomic → 局部锁 → 较快  
- reduction → 无锁 → 最快  

---

### 4. dynamic vs static  

- static → 分配固定  
- dynamic → 动态调度  

👉 负载不均时：  

dynamic 更优  

---

## 八、本节收获 (Takeaways)  

- 掌握 OpenMP 并行模型  
- 理解 race condition  
- 学会 critical / atomic / reduction  
- 理解调度策略  
- 完成性能对比实验  
- 发现反直觉现象（dynamic 更快）  

