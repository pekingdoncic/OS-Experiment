#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

#define n 10                        //�ٶ�ϵͳ����������ҵ����Ϊn
#define m 10                        //�ٶ�ϵͳ����Ŀ����������Ϊm

struct                              //�ѷ�������Ķ��壺
{
    float address;                  //�ѷַ�����ʼ��ַ
    float length;                   //�ѷַ������ȣ���λΪ�ֽ�
    int flag;                       //�ѷ�������Ǽ�����־����0��ʾ����Ŀ
} used_table[n];                    //�ѷ������� 

struct                              //��������Ķ��壺
{
    float address;                  //��������ʼ��ַ
    float length;                   //���������ȣ���λΪ�ֽ�
    int flag;                       //��������Ǽ���Ŀ��0��ʾ����Ŀ��1��ʾδ����
} free_table[m];                    //��������

struct                              //��ҵ���еĶ��壺
{
    char name;                      //��ҵ��
    float length;                   //��ҵ��С
    int flag;                       //0δ����,1�ѷ���,2�����
    float address;                  //��ҵ����ʼ��ַ
} job[10];

//ȫ�ֱ���
int L = 1024;                       //���ڴ�
int job_count;                      //�û��������ҵ����

//�Զ��庯��
void First_fit();                   //������Ӧ�㷨
void Best_fit();                    //������Ӧ�㷨
void Worst_fit();                   //���Ӧ�㷨
void Init_table();                  //��ʼ���ѷ�������Ϳ��б�
void Init_job(int);                 //��ʼ����ҵ����
int Distribute(char, float,int);        //�����ڴ棬�����и�޸��ѷ�������Ϳ��б���Ϣ
void Reclaim(char);                 //�����ڴ棬���Ǻϲ����޸��ѷ�������Ϳ��б���Ϣ
void Display();                     //��ʾ�����������ѷ���������Ϣ
void Display_job();                 //��ʾ��ҵ������Ϣ

//������
int main()
{
    srand(time(0));
    cout << "��������Ҫ�����ڴ����ҵ������1-10��: ";
    cin >> job_count;
    if (job_count < 1 || job_count > 10) {
        cout << "�������ҵ������Ч�����������" << endl;
        return 1;
    }

    int choice;
    cout << "��ѡ���ڴ�����㷨��1-������Ӧ 2-������Ӧ 3-���Ӧ: "<<endl;
    cin >> choice;

    switch (choice) {
    case 1:
        First_fit();
        break;
    case 2:
        Best_fit();
        break;
    case 3:
        Worst_fit();
        break;
    default:
        cout << "������Ч�����������" << endl;
        return 1;
    }
    return 0;
}

void First_fit()                    //������Ӧ�㷨                
{
    Init_table();                   //��ʼ������
    Init_job(job_count);            //��ʼ����ҵ����
    Display();                      //�������
    Display_job();                  //�����ҵ����
    cout << endl;
    int i, num = job_count;         //��¼δ������ҵ��Ŀ

    while (num > 0)                 //һ�ַ���һ�ֻ��գ�ֱ����ҵ������
    {
        cout << "��ʼһ�ַ���" << endl << endl;
        for (i = 0; i < job_count; i++)  //��һ������ÿһ��δ������ҵ���з���
        {
            if (job[i].flag == 0)   //�ҵ�һ��δ����
            {
                cout << "�ҵ�δ������ҵ" << job[i].name << "��СΪ" << job[i].length << endl;
                if (Distribute(job[i].name, job[i].length,0) == 1)//�����и�޸�2�����1��������Ϣ
                {
                    cout << "�ɹ�����" << endl;
                    job[i].flag = 1;    //�ѷ���
                    Display();          //�������
                    Display_job();      //�����ҵ����
                }
                else
                {
                    cout << "����ҵ���޿ռ����" << endl;
                    Display();          //�������
                    Display_job();      //�����ҵ����
                }
            }
        }

        if (num == 0)
        {
            cout << "ȫ����ҵ�������˿ռ䣬�������" << endl;
            break;
        }

        cout << "��ʼһ�ֻ���" << endl << endl;
        int choice;
        for (i = 0; i < job_count; i++)  //�ڶ�������ÿһ���ѷ�����ҵ�������
        {
            if (job[i].flag == 1)    //�ѷ���
            {
                choice = rand() % 2; // �������ΧΪ0��1
                cout << "�����������Ϊ" << choice << endl;
                if (choice == 1)     //���
                {
                    cout << "������ҵ" << job[i].name << endl;
                    Reclaim(job[i].name); //���պϲ����޸�2����Ϣ�����ղ�����ʧ��
                    job[i].flag = 2;  //�����
                    cout << "��ҵ�Ѿ����" << endl;
                    Display();        //�������
                    Display_job();    //�����ҵ����
                }
                else
                {
                    cout << "�ݲ�������ҵ" << job[i].name << endl;
                    Display();        //�������
                    Display_job();    //�����ҵ����        
                }
            }
        }

        num = 0;
        for (i = 0; i < job_count; i++) {
            if (job[i].flag == 0) {
                num++;
            }
        }
    }
}

