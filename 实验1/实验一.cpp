#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
using namespace std;

#define MAX 10
typedef struct PCB {
    char name; // 进程名
    int status; // 进程状态
    int priority; // 进程优先级
    int time; // 进程持续时间
    int original_time; // 进程初始服务时间
} PCB;

typedef struct QNode { // 队列结点
    PCB data; // PCB数据
    struct QNode* next; // 队列指针
} QNode;

typedef struct {
    QNode* front; // 队头指针
    QNode* rear; // 队尾指针
} LinkQueue;

struct DispatchRecord {
    int round;
    char process_name;
    int cpu_time_slice;
    bool completed;
    int remaining_time;
    int total_service_time;
    int priority;
};

// 初始化队列
void InitQueue(LinkQueue& Q) {
    Q.front = Q.rear = new QNode;
    Q.front->next = NULL;
}

// 尾插法入队
bool PushQueue(LinkQueue& Q, PCB new_data) {
    QNode* p = new QNode;
    p->data = new_data;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return true;
}

// 出队
PCB PopQueue(LinkQueue& Q) {
    PCB zero;
    zero.name = -1; zero.time = 0; zero.priority = -1; zero.status = 0; zero.original_time = 0;
    if (Q.front == Q.rear)
        return zero;
    else {
        QNode* p = Q.front->next;
        Q.front->next = p->next; // 队首指针指向第二个元素
        if (Q.rear == p) // 如果只剩下一个数据
            Q.rear = Q.front;
        PCB data = p->data; // 获取队头数据
        delete p;
        return data;
    }
}

// 遍历输出队列, 输出的是进程名和进程所需时间
void OuputQueue(LinkQueue Q) {
    QNode* p = Q.front;
    if (p->next == NULL)
        return;
    while (p->next->next) {
        cout << p->next->data.name << "(" << p->next->data.time << ") --> ";
        p = p->next;
    }
    cout << p->next->data.name << "(" << p->next->data.time << ")";
}

// 打印等待队列和就绪队列函数
void Print(LinkQueue& Q1, LinkQueue& Q2) {
    cout << "就绪队列为:" << endl;
    OuputQueue(Q2);
    cout << endl << "等待队列为:" << endl;
    OuputQueue(Q1);
}

// 创建并初始化十个PCB
void Init(LinkQueue& Q1, LinkQueue& Q2, map<char, int>& service_times) {
    srand(time(NULL)); // 重新选择种子
    PCB pcb[10]; // 定义PCB类型的数组
    int m = 0;
    int numbers[10]; // 不一样的10个数的随机数组
    int pri[10]; // 不一样的10个数的随机数组

    while (m < 10) {
        int number = rand() % 10 + 1; // 生成1到10之间的随机整数
        bool duplicate = false;
        for (int n = 0; n < m; n++) {
            if (numbers[n] == number) {
                duplicate = true; // 数字重复，跳出循环
                break;
            }
        }
        if (!duplicate) {
            numbers[m] = number; // 添加至数组中
            m++;
        }
    }

    m = 0;
    while (m < 10) {
        int number = rand() % 10 + 1; // 生成1到10之间的随机整数
        bool duplicate = false;
        for (int n = 0; n < m; n++) {
            if (pri[n] == number) {
                duplicate = true; // 数字重复，跳出循环
                break;
            }
        }
        if (!duplicate) {
            pri[m] = number; // 添加至数组中
            m++;
        }
    }

    for (int i = 0; i < 10; i++) {
        pcb[i].name = char(numbers[i] + 64); // 随机生成进程的名字，为A~J中的任何一个
        pcb[i].priority = pri[i]; // 优先级(1~10)
        pcb[i].time = rand() % 50 + 1; // 随机生成需要在处理机上执行的时间（1~50ms）
        pcb[i].status = rand() % 2; // 随机生成pcb的状态，0为就绪，1为等待
        pcb[i].original_time = pcb[i].time; // 保存初始服务时间

        service_times[pcb[i].name] = pcb[i].original_time; // 记录总服务时间

        if (pcb[i].status == 1)
            PushQueue(Q1, pcb[i]); // 状态为1则入等待队列
        else
            PushQueue(Q2, pcb[i]); // 状态为0则入就绪队列
    }

    // 输出表头
    cout << left << setw(10) << "进程名"
        << setw(10) << "状态"
        << setw(10) << "优先级"
        << setw(10) << "持续时间" << endl;

    // 输出每个进程的信息
    for (int i = 0; i < 10; i++) {
        cout << left << setw(10) << pcb[i].name
            << setw(10) << pcb[i].status
            << setw(10) << pcb[i].priority
            << setw(10) << pcb[i].time << endl;
    }
}

