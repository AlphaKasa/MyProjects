#include <iostream>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <vector>
#include<algorithm>
#include<math.h>
#include"AvlTree.h"

using namespace std;

#ifndef MYAPI_H_
#define MYAPI_H_

vector<int64_t> v;
pthread_mutex_t table_mutex;
pthread_mutex_t tree_mutex;
pthread_mutex_t index_mutex;

int x; 
int num;
int number=0;
//展示表中数据索引值及行数
void ShowAll(){ 
    cout<<"目前表中共"<<v.size()<<"条数据。"<<endl;
    for(int i=0;i<v.size();i++){
        cout<<"第"<<i+1<<"条数据，对应索引值为： "<<v[i]<<endl;
    }
}
//向文件中写入一条数据
int WriteFile(int fd){
    pthread_mutex_lock(&table_mutex);
    int num;
    lseek(fd,0,2); //写入尾部
    for(int i=1;i<=100;i++){

       int64_t data=rand()%1000+1; //随机生成1-1000的数据
       if(i==1){
           num=data; //得到对应数据的索引属性
       }
       write(fd,&data,sizeof(int64_t));
       
    }
    pthread_mutex_unlock(&table_mutex);
    return num; //返回索引属性的值
}

void FindData(int fd,int number,int64_t min,int64_t max){  
    //fd为文件描述符,number为想根据几号属性检索，min为检索下限，max为检索上限
    pthread_mutex_lock(&table_mutex);
    lseek(fd,(number-1)*8,0);  //根据要查的属性确定初始位置
    int64_t data;
    for(int i=1;read(fd,&data,sizeof(int64_t))!=0;i++){
       if(data>=min){
          if(data<=max){
             cout<<"第"<<i<<"行数据对应属性符合要求，其值为： "<<data<<endl;
          }
       }
       lseek(fd,792,1); //找到下条数据对应位置
    }
    pthread_mutex_unlock(&table_mutex);
}


void SaveTreeToFile(int file) {  //把容器中保存的树的节点存入tree.txt
    pthread_mutex_lock(&tree_mutex);
    for (int i = 0;i < v.size();i++) {
        write(file, &v[i], sizeof(int64_t));
    }
    pthread_mutex_unlock(&tree_mutex);
}
void BuildTreeFromFile(int file,AvlTree* a) {  //根据tree.txt建立平衡二叉树到myTree
    pthread_mutex_lock(&tree_mutex);
    v.clear();
    a->root = NULL;
    int64_t data;
    for (int i = 0; read(file,&data, sizeof(int64_t))!=0; i++) {
        v.push_back(data);
        a->InsertNode(a->root, data, v.size());
    }
    pthread_mutex_unlock(&tree_mutex);
}

//展示索引文件结构
void ShowIndexFile(int file){
    pthread_mutex_lock(&index_mutex);
    int64_t data;
    read(file,&data,sizeof(int64_t));
    cout<<"索引文件树高为"<<data<<",共有"<<pow(2,data)-1<<"个数据，结构为："<<endl;
    for(int i=1;read(file,&data,sizeof(int64_t))!=0;i++){
        cout<<data<<" "<<endl;
    }
     pthread_mutex_unlock(&index_mutex);
}
void FindByIndex(int file,int64_t aim) {
    pthread_mutex_lock(&index_mutex);
    int64_t height;
    
    x=1;
    cout<<endl<<"寻找路径为： "<<endl;
    read(file,&height, sizeof(int64_t));
    for (int i = 0;i < height;i++) {
        read(file,&number, sizeof(int64_t));
        cout << number<<"  ";
        if (number == 0) {  //到了空节点，说明数据不存在，返回
            cout << "没有找到数据"<<endl;
            pthread_mutex_unlock(&index_mutex);
            return;
        }
        if (number == aim) {
            cout << "找到数据"<<endl;
            pthread_mutex_unlock(&index_mutex);
            return;
        }
        else if (number < aim){
            x=x*2+1;       //往右节点查询
            lseek(file, x*8, 0);
        } 
        else if (number > aim) {
            x=x*2 ;     //往左节点查询
            lseek(file, x*8 , 0);
        }
        
    }
    cout << "没有找到数据"<<endl;
    pthread_mutex_unlock(&index_mutex);
}

void FindRangeByIndex(int file, int min,int max) {
    pthread_mutex_lock(&index_mutex);
    int64_t height;
    
    vector<int> vect;  //容器模拟队列，用于存放文件偏移位数
    cout <<endl<< "找到的数据及路径如下:" << endl;
    read(file,&height, sizeof(int64_t));  //第一个数为树的高度，之后才是数据
    vect.push_back(1);  //第一个数据
    while(vect.size()!=0) {
        if (vect.front() >= pow(2, height)) { //位置已超过2的高度次方，不可能找到了，返回
            return;
        }
        lseek(file, vect.front() * 8, 0);  //定位文件对应位置
        read(file,&num, sizeof(int64_t));       
        if (num == 0) {}  //空节点    
        else{
            cout << num << "  ";
            if (num <= max && num >= min) {  //数据符合要求，其左右节点位置都需要继续查询
               cout << "找到数据"<< num << endl;
               vect.push_back(vect.front() * 2);
               vect.push_back(vect.front() * 2 + 1);
            }         
            else if (num < min) {         //否则只需要查询单个子结点   
                vect.push_back(vect.front() * 2 + 1);
            }
            else if (num > max) {
                vect.push_back(vect.front() * 2);
            }
        }
        vect.erase(vect.begin(), vect.begin() + 1);  //队列弹出
    }
    pthread_mutex_unlock(&index_mutex);
}
#endif