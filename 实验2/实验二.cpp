#include <iostream>
#include <vector>
#define MAXSIZE 100  // Just a limit to avoid overflows in array, this can be adjusted as needed

using namespace std;

typedef struct PCB {
    int flag;  // Process type flag: 0=producer, 1=consumer
    int id;  // Process ID
    char state;  // Process state: 'W'=waiting, 'R'=ready, 'F'=finished
    char name;  // Process product name
} PCB;

typedef struct QNode {  // Queue node
    PCB data;  // PCB data
    struct QNode* next;  // Queue pointer
} QNode;

typedef struct {
    QNode* front;  // Queue front pointer
    QNode* rear;  // Queue rear pointer
} LinkQueue;

LinkQueue ready, producer, consumer, over;  // Queue declarations
vector<int> buffer;  // Buffer as a dynamic array
int buffersize;  // Buffer size
int product = 0;  // Product count
int head = 0;  // Buffer head
int tail = 0;  // Buffer tail

int mutex = 1;  // Mutex semaphore
int empty_;  // Empty buffer semaphore
int full = 0;  // Full buffer semaphore

void InitQueue(LinkQueue& Q) {  // Initialize queue
    Q.front = Q.rear = new QNode;
    Q.front->next = NULL;
}

// Enqueue at the rear
bool PushQueue(LinkQueue& Q, PCB new_data) {
    QNode* p = new QNode;
    p->data = new_data;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return true;
}

// Dequeue from the front
PCB PopQueue(LinkQueue& Q) {
    PCB zero;
    zero.flag = -1; zero.id = 0; zero.state = -1; zero.name = 0;
    if (Q.front == Q.rear)
        return zero;
    else {
        QNode* p = Q.front->next;
        Q.front->next = p->next;  // Front pointer points to the second element
        if (Q.rear == p)  // If only one data remains
            Q.rear = Q.front;
        PCB data = p->data;
        delete p;
        return data;  // Get the front data
    }
}

void InitProducts() {
    head = 0;  // Reset head
    tail = 0;  // Reset tail
    mutex = 1;  // Reset mutex semaphore
    empty_ = buffersize;  // Reset empty semaphore
    full = 0;  // Reset full semaphore
}

int P(int& s) {  // P operation
    if (s >= 1) {
        s = s - 1;
        return s;
    }
    else {
        return -1;
    }
}

int V(int& s) {  // V operation
    s = s + 1;
    return s;
}

// Wake consumer
void Wakeconsumer(LinkQueue& consumer, LinkQueue& ready) {
    if (consumer.front->next != NULL) {  // Wake only if the queue is not empty
        PCB p = PopQueue(consumer);  // Dequeue from waiting queue
        p.state = 'R';  // Change state to ready
        PushQueue(ready, p);  // Enqueue to ready queue
        cout << "唤醒消费者进程 " << p.id << "，插入就绪队尾" << endl;
    }
    else {
        cout << "消费者等待队列为空，没有可唤醒对象" << endl;
    }
}

// Wake producer
void Wakeproducer(LinkQueue& producer, LinkQueue& ready) {
    if (producer.front->next != NULL) {  // Wake only if the queue is not empty
        PCB p = PopQueue(producer);  // Dequeue from waiting queue
        p.state = 'R';  // Change state to ready
        PushQueue(ready, p);  // Enqueue to ready queue
        cout << "唤醒生产者进程 " << p.id << "，插入就绪队尾" << endl;
    }
    else {
        cout << "生产者等待队列为空，没有可唤醒对象" << endl;
    }
}

void DisplayBuffer() {
    cout << "缓冲区: ";
    for (int i = 0; i < buffersize; ++i) {
        if (buffer[i] == -1) {
            cout << "空 ";
        }
        else {
            cout << buffer[i] << " ";
        }
    }
    cout << endl;
    cout << "产品总数: " << product << ", 放入缓冲区指针(tail): " << tail << ", 取走缓冲区指针(head): " << head << endl;
}