// 基于最短执行时间调度算法的获得最小时间函数
PCB LeastTime(LinkQueue& Q) {
    QNode* p = Q.front;
    PCB m;
    m.name = -1; m.time = 8000; m.priority = -1; m.status = 0;
    int min_time = 800;
    cout << "就绪队列中各进程及其所需要的时间分别为：";
    // 逐个遍历输出就绪队列各进程及所需时间
    while (p->next) {
        cout << p->next->data.name << "(" << p->next->data.time << ")  ";
        if (p->next->data.time <= min_time) // 遍历过程即找到最短时间
            min_time = p->next->data.time;
        p = p->next;
    }
    cout << endl;
    p = Q.front;
    // 寻找哪个进程的所需时间与最短时间相同
    while (p->next->next) {
        if (p->next->data.time == min_time) {
            m = p->next->data; // m即为所要的进程
            QNode* s = p->next;
            p->next = s->next;
            break;
        }
        p = p->next;
    }
    if (p->next->data.time == min_time) {
        m = p->next->data;
        Q.rear = p;
        Q.rear->next = NULL;
    }
    return m;
}

// 基于最大优先级调度算法的获得最大优先级函数
PCB Priority(LinkQueue& Q) {
    QNode* p = Q.front;
    PCB m;
    m.name = -1; m.time = 0; m.priority = -1; m.status = 0;
    int priority = Q.front->next->data.priority;
    cout << "就绪队列中各进程及其优先级为：";
    // 逐个遍历输出就绪队列各进程及其优先级
    while (p->next) {
        cout << p->next->data.name << "(" << p->next->data.priority << ")  ";
        if (p->next->data.priority > priority)
            priority = p->next->data.priority; // 遍历过程即找到最大优先级
        p = p->next;
    }
    cout << endl;
    p = Q.front;
    // 寻找哪个进程的优先级与最大优先级相同
    while (p->next->next) {
        if (p->next->data.priority == priority) {
            m = p->next->data; // m即为所需的进程
            QNode* s = p->next;
            p->next = s->next;
            break;
        }
        p = p->next;
    }
    if (p->next->data.priority == priority) {
        m = p->next->data;
        Q.rear = p;
        Q.rear->next = NULL;
    }
    return m;
}