void Best_fit()                    //������Ӧ�㷨                
{
    Init_table();                   //��ʼ������
    Init_job(job_count);            //��ʼ����ҵ����
    Display();                      //�������
    Display_job();                  //�����ҵ����
    cout << endl;
    int i, num = job_count;         //��¼δ������ҵ��Ŀ

    while (num > 0)                 //һ�ַ���һ�ֻ��գ�ֱ����ҵ������
    {
        cout << "��ʼһ�ַ���" << endl << endl;
        sort(free_table, free_table + m, [](auto& a, auto& b) { return a.length < b.length; }); // ��������������
        for (i = 0; i < job_count; i++)  //��һ������ÿһ��δ������ҵ���з���
        {
            if (job[i].flag == 0)   //�ҵ�һ��δ����
            {
                cout << "�ҵ�δ������ҵ" << job[i].name << "��СΪ" << job[i].length << endl;
                if (Distribute(job[i].name, job[i].length,1) == 1)//�����и�޸�2�����1��������Ϣ
                {
                    cout << "�ɹ�����" << endl;
                    job[i].flag = 1;    //�ѷ���
                    Display();          //�������
                    Display_job();      //�����ҵ����
                }
                else
                {
                    cout << "����ҵ���޿ռ����" << endl;
                    Display();          //�������
                    Display_job();      //�����ҵ����
                }
            }
        }

        if (num == 0)
        {
            cout << "ȫ����ҵ�������˿ռ䣬�������" << endl;
            break;
        }

        cout << "��ʼһ�ֻ���" << endl << endl;
        int choice;
        for (i = 0; i < job_count; i++)  //�ڶ�������ÿһ���ѷ�����ҵ�������
        {
            if (job[i].flag == 1)    //�ѷ���
            {
                choice = rand() % 2; // �������ΧΪ0��1
                cout << "�����������Ϊ" << choice << endl;
                if (choice == 1)     //���
                {
                    cout << "������ҵ" << job[i].name << endl;
                    Reclaim(job[i].name); //���պϲ����޸�2����Ϣ�����ղ�����ʧ��
                    job[i].flag = 2;  //�����
                    cout << "��ҵ�Ѿ����" << endl;
                    Display();        //�������
                    Display_job();    //�����ҵ����
                }
                else
                {
                    cout << "�ݲ�������ҵ" << job[i].name << endl;
                    Display();        //�������
                    Display_job();    //�����ҵ����        
                }
            }
        }

        num = 0;
        for (i = 0; i < job_count; i++) {
            if (job[i].flag == 0) {
                num++;
            }
        }
    }
}

