# OS-Experiment
这里存储着北京林业大学操作系统课程的实验。其中包含了实验内容，实验要求以及实验报告。

如上实验均使用c++撰写，建议使用**vs2022的控制台程序**运行！

当然，也可以使用Dev cpp这个应用运行。

三个实验的内容分别放到了三个文件夹当中，其中**cpp文件为源码**，而其中的**实验报告**.doc是对其源代码实现的详细注释。

剩余每个文件夹中的文件即为每个实验的实验要求。

## 实验内容

### 实验一：进程调度

进程调度模拟程序：假设有10个进程需要在CPU上执行，分别用：

- [x] 先进先出调度算法；
- [x] 基于优先数的调度算法；
- [x] 最短执行时间调度算法

确定这10个进程在CPU上的执行过程。要求每次进程调度时在屏幕上显示：

- 当前执行进程；
- 就绪队列；
- 等待队列

### 实验二：生产者与消费者

1. 由用户指定要产生的进程及其类别，存入进入就绪队列
2. 调度程序从就绪队列中提取一个就绪进程运行。如果申请的资源被阻塞则进入相应的等待队列，调度程序调度就绪队列中的下一个进程。进程运行结束时，会检查对应的等待队列，激活队列中的进程进入就绪队列。运行结束的进程进入over链表。重复这一过程直至就绪队列为空。　　
3.  程序询问是否要继续？如果要转直1开始执行，否则退出程序。

### 实验三：存储管理

可变分区调度算法有：

- 最先适应分配算法，
- 最优适应分配算法，
- 最坏适应算法。

用户提出内存空间的申请；系统根据申请者的要求，按照一定的分配策略分析内存空间的使用情况，找出能满足请求的空闲区，分给申请者；当程序执行完毕或主动归还内存资源时，系统要收回它所占用的内存空间或它归还的部分内存空间。

根据可变分区的主存分配思想，首先设计记录主存使用情况的数据表格，用来记录空闲区和作业占用的区域，即“已分配区表”和“空闲区表”。然后在数据表格上进行主存的分配，其主存分配算法选择一种算法。程序中可选择进行主存分配或主存回收，若是主存分配，输入作业名和所需主存空间大小；若是回收，输入回收作业的作业名，以循环进行主存分配和回收。
