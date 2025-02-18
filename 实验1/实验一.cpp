#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
using namespace std;

#define MAX 10
typedef struct PCB {
    char name; // ������
    int status; // ����״̬
    int priority; // �������ȼ�
    int time; // ���̳���ʱ��
    int original_time; // ���̳�ʼ����ʱ��
} PCB;

typedef struct QNode { // ���н��
    PCB data; // PCB����
    struct QNode* next; // ����ָ��
} QNode;

typedef struct {
    QNode* front; // ��ͷָ��
    QNode* rear; // ��βָ��
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

// ��ʼ������
void InitQueue(LinkQueue& Q) {
    Q.front = Q.rear = new QNode;
    Q.front->next = NULL;
}

// β�巨���
bool PushQueue(LinkQueue& Q, PCB new_data) {
    QNode* p = new QNode;
    p->data = new_data;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return true;
}

// ����
PCB PopQueue(LinkQueue& Q) {
    PCB zero;
    zero.name = -1; zero.time = 0; zero.priority = -1; zero.status = 0; zero.original_time = 0;
    if (Q.front == Q.rear)
        return zero;
    else {
        QNode* p = Q.front->next;
        Q.front->next = p->next; // ����ָ��ָ��ڶ���Ԫ��
        if (Q.rear == p) // ���ֻʣ��һ������
            Q.rear = Q.front;
        PCB data = p->data; // ��ȡ��ͷ����
        delete p;
        return data;
    }
}

// �����������, ������ǽ������ͽ�������ʱ��
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

// ��ӡ�ȴ����к;������к���
void Print(LinkQueue& Q1, LinkQueue& Q2) {
    cout << "��������Ϊ:" << endl;
    OuputQueue(Q2);
    cout << endl << "�ȴ�����Ϊ:" << endl;
    OuputQueue(Q1);
}

// ��������ʼ��ʮ��PCB
void Init(LinkQueue& Q1, LinkQueue& Q2, map<char, int>& service_times) {
    srand(time(NULL)); // ����ѡ������
    PCB pcb[10]; // ����PCB���͵�����
    int m = 0;
    int numbers[10]; // ��һ����10�������������
    int pri[10]; // ��һ����10�������������

    while (m < 10) {
        int number = rand() % 10 + 1; // ����1��10֮����������
        bool duplicate = false;
        for (int n = 0; n < m; n++) {
            if (numbers[n] == number) {
                duplicate = true; // �����ظ�������ѭ��
                break;
            }
        }
        if (!duplicate) {
            numbers[m] = number; // �����������
            m++;
        }
    }

    m = 0;
    while (m < 10) {
        int number = rand() % 10 + 1; // ����1��10֮����������
        bool duplicate = false;
        for (int n = 0; n < m; n++) {
            if (pri[n] == number) {
                duplicate = true; // �����ظ�������ѭ��
                break;
            }
        }
        if (!duplicate) {
            pri[m] = number; // �����������
            m++;
        }
    }

    for (int i = 0; i < 10; i++) {
        pcb[i].name = char(numbers[i] + 64); // ������ɽ��̵����֣�ΪA~J�е��κ�һ��
        pcb[i].priority = pri[i]; // ���ȼ�(1~10)
        pcb[i].time = rand() % 50 + 1; // ���������Ҫ�ڴ������ִ�е�ʱ�䣨1~50ms��
        pcb[i].status = rand() % 2; // �������pcb��״̬��0Ϊ������1Ϊ�ȴ�
        pcb[i].original_time = pcb[i].time; // �����ʼ����ʱ��

        service_times[pcb[i].name] = pcb[i].original_time; // ��¼�ܷ���ʱ��

        if (pcb[i].status == 1)
            PushQueue(Q1, pcb[i]); // ״̬Ϊ1����ȴ�����
        else
            PushQueue(Q2, pcb[i]); // ״̬Ϊ0�����������
    }

    // �����ͷ
    cout << left << setw(10) << "������"
        << setw(10) << "״̬"
        << setw(10) << "���ȼ�"
        << setw(10) << "����ʱ��" << endl;

    // ���ÿ�����̵���Ϣ
    for (int i = 0; i < 10; i++) {
        cout << left << setw(10) << pcb[i].name
            << setw(10) << pcb[i].status
            << setw(10) << pcb[i].priority
            << setw(10) << pcb[i].time << endl;
    }
}

// �������ִ��ʱ������㷨�Ļ����Сʱ�亯��
PCB LeastTime(LinkQueue& Q) {
    QNode* p = Q.front;
    PCB m;
    m.name = -1; m.time = 8000; m.priority = -1; m.status = 0;
    int min_time = 800;
    cout << "���������и����̼�������Ҫ��ʱ��ֱ�Ϊ��";
    // �����������������и����̼�����ʱ��
    while (p->next) {
        cout << p->next->data.name << "(" << p->next->data.time << ")  ";
        if (p->next->data.time <= min_time) // �������̼��ҵ����ʱ��
            min_time = p->next->data.time;
        p = p->next;
    }
    cout << endl;
    p = Q.front;
    // Ѱ���ĸ����̵�����ʱ�������ʱ����ͬ
    while (p->next->next) {
        if (p->next->data.time == min_time) {
            m = p->next->data; // m��Ϊ��Ҫ�Ľ���
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

// ����������ȼ������㷨�Ļ��������ȼ�����
PCB Priority(LinkQueue& Q) {
    QNode* p = Q.front;
    PCB m;
    m.name = -1; m.time = 0; m.priority = -1; m.status = 0;
    int priority = Q.front->next->data.priority;
    cout << "���������и����̼������ȼ�Ϊ��";
    // �����������������и����̼������ȼ�
    while (p->next) {
        cout << p->next->data.name << "(" << p->next->data.priority << ")  ";
        if (p->next->data.priority > priority)
            priority = p->next->data.priority; // �������̼��ҵ�������ȼ�
        p = p->next;
    }
    cout << endl;
    p = Q.front;
    // Ѱ���ĸ����̵����ȼ���������ȼ���ͬ
    while (p->next->next) {
        if (p->next->data.priority == priority) {
            m = p->next->data; // m��Ϊ����Ľ���
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

// �����Ƚ��ȳ������㷨
void Dispatch(LinkQueue Q1, LinkQueue Q2, int Dispatch_method, vector<DispatchRecord>& records, map<char, int>& service_times) {
    int i = 1;
    while (Q1.front->next != NULL || Q2.front->next != NULL) {
        cout << endl << "============================" << endl;
        cout << "��" << i << "��" << endl;
        int Rand = rand() % 2; // �õ�һ�������0/1
        cout << "����ִ�й����������������" << Rand;
        if (Rand == 0) {
            cout << " ��ˣ�û�еȴ����̽����������" << endl;
        }
        else {
            if (Q1.front == Q1.rear) {
                cout << "��ˣ����ȴ������еĵ�һ��PCB����������еĶ�β,���ǣ��ȴ�����Ϊ�գ����ƽ�����" << endl;
            }
            else {
                PCB temp = PopQueue(Q1); // ��ȡ�ȴ������еĵ�һ��PCB
                cout << " ��ˣ����ȴ������еĵ�һ��PCB" << temp.name << "����������еĶ�β" << endl;
                PushQueue(Q2, temp); // ���ȴ������еĵ�һ��PCB����������еĶ�β
            }
        }
        if (Q2.front == Q2.rear) {
            cout << "��ǰû�н���Ϊ����״̬" << endl;
        }
        else {
            PCB first;
            first.name = -1; first.time = 0; first.priority = -1; first.status = 0; first.original_time = 0;
            if (Dispatch_method == 1)
                first = PopQueue(Q2); // ��ȡ���������еĵ�һ��PCB
            if (Dispatch_method == 2)
                first = Priority(Q2);
            if (Dispatch_method == 3)
                first = LeastTime(Q2);
            cout << "��ǰִ�н���Ϊ:" << first.name << "  �ý�������Ҫ��ʱ��Ϊ:" << first.time << endl;
            int GivenTime = (rand() % 15 + 6);
            cout << "CPU�����ʱ��ƬΪ:" << GivenTime;

            DispatchRecord record;
            record.round = i;
            record.process_name = first.name;
            record.cpu_time_slice = GivenTime;
            record.total_service_time = service_times[first.name];
            record.priority = first.priority;

            if (first.time <= GivenTime) {
                cout << "  ʱ���㹻������" << first.name << "����ִ�����" << endl;
                record.completed = true;
                record.remaining_time = 0;
            }
            else {
                first.time = first.time - GivenTime;
                int s = rand() % 2; // ִ�н���û��ִ�����,���������0��1��
                cout << endl << "ʱ��Ƭ�����꣬����Ҫ��ʱ��Ϊ" << first.time;
                cout << "  ��ʱ�����������" << s;
                record.completed = false;
                record.remaining_time = first.time;

                if (s == 0) {
                    cout << " ��ˣ�������" << first.name << "����������ж�β" << endl;
                    PushQueue(Q2, first);
                }
                else {
                    cout << " ��ˣ�������" << first.name << "����ȴ����ж�β" << endl;
                    PushQueue(Q1, first);
                }
            }

            records.push_back(record);
        }
        cout << "��һ�����յĵȴ����к;�������Ϊ��" << endl;
        Print(Q1, Q2);
        i++;
    }
}

void PrintDispatchRecords(const vector<DispatchRecord>& records) {
    cout << left << setw(10) << "�ִ�"
        << setw(15) << "������"
        << setw(20) << "CPUʱ��Ƭ"
        << setw(15) << "�Ƿ����"
        << setw(20) << "ʣ��ʱ��"
        << setw(20) << "�ܷ���ʱ��"
        << setw(15) << "���ȼ�" << endl;

    for (const auto& record : records) {
        cout << left << setw(10) << record.round
            << setw(15) << record.process_name
            << setw(20) << record.cpu_time_slice
            << setw(15) << (record.completed ? "��" : "��")
            << setw(20) << record.remaining_time
            << setw(20) << record.total_service_time
            << setw(15) << record.priority << endl;
    }
}

int main() {
    while (1) {
        LinkQueue Q1, Q2; // ����Q1�ȴ����У�Q2��������
        InitQueue(Q1); // ��ʼ��Q1�ȴ�����
        InitQueue(Q2); // ��ʼ��Q2��������
        cout << "����������ѡ��(ѡ�1,2,3)" << endl;
        cout << "1.�����Ƚ��ȳ������㷨" << endl;
        cout << "2.�������ȼ��ĵ����㷨" << endl;
        cout << "3.�������ִ��ʱ������㷨" << endl;
        cout << "����ѡ��Ϊ:";
        int choice; // �û���ѡ��
        cin >> choice;
        if (choice == 1 || choice == 2 || choice == 3) {
            map<char, int> service_times;
            Init(Q1, Q2, service_times);
            cout << "-------------------------" << endl;
            cout << "��ʼ��ʮ��PCB�Ľ�����£�" << endl;
            Print(Q1, Q2);
            cout << endl << "-------------------------" << endl;

            vector<DispatchRecord> records;
            Dispatch(Q1, Q2, choice, records, service_times);
            cout << "==============================" << endl;
            cout << "����ִ�����" << endl;
            cout << "==============================" << endl;
            cout << endl << endl << endl;
            cout << "==============================" << endl;
            cout << "��һ�ֽ��̵���������ʾ��" << endl;

            // ��ӡ���ȼ�¼
            PrintDispatchRecords(records);

            // �ȴ��û������������
            cout << "�����������..." << endl;
            cin.ignore();
            cin.get();
        }
        else {
            cout << "����������������룡��" << endl;
        }
    }
    return 0;
}