void Worst_fit()                    //���Ӧ�㷨                
{
    Init_table();                   //��ʼ������
    Init_job(job_count);            //��ʼ����ҵ����
    Display();                      //�������
    Display_job();                  //�����ҵ����
    cout << endl;
    int i, num = job_count;         //��¼δ������ҵ��Ŀ

    while (num > 0)                 //һ�ַ���һ�ֻ��գ�ֱ����ҵ������
    {
        cout << "��ʼһ�ַ���" << endl << endl;
        sort(free_table, free_table + m, [](auto& a, auto& b) { return a.length > b.length; }); // �����Ƚ�������
        for (i = 0; i < job_count; i++)  //��һ������ÿһ��δ������ҵ���з���
        {
            if (job[i].flag == 0)   //�ҵ�һ��δ����
            {
                cout << "�ҵ�δ������ҵ" << job[i].name << "��СΪ" << job[i].length << endl;
                if (Distribute(job[i].name, job[i].length,2) == 1)//�����и�޸�2�����1��������Ϣ
                {
                    cout << "�ɹ�����" << endl;
                    job[i].flag = 1;    //�ѷ���
                    Display();          //�������
                    Display_job();      //�����ҵ����
                }
                else
                {
                    cout << "����ҵ���޿ռ����" << endl;
                    Display();          //�������
                    Display_job();      //�����ҵ����
                }
            }
        }

        if (num == 0)
        {
            cout << "ȫ����ҵ�������˿ռ䣬�������" << endl;
            break;
        }

        cout << "��ʼһ�ֻ���" << endl << endl;
        int choice;
        for (i = 0; i < job_count; i++)  //�ڶ�������ÿһ���ѷ�����ҵ�������
        {
            if (job[i].flag == 1)    //�ѷ���
            {
                choice = rand() % 2; // �������ΧΪ0��1
                cout << "�����������Ϊ" << choice << endl;
                if (choice == 1)     //���
                {
                    cout << "������ҵ" << job[i].name << endl;
                    Reclaim(job[i].name); //���պϲ����޸�2����Ϣ�����ղ�����ʧ��
                    job[i].flag = 2;  //�����
                    cout << "��ҵ�Ѿ����" << endl;
                    Display();        //�������
                    Display_job();    //�����ҵ����
                }
                else
                {
                    cout << "�ݲ�������ҵ" << job[i].name << endl;
                    Display();        //�������
                    Display_job();    //�����ҵ����        
                }
            }
        }

        num = 0;
        for (i = 0; i < job_count; i++) {
            if (job[i].flag == 0) {
                num++;
            }
        }
    }
}

void Init_table()                            //��ʼ������            
{
    int i;
    for (i = 0; i < n; i++)
        used_table[i].flag = 0;              //�ѷ�������0�ձ��Ŀ����0(��ҵ��)�ѷ���

    free_table[0].address = 0;
    free_table[0].length = 2;
    free_table[0].flag = 1;

    free_table[1].address = free_table[0].address + free_table[0].length;
    free_table[1].length = 2;
    free_table[1].flag = 1;

    for (i = 2; i < m; i++)
    {
        free_table[i].address = free_table[i - 1].address + free_table[i - 1].length; //��ַ
        free_table[i].length = free_table[i - 1].length * 2;                         //��С
        free_table[i].flag = 1;                                                      //״̬
    }
    cout << "���������ѷ�������ʼ�����" << endl;
}

void Init_job(int job_count)                   //��ʼ����ҵ
{
    for (int i = 0; i < job_count; i++)
    {
        job[i].name = 'A' + i;                 //��ҵ����A~J
        job[i].length = rand() % (L / 4) + 1;  //��С����1~L/4
        job[i].flag = 0;                       //״̬��0δ���䣬1�ѷ��䣬2�ѽ���
        job[i].address = -1;                   // ��ʼ��ַΪ -1 ��ʾδ����
    }
    cout << "��ҵѭ�����г�ʼ�����" << endl;
}

void Display()                                                                    //�������
{
    cout << endl << "================================" << endl;
    cout << "��������(��ַ�ɵ��������)��" << endl;
    cout << "���" << "\t��ַ" << "\t��С" << "\t��־" << endl;;
    for (int i = 0; i < m; i++)
    {
        if (free_table[i].flag == 1)                                                //2��״̬
            cout << i + 1 << "\t" << free_table[i].address << "\t" << free_table[i].length << "\t����" << endl;
        else
            cout << i + 1 << "\t--\t--\t����Ŀ" << endl;
    }
    cout << "================================" << endl;
    cout << endl << "================================" << endl;
    cout << "�ѷ�������(��������)" << endl;
    cout << "���" << "\t��ַ" << "\t��С" << "\t��־" << endl;;
    for (int j = 0; j < n; j++)
    {
        if (used_table[j].flag == 0)                                                //2��״̬
            cout << j + 1 << "\t--\t--\t����Ŀ" << endl;
        else
            cout << j + 1 << "\t" << used_table[j].address << "\t" << used_table[j].length << "\t" << "�ѷ���" << endl;
    }
    cout << "================================" << endl;
}

void Display_job()                                                                //�����ҵ����
{
    cout << endl << "=========================" << endl;
    cout << "��ҵ���У�" << endl;
    cout << "��ҵ��" << "\t��С" << "\t��־" << "\t��ʼ��ַ" << endl;
    for (int i = 0; i < job_count; i++)
    {
        cout << job[i].name << "\t" << job[i].length;
        if (job[i].flag == 0)                                                        //3��״̬
            cout << "\tδ����\t" << "--" << endl;
        else if (job[i].flag == 1)
            cout << "\t�ѷ���\t" << job[i].address << endl;
        else
            cout << "\t�ѽ���\t" << job[i].address << endl;
    }
    cout << "=========================" << endl << endl;
}

