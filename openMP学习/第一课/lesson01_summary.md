# Lesson 01: OpenMP 基础总结

## 一、学习目标 (Learning Goals)

本节主要学习 OpenMP 在 C++ 中的基本并行编程模型，理解线程如何分工执行任务，以及如何避免并发问题。

---

## 二、核心内容 (Core Concepts)

### 1. 并行循环 (Parallel For)

使用：

    #pragma omp parallel for

将 for 循环的迭代分配给多个线程执行。

👉 前提：循环之间必须是 **independent（无依赖）**

---

### 2. 调度策略 (Scheduling)

OpenMP 提供不同的任务分配方式：

- `schedule(static)`
  - 编译时就分配好任务
  - 适合 workload 均匀

- `schedule(dynamic)`
  - 运行时动态分配
  - 适合 workload 不均匀

👉 本质区别：  
**load balancing（负载均衡）能力不同**

---

### 3. 共享变量 vs 私有变量

- `shared`
  - 所有线程共享同一份变量
  - ❗容易产生 race condition

- `private`
  - 每个线程都有独立副本
  - 不会互相影响

---

### 4. 竞争条件 (Race Condition)

多个线程同时修改同一个变量时，结果不确定。

👉 典型例子：

    sum += a[i];

---

### 5. 临界区 (Critical Section)

    #pragma omp critical

👉 特点：

- 相当于加锁（lock）
- 同一时间只能一个线程进入
- ❗性能很差（严重串行化）

---

### 6. 原子操作 (Atomic)

    #pragma omp atomic

👉 特点：

- 只保护**单个操作**
- 比 critical 更轻量
- 只能用于简单语句（如 `+=`）

---

### 7. 归约 (Reduction)

    #pragma omp parallel for reduction(+:sum)

👉 原理：

- 每个线程有自己的局部变量（local copy）
- 最后统一合并（combine）

👉 优点：

- 无锁（no lock）
- 高性能（high performance）
- 推荐优先使用

---

## 三、重要对比 (Key Comparisons)

| 方法 | 是否加锁 | 性能 | 使用场景 |
|------|--------|------|--------|
| critical | 是 | 慢 | 任意复杂代码 |
| atomic | 是（轻量） | 中 | 简单操作 |
| reduction | 否 | 快 | 累加、统计 |

---

## 四、底层理解 (Low-level Understanding)

OpenMP 本质是：

👉 **线程池（thread team） + 循环任务分配（work sharing）**

执行流程：

1. 创建线程（thread team）
2. 分配 iteration
3. 每个线程执行
4. 同步（barrier）
5. 合并结果（reduction）

---

## 五、本节最大收获 (Takeaways)

- 并行 ≠ 一定更快（锁会拖慢性能）
- reduction 是最优解（用于 aggregation）
- private 用来避免冲突
- shared 必须谨慎处理

---

## 六、下一步提升方向

- 深入理解 memory model（内存模型）
- 学习 thread affinity（线程绑定）
- 尝试更复杂的并行任务（如矩阵计算）