void DisplayQueue(LinkQueue Q, const string& name) {
    cout << name << "内容如下:" << endl;
    QNode* p = Q.front;
    if (p->next == NULL) {
        cout << "--------队列为空------------" << endl;
        return;
    }
    while (p->next) {
        cout << "进程类型: ";
        if (p->next->data.flag)
            cout << "消费者";
        else
            cout << "生产者";
        cout << "\t进程号: " << p->next->data.id << "\t进程状态: ";
        if (p->next->data.state == 'F')
            cout << "完成" << endl;
        else if (p->next->data.state == 'R')
            cout << "就绪" << endl;
        else
            cout << "等待" << endl;
        p = p->next;
    }
}

void DisplayAll() {
    DisplayBuffer();
    DisplayQueue(ready, "就绪队列");
    DisplayQueue(producer, "生产者等待队列");
    DisplayQueue(consumer, "消费者等待队列");
    DisplayQueue(over, "完成队列");
}

int main() {
    bool flag = true;
    InitQueue(ready);  // Initialize queues
    InitQueue(producer);
    InitQueue(consumer);
    InitQueue(over);

    cout << "请输入缓存区的大小:" << endl;
    cin >> buffersize;
    buffer.resize(buffersize, -1);  // Resize buffer to the input size and initialize with -1
    InitProducts();  // Initialize products and semaphores

    int count = 1;  // Process system ID
    while (flag) {
        int num;
        cout << "请输入创建进程的个数:" << endl;
        cin >> num;

        for (int i = 0; i < num; i++) {
            PCB p;
            p.id = count++;  // Assign process ID
            p.state = 'R';  // Ready state
            cout << "请输入0或1, 0表示生产者，1表示消费者:" << endl;
            cin >> p.flag;
            if (p.flag) {
                cout << "第" << i + 1 << "个进程: 进程类型: 消费者";
                PushQueue(ready, p);
            }
            else {
                cout << "第" << i + 1 << "个进程: 进程类型: 生产者";
                PushQueue(ready, p);
            }
            cout << "    进程状态: " << p.state << "    进程系统号: " << p.id << endl;
        }

        // Process all ready queue processes
        while (ready.front->next != NULL) {
            cout << endl;
            cout << "------------------------------" << endl;
            PCB p = PopQueue(ready);
            if (p.flag == 0) {  // Producer process
                if (P(empty_) >= 0 && P(mutex) >= 0) {  // P operation to apply for critical resource
                    buffer[tail] = p.id;  // Fill buffer with process ID
                    product++;
                    tail = (tail + 1) % buffersize;
                    p.state = 'F';  // Change state to finished
                    PushQueue(over, p);  // Enqueue to over queue
                    cout << "生产者进程 " << p.id << " 放入产品，产品总数: " << product << endl;
                    V(mutex);
                    V(full);  // V operation
                    Wakeconsumer(consumer, ready);  // Wake consumer
                }
                else {
                    p.state = 'W';  // Change state to waiting
                    PushQueue(producer, p);  // Enqueue to producer waiting queue
                    cout << "生产者进程 " << p.id << " 进入等待队列" << endl;
                }
            }
            else {  // Consumer process
                if (P(full) >= 0 && P(mutex) >= 0) {  // P operation to apply for resources
                    int goods = buffer[head];  // Get product from buffer
                    buffer[head] = -1;  // Clear buffer slot
                    product--;
                    head = (head + 1) % buffersize;
                    p.state = 'F';  // Change state to finished
                    PushQueue(over, p);  // Enqueue to over queue
                    cout << "消费者进程 " << p.id << " 取走产品，产品总数: " << product << endl;
                    V(mutex);
                    V(empty_);  // V operation
                    Wakeproducer(producer, ready);  // Wake producer
                }
                else {
                    p.state = 'W';  // Change state to waiting
                    PushQueue(consumer, p);  // Enqueue to consumer waiting queue
                    cout << "消费者进程 " << p.id << " 进入等待队列" << endl;
                }
            }
            // Display the state after each process
            cout << "当前状态如下:" << endl;
            DisplayAll();
            cout << "------------------------------" << endl;
            cout << endl;
        }

        cout << "是否继续输入（y/n）？" << endl;
        char ch;
        cin >> ch;
        if (ch == 'n') {
            flag = false;
        }
    }

    cout << "最终状态如下:" << endl;
    DisplayAll();
    return 0;
}