int Distribute(char name, float len,int type)    //�����и�޸�2����Ϣ
{
    int find = 0;                      //0������ʧ�ܣ�1������ɹ�
    int i = 0, j;
    for (i = 0; i < m; i++)            //�ӵ͵�ַ��ʼ��
    {
        if (free_table[i].length < len)//С��
        {//��������һ��
        }
        else if (free_table[i].length == len)//����
        {
            switch (type)
            {
            case 0:
                cout << "��ַ�ɵ͵��ߣ��ҵ��׸���С����ҵ�Ŀռ䣬���СΪ�� " << free_table[i].length << "��������ҵ��С�����÷ָ�" << endl;
                break;
            case 1:
                cout << "�ҵ���С�ġ����ܹ����ɴ�������ҵ�Ŀ��з��������СΪ�� " << free_table[i].length << "��������ҵ��С�����÷ָ�" << endl;
                break;
            case 2:
                cout << "�ҵ��ڿ��������в������Ŀ��з��������СΪ�� " << free_table[i].length << "��������ҵ��С�����÷ָ�" << endl;
                break;
            }
           
            //�ѷ�����
            for (j = 0; j < n; j++)
            {
                if (used_table[j].flag == 0)//�ձ�Ŀ
                {
                    //�ѷ����� ����ַ ��С ��־ 
                    used_table[j].address = free_table[i].address;//����ַ
                    cout << "�ѷ���ĵ�ַ�� " << used_table[j].address << endl;
                    used_table[j].length = len;                    //�Ĵ�С
                    used_table[j].flag = name;                     //�ı�־
                    // ������ҵ�����е���ʼ��ַ
                    for (int k = 0; k < job_count; k++) {
                        if (job[k].name == name) {
                            job[k].address = used_table[j].address;
                            break;
                        }
                    }
                    break;
                }
            }
            //������ 
            for (j = i; j <= m - 2; j++)
                free_table[j] = free_table[j + 1]; //��λ��
            free_table[m - 1].flag = 0;            //�ı�־
            find = 1;                              //�������
            break;
        }
        else //����
        {
            switch (type)
            {
            case 0:
                cout << "��ַ�ɵ͵��ߣ��ҵ��׸���С����ҵ�Ŀռ䣬���СΪ�� " << free_table[i].length << "��������ҵ��Ҫ�ָ�" << endl;
                break;
            case 1:
                cout << "�ҵ���С�ġ����ܹ����ɴ�������ҵ�Ŀ��з��������СΪ�� " << free_table[i].length << "��������ҵ��Ҫ�ָ�" << endl;
                break;
            case 2:
                cout << "�ҵ��ڿ��������в������Ŀ��з��������СΪ�� " << free_table[i].length << "��������ҵ��Ҫ�ָ�" << endl;
                break;
            default:
                break;
            }
            
            //�ѷ�����
            for (j = 0; j < n; j++)
            {
                if (used_table[j].flag == 0)        //�ձ�Ŀ
                {
                    //�ѷ����� ����ַ ��С ��־ 
                    used_table[j].address = free_table[i].address;//����ַ
                    used_table[j].length = len;                    //�Ĵ�С
                    used_table[j].flag = name;                     //�ı�־
                    // ������ҵ�����е���ʼ��ַ
                    for (int k = 0; k < job_count; k++) {
                        if (job[k].name == name) {
                            job[k].address = used_table[j].address;
                            break;
                        }
                    }
                    break;
                }
            }
            //������
            free_table[i].address += len; //�ĵ�ַ
            free_table[i].length -= len;  //�Ĵ�С
            find = 1;                     //�������
            break;
        }
    }
    return find;
}

