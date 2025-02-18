#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

#define n 10                        //假定系统允许的最大作业数量为n
#define m 10                        //假定系统允许的空闲区表最大为m

struct                              //已分配区表的定义：
{
    float address;                  //已分分区起始地址
    float length;                   //已分分区长度，单位为字节
    int flag;                       //已分配表区登记栏标志，用0表示空栏目
} used_table[n];                    //已分配区表 

struct                              //空闲区表的定义：
{
    float address;                  //空闲区起始地址
    float length;                   //空闲区长度，单位为字节
    int flag;                       //空闲区表登记栏目用0表示空栏目，1表示未分配
} free_table[m];                    //空闲区表

struct                              //作业队列的定义：
{
    char name;                      //作业名
    float length;                   //作业大小
    int flag;                       //0未分配,1已分配,2已完成
    float address;                  //作业的起始地址
} job[10];

//全局变量
int L = 1024;                       //总内存
int job_count;                      //用户输入的作业数量

//自定义函数
void First_fit();                   //最先适应算法
void Best_fit();                    //最优适应算法
void Worst_fit();                   //最坏适应算法
void Init_table();                  //初始化已分配区表和空闲表
void Init_job(int);                 //初始化作业队列
int Distribute(char, float,int);        //分配内存，考虑切割，修改已分配区表和空闲表信息
void Reclaim(char);                 //回收内存，考虑合并，修改已分配区表和空闲表信息
void Display();                     //显示空闲区表与已分配区表信息
void Display_job();                 //显示作业队列信息

//主函数
int main()
{
    srand(time(0));
    cout << "请输入需要分配内存的作业数量（1-10）: ";
    cin >> job_count;
    if (job_count < 1 || job_count > 10) {
        cout << "输入的作业数量无效，程序结束。" << endl;
        return 1;
    }

    int choice;
    cout << "请选择内存分配算法：1-最先适应 2-最优适应 3-最坏适应: "<<endl;
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
        cout << "输入无效，程序结束。" << endl;
        return 1;
    }
    return 0;
}