// 基于先进先出调度算法
void Dispatch(LinkQueue Q1, LinkQueue Q2, int Dispatch_method, vector<DispatchRecord>& records, map<char, int>& service_times) {
    int i = 1;
    while (Q1.front->next != NULL || Q2.front->next != NULL) {
        cout << endl << "============================" << endl;
        cout << "第" << i << "轮" << endl;
        int Rand = rand() % 2; // 得到一个随机数0/1
        cout << "程序执行过程中生成随机数：" << Rand;
        if (Rand == 0) {
            cout << " 因此，没有等待进程进入就绪队列" << endl;
        }
        else {
            if (Q1.front == Q1.rear) {
                cout << "因此，将等待队列中的第一个PCB加入就绪队列的队尾,但是，等待队列为空，不移交进程" << endl;
            }
            else {
                PCB temp = PopQueue(Q1); // 获取等待队列中的第一个PCB
                cout << " 因此，将等待队列中的第一个PCB" << temp.name << "加入就绪队列的队尾" << endl;
                PushQueue(Q2, temp); // 将等待队列中的第一个PCB加入就绪队列的队尾
            }
        }
        if (Q2.front == Q2.rear) {
            cout << "当前没有进程为就绪状态" << endl;
        }
        else {
            PCB first;
            first.name = -1; first.time = 0; first.priority = -1; first.status = 0; first.original_time = 0;
            if (Dispatch_method == 1)
                first = PopQueue(Q2); // 获取就绪队列中的第一个PCB
            if (Dispatch_method == 2)
                first = Priority(Q2);
            if (Dispatch_method == 3)
                first = LeastTime(Q2);
            cout << "当前执行进程为:" << first.name << "  该进程仍需要的时间为:" << first.time << endl;
            int GivenTime = (rand() % 15 + 6);
            cout << "CPU分配的时间片为:" << GivenTime;

            DispatchRecord record;
            record.round = i;
            record.process_name = first.name;
            record.cpu_time_slice = GivenTime;
            record.total_service_time = service_times[first.name];
            record.priority = first.priority;

            if (first.time <= GivenTime) {
                cout << "  时间足够，进程" << first.name << "将被执行完毕" << endl;
                record.completed = true;
                record.remaining_time = 0;
            }
            else {
                first.time = first.time - GivenTime;
                int s = rand() % 2; // 执行进程没有执行完成,产生随机数0或1，
                cout << endl << "时间片被用完，仍需要的时间为" << first.time;
                cout << "  此时生成随机数：" << s;
                record.completed = false;
                record.remaining_time = first.time;

                if (s == 0) {
                    cout << " 因此，将进程" << first.name << "加入就绪队列队尾" << endl;
                    PushQueue(Q2, first);
                }
                else {
                    cout << " 因此，将进程" << first.name << "加入等待队列队尾" << endl;
                    PushQueue(Q1, first);
                }
            }

            records.push_back(record);
        }
        cout << "这一轮最终的等待队列和就绪队列为：" << endl;
        Print(Q1, Q2);
        i++;
    }
}

void PrintDispatchRecords(const vector<DispatchRecord>& records) {
    cout << left << setw(10) << "轮次"
        << setw(15) << "进程名"
        << setw(20) << "CPU时间片"
        << setw(15) << "是否完成"
        << setw(20) << "剩余时间"
        << setw(20) << "总服务时间"
        << setw(15) << "优先级" << endl;

    for (const auto& record : records) {
        cout << left << setw(10) << record.round
            << setw(15) << record.process_name
            << setw(20) << record.cpu_time_slice
            << setw(15) << (record.completed ? "是" : "否")
            << setw(20) << record.remaining_time
            << setw(20) << record.total_service_time
            << setw(15) << record.priority << endl;
    }
}

int main() {
    while (1) {
        LinkQueue Q1, Q2; // 定义Q1等待队列，Q2就绪队列
        InitQueue(Q1); // 初始化Q1等待队列
        InitQueue(Q2); // 初始化Q2就绪队列
        cout << "请输入您的选择(选项：1,2,3)" << endl;
        cout << "1.基于先进先出调度算法" << endl;
        cout << "2.基于优先级的调度算法" << endl;
        cout << "3.基于最短执行时间调度算法" << endl;
        cout << "您的选择为:";
        int choice; // 用户的选择
        cin >> choice;
        if (choice == 1 || choice == 2 || choice == 3) {
            map<char, int> service_times;
            Init(Q1, Q2, service_times);
            cout << "-------------------------" << endl;
            cout << "初始化十个PCB的结果如下：" << endl;
            Print(Q1, Q2);
            cout << endl << "-------------------------" << endl;

            vector<DispatchRecord> records;
            Dispatch(Q1, Q2, choice, records, service_times);
            cout << "==============================" << endl;
            cout << "程序执行完毕" << endl;
            cout << "==============================" << endl;
            cout << endl << endl << endl;
            cout << "==============================" << endl;
            cout << "下一轮进程调度如下所示：" << endl;

            // 打印调度记录
            PrintDispatchRecords(records);

            // 等待用户按任意键继续
            cout << "按任意键继续..." << endl;
            cin.ignore();
            cin.get();
        }
        else {
            cout << "输入错误，请重新输入！！" << endl;
        }
    }
    return 0;
}