void Reclaim(char name) {          // ���պϲ����޸�2����Ϣ
    int i, j, key = -1;            // �жϿ������Ƿ��пձ�Ŀ
    int key2 = -1;                 // û�пձ�ʱ���´�����ҵ��Ϣ���Ǹ��ѷ�����

    // �����ѷ������ҵ��Ϣ���޸Ŀ�������Ϣ
    for (i = 0; i < n; i++) {
        if (used_table[i].flag == name) {  // �ҵ�����ҵ��Ϣ
            key2 = i;
            // �޸Ŀ�������Ϣ
            for (j = 0; j < m; j++) {
                // �ҿձ�Ŀ �޸���ַ ��С ��־ ���� Ȼ��ϲ� һ����һ���ϲ�
                if (free_table[j].flag == 0) {
                    free_table[j].address = used_table[i].address;  // ����ַ
                    free_table[j].length = used_table[i].length;    // �Ĵ�С
                    free_table[j].flag = 1;                         // �ı�־
                    key = j;                                        // ��¼����Ŀλ��
                    break;                                          // ֻҪ�ҵ��׸��ձ�Ŀ
                }
            }
            // �޸��ѷ�������Ϣ
            used_table[i].flag = 0;  // �ձ�Ŀ
            break;
        }
    }

    // �����������ȷ������ַ����
    for (j = 0; j < m - 1; j++) {
        if (free_table[j].flag && free_table[j + 1].flag) {  // ��������Ƚ�
            if (free_table[j].address > free_table[j + 1].address) {
                swap(free_table[j], free_table[j + 1]);
            }
        }
        else {
            break;  // ���ձ�Ŀ�������
        }
    }

    // �ϲ����ڿ�����
    if (key != -1) {
        cout << "���տ��Ѿ����������" << endl;
        for (i = 0; i < m; i++) {  // ���Ͽ�����
            if (free_table[i].flag == 1) {  // ����
                if (free_table[i].address + free_table[i].length == free_table[key].address) {
                    cout << "�ҵ����ڿ�����������ʼ��ַΪ�� "<<free_table[i].address<<"���СΪ�� "<< free_table[i].length <<"���ںϲ�" << endl;
                    // ֻҪ�ĳ��� ��������1 Ǩ��
                    free_table[i].length += free_table[key].length;
                    for (j = key; j < m - 1; j++)  // ��������1
                        free_table[j] = free_table[j + 1];
                    free_table[m - 1].flag = 0;  // �ձ�Ŀ
                    key = i;                     // ���յĿ��
                    break;                       // �ϲ����
                }
            }
        }
    }
    else {  // û�п���Ŀ
        cout << "��ʱ������û�п���Ŀ��ֻ��ֱ�Ӻϲ�" << endl;
        for (i = 0; i < m; i++) {  // ���Ͽ�����
            if (free_table[i].flag == 1) {  // ����
                if (free_table[i].address + free_table[i].length == used_table[key2].address) {
                    cout << "�ҵ����ڿ�����������ʼ��ַΪ�� " << free_table[i].address << "���СΪ�� " << free_table[i].length << "���ںϲ�" << endl;
                    // ֻ�ĳ��� �������Ŀ�����
                    free_table[i].length += used_table[key2].length;
                    key = i;
                    break;
                }
            }
        }
    }

    // �ϲ����ڿ�����
    if (key != -1) {  // �Ѵ���
        cout << "���տ��Ѿ����������" << endl;
        for (i = 0; i < m; i++) {  // ���¿�����
            if (free_table[i].flag == 1) {  // ����
                if (free_table[key].address + free_table[key].length == free_table[i].address) {
                    cout << "�ҵ����ڿ�����������ʼ��ַΪ�� " << free_table[i].address << "���СΪ�� " << free_table[i].length << "���ںϲ�" << endl;
                    // �ϲ�ʱ������ʼ��ַ����
                    free_table[key].length += free_table[i].length;
                    for (j = i; j < m - 1; j++)  // ��������1
                        free_table[j] = free_table[j + 1];
                    free_table[m - 1].flag = 0;  // �ձ�Ŀ
                    break;                       // �ϲ����
                }
            }
        }
    }
    else {
        for (i = 0; i < m; i++) {  // ���¿�����
            if (free_table[i].flag == 1) {  // ����
                if (used_table[key2].address + used_table[key2].length == free_table[i].address) {
                    cout << "�ҵ����ڿ�����������ʼ��ַΪ�� " << free_table[i].address << "���СΪ�� " << free_table[i].length << "���ںϲ�" << endl;
                    // �ϲ�ʱ��Ҫ������ʼ��ַΪ��ҵ�����ʼ��ַ
                    free_table[i].address = used_table[key2].address;
                    free_table[i].length += used_table[key2].length;
                    key = i;
                    break;  // �ϲ����
                }
            }
        }
    }

    // �ٴ������������ȷ������ַ����
    for (j = 0; j < m - 1; j++) {
        if (free_table[j].flag && free_table[j + 1].flag) {  // ��������Ƚ�
            if (free_table[j].address > free_table[j + 1].address) {
                swap(free_table[j], free_table[j + 1]);
            }
        }
        else {
            break;  // ���ձ�Ŀ�������
        }
    }
}