void First_fit()                    //最先适应算法                
{
    Init_table();                   //初始化两表
    Init_job(job_count);            //初始化作业队列
    Display();                      //输出两表
    Display_job();                  //输出作业队列
    cout << endl;
    int i, num = job_count;         //记录未分配作业数目

    while (num > 0)                 //一轮分配一轮回收，直到作业分配完
    {
        cout << "开始一轮分配" << endl << endl;
        for (i = 0; i < job_count; i++)  //第一步：对每一个未分配作业进行分配
        {
            if (job[i].flag == 0)   //找到一个未分配
            {
                cout << "找到未分配作业" << job[i].name << "大小为" << job[i].length << endl;
                if (Distribute(job[i].name, job[i].length,0) == 1)//分配切割，修改2个表和1个队列信息
                {
                    cout << "成功分配" << endl;
                    job[i].flag = 1;    //已分配
                    Display();          //输出两表
                    Display_job();      //输出作业队列
                }
                else
                {
                    cout << "该作业暂无空间分配" << endl;
                    Display();          //输出两表
                    Display_job();      //输出作业队列
                }
            }
        }

        if (num == 0)
        {
            cout << "全部作业都分配了空间，程序结束" << endl;
            break;
        }

        cout << "开始一轮回收" << endl << endl;
        int choice;
        for (i = 0; i < job_count; i++)  //第二步：对每一个已分配作业随机回收
        {
            if (job[i].flag == 1)    //已分配
            {
                choice = rand() % 2; // 随机数范围为0或1
                cout << "产生的随机数为" << choice << endl;
                if (choice == 1)     //随机
                {
                    cout << "回收作业" << job[i].name << endl;
                    Reclaim(job[i].name); //回收合并（修改2表信息）回收不可能失败
                    job[i].flag = 2;  //已完成
                    cout << "作业已经完成" << endl;
                    Display();        //输出两表
                    Display_job();    //输出作业队列
                }
                else
                {
                    cout << "暂不回收作业" << job[i].name << endl;
                    Display();        //输出两表
                    Display_job();    //输出作业队列        
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

void Best_fit()                    //最优适应算法                
{
    Init_table();                   //初始化两表
    Init_job(job_count);            //初始化作业队列
    Display();                      //输出两表
    Display_job();                  //输出作业队列
    cout << endl;
    int i, num = job_count;         //记录未分配作业数目

    while (num > 0)                 //一轮分配一轮回收，直到作业分配完
    {
        cout << "开始一轮分配" << endl << endl;
        sort(free_table, free_table + m, [](auto& a, auto& b) { return a.length < b.length; }); // 按长度升序排序
        for (i = 0; i < job_count; i++)  //第一步：对每一个未分配作业进行分配
        {
            if (job[i].flag == 0)   //找到一个未分配
            {
                cout << "找到未分配作业" << job[i].name << "大小为" << job[i].length << endl;
                if (Distribute(job[i].name, job[i].length,1) == 1)//分配切割，修改2个表和1个队列信息
                {
                    cout << "成功分配" << endl;
                    job[i].flag = 1;    //已分配
                    Display();          //输出两表
                    Display_job();      //输出作业队列
                }
                else
                {
                    cout << "该作业暂无空间分配" << endl;
                    Display();          //输出两表
                    Display_job();      //输出作业队列
                }
            }
        }

        if (num == 0)
        {
            cout << "全部作业都分配了空间，程序结束" << endl;
            break;
        }

        cout << "开始一轮回收" << endl << endl;
        int choice;
        for (i = 0; i < job_count; i++)  //第二步：对每一个已分配作业随机回收
        {
            if (job[i].flag == 1)    //已分配
            {
                choice = rand() % 2; // 随机数范围为0或1
                cout << "产生的随机数为" << choice << endl;
                if (choice == 1)     //随机
                {
                    cout << "回收作业" << job[i].name << endl;
                    Reclaim(job[i].name); //回收合并（修改2表信息）回收不可能失败
                    job[i].flag = 2;  //已完成
                    cout << "作业已经完成" << endl;
                    Display();        //输出两表
                    Display_job();    //输出作业队列
                }
                else
                {
                    cout << "暂不回收作业" << job[i].name << endl;
                    Display();        //输出两表
                    Display_job();    //输出作业队列        
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

void Worst_fit()                    //最坏适应算法                
{
    Init_table();                   //初始化两表
    Init_job(job_count);            //初始化作业队列
    Display();                      //输出两表
    Display_job();                  //输出作业队列
    cout << endl;
    int i, num = job_count;         //记录未分配作业数目

    while (num > 0)                 //一轮分配一轮回收，直到作业分配完
    {
        cout << "开始一轮分配" << endl << endl;
        sort(free_table, free_table + m, [](auto& a, auto& b) { return a.length > b.length; }); // 按长度降序排序
        for (i = 0; i < job_count; i++)  //第一步：对每一个未分配作业进行分配
        {
            if (job[i].flag == 0)   //找到一个未分配
            {
                cout << "找到未分配作业" << job[i].name << "大小为" << job[i].length << endl;
                if (Distribute(job[i].name, job[i].length,2) == 1)//分配切割，修改2个表和1个队列信息
                {
                    cout << "成功分配" << endl;
                    job[i].flag = 1;    //已分配
                    Display();          //输出两表
                    Display_job();      //输出作业队列
                }
                else
                {
                    cout << "该作业暂无空间分配" << endl;
                    Display();          //输出两表
                    Display_job();      //输出作业队列
                }
            }
        }

        if (num == 0)
        {
            cout << "全部作业都分配了空间，程序结束" << endl;
            break;
        }

        cout << "开始一轮回收" << endl << endl;
        int choice;
        for (i = 0; i < job_count; i++)  //第二步：对每一个已分配作业随机回收
        {
            if (job[i].flag == 1)    //已分配
            {
                choice = rand() % 2; // 随机数范围为0或1
                cout << "产生的随机数为" << choice << endl;
                if (choice == 1)     //随机
                {
                    cout << "回收作业" << job[i].name << endl;
                    Reclaim(job[i].name); //回收合并（修改2表信息）回收不可能失败
                    job[i].flag = 2;  //已完成
                    cout << "作业已经完成" << endl;
                    Display();        //输出两表
                    Display_job();    //输出作业队列
                }
                else
                {
                    cout << "暂不回收作业" << job[i].name << endl;
                    Display();        //输出两表
                    Display_job();    //输出作业队列        
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

void Init_table()                            //初始化两表            
{
    int i;
    for (i = 0; i < n; i++)
        used_table[i].flag = 0;              //已分配区：0空表格目，非0(作业名)已分配

    free_table[0].address = 0;
    free_table[0].length = 2;
    free_table[0].flag = 1;

    free_table[1].address = free_table[0].address + free_table[0].length;
    free_table[1].length = 2;
    free_table[1].flag = 1;

    for (i = 2; i < m; i++)
    {
        free_table[i].address = free_table[i - 1].address + free_table[i - 1].length; //起址
        free_table[i].length = free_table[i - 1].length * 2;                         //大小
        free_table[i].flag = 1;                                                      //状态
    }
    cout << "空闲区与已分配区初始化完成" << endl;
}

void Init_job(int job_count)                   //初始化作业
{
    for (int i = 0; i < job_count; i++)
    {
        job[i].name = 'A' + i;                 //作业名：A~J
        job[i].length = rand() % (L / 4) + 1;  //大小：在1~L/4
        job[i].flag = 0;                       //状态：0未分配，1已分配，2已结束
        job[i].address = -1;                   // 初始地址为 -1 表示未分配
    }
    cout << "作业循环队列初始化完成" << endl;
}

void Display()                                                                    //输出两表
{
    cout << endl << "================================" << endl;
    cout << "空闲区表(起址由低向高排序)：" << endl;
    cout << "序号" << "\t起址" << "\t大小" << "\t标志" << endl;;
    for (int i = 0; i < m; i++)
    {
        if (free_table[i].flag == 1)                                                //2种状态
            cout << i + 1 << "\t" << free_table[i].address << "\t" << free_table[i].length << "\t空闲" << endl;
        else
            cout << i + 1 << "\t--\t--\t空栏目" << endl;
    }
    cout << "================================" << endl;
    cout << endl << "================================" << endl;
    cout << "已分配区表(不用排序)" << endl;
    cout << "序号" << "\t起址" << "\t大小" << "\t标志" << endl;;
    for (int j = 0; j < n; j++)
    {
        if (used_table[j].flag == 0)                                                //2种状态
            cout << j + 1 << "\t--\t--\t空栏目" << endl;
        else
            cout << j + 1 << "\t" << used_table[j].address << "\t" << used_table[j].length << "\t" << "已分配" << endl;
    }
    cout << "================================" << endl;
}

void Display_job()                                                                //输出作业队列
{
    cout << endl << "=========================" << endl;
    cout << "作业队列：" << endl;
    cout << "作业名" << "\t大小" << "\t标志" << "\t起始地址" << endl;
    for (int i = 0; i < job_count; i++)
    {
        cout << job[i].name << "\t" << job[i].length;
        if (job[i].flag == 0)                                                        //3种状态
            cout << "\t未分配\t" << "--" << endl;
        else if (job[i].flag == 1)
            cout << "\t已分配\t" << job[i].address << endl;
        else
            cout << "\t已结束\t" << job[i].address << endl;
    }
    cout << "=========================" << endl << endl;
}

int Distribute(char name, float len,int type)    //分配切割，修改2表信息
{
    int find = 0;                      //0：分配失败，1：分配成功
    int i = 0, j;
    for (i = 0; i < m; i++)            //从低地址开始找
    {
        if (free_table[i].length < len)//小于
        {//继续找下一个
        }
        else if (free_table[i].length == len)//等于
        {
            switch (type)
            {
            case 0:
                cout << "地址由低到高，找到首个不小于作业的空间，其大小为： " << free_table[i].length << "，等于作业大小，不用分割" << endl;
                break;
            case 1:
                cout << "找到最小的、但能够容纳待分配作业的空闲分区，其大小为： " << free_table[i].length << "，等于作业大小，不用分割" << endl;
                break;
            case 2:
                cout << "找到在空闲区表中查找最大的空闲分区，其大小为： " << free_table[i].length << "，等于作业大小，不用分割" << endl;
                break;
            }
           
            //已分配区
            for (j = 0; j < n; j++)
            {
                if (used_table[j].flag == 0)//空表目
                {
                    //已分配区 改起址 大小 标志 
                    used_table[j].address = free_table[i].address;//改起址
                    cout << "已分配的地址： " << used_table[j].address << endl;
                    used_table[j].length = len;                    //改大小
                    used_table[j].flag = name;                     //改标志
                    // 更新作业队列中的起始地址
                    for (int k = 0; k < job_count; k++) {
                        if (job[k].name == name) {
                            job[k].address = used_table[j].address;
                            break;
                        }
                    }
                    break;
                }
            }
            //空闲区 
            for (j = i; j <= m - 2; j++)
                free_table[j] = free_table[j + 1]; //改位置
            free_table[m - 1].flag = 0;            //改标志
            find = 1;                              //分配完成
            break;
        }
        else //大于
        {
            switch (type)
            {
            case 0:
                cout << "地址由低到高，找到首个不小于作业的空间，其大小为： " << free_table[i].length << "，大于作业，要分割" << endl;
                break;
            case 1:
                cout << "找到最小的、但能够容纳待分配作业的空闲分区，其大小为： " << free_table[i].length << "，大于作业，要分割" << endl;
                break;
            case 2:
                cout << "找到在空闲区表中查找最大的空闲分区，其大小为： " << free_table[i].length << "，大于作业，要分割" << endl;
                break;
            default:
                break;
            }
            
            //已分配区
            for (j = 0; j < n; j++)
            {
                if (used_table[j].flag == 0)        //空表目
                {
                    //已分配区 改起址 大小 标志 
                    used_table[j].address = free_table[i].address;//改起址
                    used_table[j].length = len;                    //改大小
                    used_table[j].flag = name;                     //改标志
                    // 更新作业队列中的起始地址
                    for (int k = 0; k < job_count; k++) {
                        if (job[k].name == name) {
                            job[k].address = used_table[j].address;
                            break;
                        }
                    }
                    break;
                }
            }
            //空闲区
            free_table[i].address += len; //改地址
            free_table[i].length -= len;  //改大小
            find = 1;                     //分配完成
            break;
        }
    }
    return find;
}

void Reclaim(char name) {          // 回收合并，修改2表信息
    int i, j, key = -1;            // 判断空闲区是否有空表目
    int key2 = -1;                 // 没有空表时记下存有作业信息的那个已分配区

    // 查找已分配的作业信息并修改空闲区信息
    for (i = 0; i < n; i++) {
        if (used_table[i].flag == name) {  // 找到了作业信息
            key2 = i;
            // 修改空闲区信息
            for (j = 0; j < m; j++) {
                // 找空表目 修改起址 大小 标志 排序 然后合并 一个接一个合并
                if (free_table[j].flag == 0) {
                    free_table[j].address = used_table[i].address;  // 改起址
                    free_table[j].length = used_table[i].length;    // 改大小
                    free_table[j].flag = 1;                         // 改标志
                    key = j;                                        // 记录该栏目位置
                    break;                                          // 只要找到首个空表目
                }
            }
            // 修改已分配区信息
            used_table[i].flag = 0;  // 空表目
            break;
        }
    }

    // 排序空闲区表，确保按地址有序
    for (j = 0; j < m - 1; j++) {
        if (free_table[j].flag && free_table[j + 1].flag) {  // 都空闲则比较
            if (free_table[j].address > free_table[j + 1].address) {
                swap(free_table[j], free_table[j + 1]);
            }
        }
        else {
            break;  // 到空表目排序结束
        }
    }

    // 合并上邻空闲区
    if (key != -1) {
        cout << "回收块已经存入空闲区" << endl;
        for (i = 0; i < m; i++) {  // 找上空闲区
            if (free_table[i].flag == 1) {  // 空闲
                if (free_table[i].address + free_table[i].length == free_table[key].address) {
                    cout << "找到上邻空闲区，其起始地址为： "<<free_table[i].address<<"其大小为： "<< free_table[i].length <<"现在合并" << endl;
                    // 只要改长度 空闲区少1 迁移
                    free_table[i].length += free_table[key].length;
                    for (j = key; j < m - 1; j++)  // 空闲区少1
                        free_table[j] = free_table[j + 1];
                    free_table[m - 1].flag = 0;  // 空表目
                    key = i;                     // 回收的块号
                    break;                       // 合并完成
                }
            }
        }
    }
    else {  // 没有空栏目
        cout << "此时空闲区没有空栏目，只能直接合并" << endl;
        for (i = 0; i < m; i++) {  // 找上空闲区
            if (free_table[i].flag == 1) {  // 空闲
                if (free_table[i].address + free_table[i].length == used_table[key2].address) {
                    cout << "找到上邻空闲区，其起始地址为： " << free_table[i].address << "其大小为： " << free_table[i].length << "现在合并" << endl;
                    // 只改长度 还是满的空闲区
                    free_table[i].length += used_table[key2].length;
                    key = i;
                    break;
                }
            }
        }
    }

    // 合并下邻空闲区
    if (key != -1) {  // 已存入
        cout << "回收块已经存入空闲区" << endl;
        for (i = 0; i < m; i++) {  // 找下空闲区
            if (free_table[i].flag == 1) {  // 空闲
                if (free_table[key].address + free_table[key].length == free_table[i].address) {
                    cout << "找到下邻空闲区，其起始地址为： " << free_table[i].address << "其大小为： " << free_table[i].length << "现在合并" << endl;
                    // 合并时保持起始地址不变
                    free_table[key].length += free_table[i].length;
                    for (j = i; j < m - 1; j++)  // 空闲区少1
                        free_table[j] = free_table[j + 1];
                    free_table[m - 1].flag = 0;  // 空表目
                    break;                       // 合并完成
                }
            }
        }
    }
    else {
        for (i = 0; i < m; i++) {  // 找下空闲区
            if (free_table[i].flag == 1) {  // 空闲
                if (used_table[key2].address + used_table[key2].length == free_table[i].address) {
                    cout << "找到下邻空闲区，其起始地址为： " << free_table[i].address << "其大小为： " << free_table[i].length << "现在合并" << endl;
                    // 合并时需要更新起始地址为作业块的起始地址
                    free_table[i].address = used_table[key2].address;
                    free_table[i].length += used_table[key2].length;
                    key = i;
                    break;  // 合并完成
                }
            }
        }
    }

    // 再次排序空闲区表，确保按地址有序
    for (j = 0; j < m - 1; j++) {
        if (free_table[j].flag && free_table[j + 1].flag) {  // 都空闲则比较
            if (free_table[j].address > free_table[j + 1].address) {
                swap(free_table[j], free_table[j + 1]);
            }
        }
        else {
            break;  // 到空表目排序结束
        }
    }
}





