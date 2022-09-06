#include <iostream>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<thread>
#include<threads.h>
#include"MyAPI.h"
#include"AvlTree.h"
using namespace std;


AvlTree myTree; //索引结构平衡二叉树


void UpdataIndex(){  //更新index文件，将平衡二叉树写入文件
   int file=open("index.txt", O_WRONLY|O_TRUNC);
   myTree.SaveIndex(file);
   close(file);
}

void Initialization(){     
   //执行程序后，首先初始化，从保存索引结构的tree.txt文件中读取数据，建立平衡二叉树
   int file=open("tree.txt",O_RDONLY);
   BuildTreeFromFile(file,&myTree); //利用tree.txt建立索引结构平衡二叉树，并把其节点存入容器v
   close(file);
   UpdataIndex(); //更新index文件
}
void DeleteAll(){
   //清除所有数据，重新还原原本的测试数据状态
   int tableFile=open("table.txt",O_WRONLY|O_TRUNC); 
   int treefile=open("tree.txt",O_WRONLY|O_TRUNC);
   for(int i=0;i<10;i++){  //写入10条数据，数据固定，方便展示功能
      for(int j=1;j<=100;j++){
         int64_t data = i*100+j;
         write(tableFile,&data,sizeof(int64_t));
         if(j==1){  //第一个属性是索引属性，因此写入平衡二叉树文件中
            write(treefile,&data,sizeof(int64_t));
         }
      }
   }
   close(tableFile);
   close(treefile);
   Initialization();
   
}
void SynchronizeIndex(){  
   //WriteFile后可以调用，同步数据到mytree和tree.txt,index.txt方便再次启动程序时直接构建树
    int fd=open("table.txt",O_RDONLY); 
    int file=open("tree.txt",O_WRONLY); //只写如一条新数据，原有数据不动
    lseek(fd,-800,2);
    int64_t data;
    read(fd,&data,sizeof(int64_t));
    v.push_back(data); //放入容器中
    myTree.InsertNode(myTree.root,data,v.size()); //插入平衡二叉树
   // cout<<" "<<data<<endl;
    lseek(file,0,2);
    write(file,&data,sizeof(int64_t));
    close(fd);
    close(file);
    UpdataIndex();  //同步到索引文件
}

void *thread1(void *arg) //多线程功能测试
{
    long i=(long)arg;
    cout << "in thread, tid = " << pthread_self() << endl;
    if(i==1){     //功能2：写入一条数据
         int file = open("table.txt",O_WRONLY);          
         WriteFile(file);          
         close(file);
         SynchronizeIndex();
    }
    else if(i==2){ //功能5：用二叉树查找
         int min,max;
         cout<<"请输入查找上下限，若相同则为精确查找，否则为范围查找："<<endl;
         cout<<"请输入下限："<<endl;
         cin>>min;
         cout<<"请输入上限："<<endl;
         cin>>max;
         if(min==max){
             myTree.FindNode(myTree.root,min);
         }
         else{
            myTree.FindRange(myTree.root,min,max);
         }
    }
    else if(i==3){ //功能7：用索引文件查找
         int file=open("index.txt",O_RDONLY); 
         int min,max;
         cout<<"请输入查找上下限，若相同则为精确查找，否则为范围查找："<<endl;
         cout<<"请输入下限："<<endl;
         cin>>min;
         cout<<"请输入上限："<<endl;
         cin>>max;
         if(min==max){
             FindByIndex(file,min);
         }
         else{
            FindRangeByIndex(file,min,max);
         }       
    }
    return (void *)0;
}

int main(){
   DeleteAll();

   cout<<"请输入想执行的功能："<<endl;
   cout<<"1:查看现在表格中数据个数，以及对应的索引属性值。"<<endl;
   cout<<"2:向表格中插入一条数据，数据自动生成。"<<endl;
   cout<<"3:查找：在表格中直接查找。"<<endl;
   cout<<"4:中序输出目前平衡二叉树的所有节点。"<<endl;
   cout<<"5:查找：用平衡二叉树查找。"<<endl;
   cout<<"6:查看目前索引文件的内容。"<<endl;
   cout<<"7:查找：用索引文件查找。"<<endl;
   cout<<"8:多线程测试：生成一个向表中插入一条数据（功能2）的线程。"<<endl;
   int instruction;
   cin>>instruction;
   while(instruction!=0){
      switch (instruction)
      {
      case 1:
         ShowAll();
         break;
      case 2:
         {
            int file = open("table.txt",O_WRONLY);          
            WriteFile(file);          
            close(file);
            SynchronizeIndex();
            break;
         }
      case 3:
         {
            int num,min,max;
            cout<<"请输入对应的属性号码："<<endl;
            cin>>num;
            cout<<"请输入查找上下限，若相同则为精确查找，否则为范围查找："<<endl;
            cout<<"请输入下限："<<endl;
            cin>>min;
            cout<<"请输入上限："<<endl;
            cin>>max;
            int file=open("table.txt",O_RDONLY);           
            FindData(file,num,min,max);          
            close(file);
            break;
         }      
      case 4:
         myTree.InOrderTraversal(myTree.root);         
         break;
      case 5:
      {
         int min,max;
         cout<<"请输入查找上下限，若相同则为精确查找，否则为范围查找："<<endl;
         cout<<"请输入下限："<<endl;
         cin>>min;
         cout<<"请输入上限："<<endl;
         cin>>max;
         if(min==max){
             myTree.FindNode(myTree.root,min);
         }
         else{
            myTree.FindRange(myTree.root,min,max);
         }
         break;
      }
      case 6:
      {
         int file=open("index.txt",O_RDONLY); 
         ShowIndexFile(file);
         break;
      }
      case 7:
      {
         int file=open("index.txt",O_RDONLY); 
         int min,max;
         cout<<"请输入查找上下限，若相同则为精确查找，否则为范围查找："<<endl;
         cout<<"请输入下限："<<endl;
         cin>>min;
         cout<<"请输入上限："<<endl;
         cin>>max;
         if(min==max){
             FindByIndex(file,min);
         }
         else{
            FindRangeByIndex(file,min,max);
         }
         break;
      }
      case 8:
      {
         long i;
         cin>>i;
         pthread_t tid;
         if(pthread_create(&tid,NULL,thread1,(void*)i)!=0){
            cout<<"create failed";
         }
         else{
            pthread_join(tid,0);
            cout<<"thread test succeed"<<endl;        
         }
         break;
      }
      }
      cin>>instruction;
   }
   return 0;
}