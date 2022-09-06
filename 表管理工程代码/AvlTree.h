#include <iostream>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<algorithm>
#include<math.h>
#include <vector>
using namespace std;
#ifndef AVLTREE_H_
#define AVLTREE_H_


class Node {
public:
    int64_t data;  //数据，64位
    int height;   //节点的高度
    int line;    //节点数据在表中第几行
    Node* left;   //左子树根节点
    Node* right;  //右子树根节点
};

class AvlTree {
public:
    Node* root;  //树的根节点
    int height;  //树的高度，大小为根节点高度
    
    int GetHeight(Node* r) {   //获取树的高度
        if (!r) return 0;
        return r->height;
    }

    int GetFactor(Node* r) {   //得到某节点的平衡因子
        if (!r)
           return 0;
        return GetHeight(r->left) - GetHeight(r->right);
    }

    void InsertNode(Node*& r, int64_t data,int line) { //line为数据所在行数
        //插入新节点
        if (r == NULL) { //节点赋值
            r = new Node;
            r->data = data;
            r->line = line;
            r->left = NULL;
            r->right = NULL;
        }
        else if (data <= r->data)
        {
            InsertNode(r->left, data, line);
        }
        else if (data > r->data)
        {
            InsertNode(r->right, data, line);
        }
        else {
        }
        int factor = GetFactor(r);  //得到平衡因子
        //根据平衡因子执行对应旋转操作
        if (factor > 1 && GetFactor(r->left) > 0)
            r = LL(r);
        else if (factor > 1 && GetFactor(r->left) < 0)
            r = LR(r);
        else if (factor < -1 && GetFactor(r->right) > 0)
            r = RL(r);
        else if (factor < -1 && GetFactor(r->right) < 0) 
            r = RR(r);
        r->height = max(GetHeight(r->left), GetHeight(r->right)) + 1; //更新树的高度
    }
    //左左插入，单旋转
    Node* LL(Node*& r) {
        Node* q = r->left;
        r->left = q->right;
        q->right = r;
        r = q;
        r->right->height = max(GetHeight(r->right->left), GetHeight(r->right->right)) + 1;
        return q;
    }
    //右右插入，单旋转
    Node* RR(Node*& r)
    {
        Node* q = r->right;
        r->right = q->left;
        q->left = r;
        r = q;
        r->left->height = max(GetHeight(r->left->left), GetHeight(r->left->right)) + 1;
        return q;
    }
    //双旋转的两种情况，通过两次单旋转实现
    //对t的左结点进行RR旋转，再对根节点进行LL旋转
    Node* LR(Node*& r)
    {
        RR(r->left);
        return LL(r);
    }
    //对t的左结点进行LL旋转，再对根节点进行RR旋转
    Node* RL(Node*& r)
    {
        LL(r->right);
        return RR(r);
    }
    //中序遍历平衡二叉树，结果应为一个递增序列
    void InOrderTraversal(Node* n) {
        cout << "此二叉树中序遍历结果为: "<<endl;
        IOTFuction(n);
    }
    //具体实现
    void IOTFuction(Node* n) {
        if (n != NULL) {
            IOTFuction(n->left);
            cout << n->data << "  " << n->height << endl;
            IOTFuction(n->right);
        }
    }

    //精确查找某个节点
    void FindNode(Node* n, int64_t aim) {
        while (n != NULL) {
            if (n->data == aim) {
                cout << "找到数据，树中高度为" << n->height << ",为表中第" << n->line << "行数据。" << endl;
                return;
            }
            else if (n->data > aim) {
                n = n->left;
            }
            else if (n->data < aim) {
                n = n->right;
            }
        }
        cout<<"没有对应数据"<<endl;
    }
    //范围查找某些节点，设置上下限
    void FindRange(Node* n, int64_t min, int64_t max) {
        if (n == NULL) {
            return;
        }
        else if(n->data < min){
            FindRange(n->right, min, max);
        }
        else if (n->data > max) {
            FindRange(n->left, min, max);       
        }
        else if (min <= n->data && n->data <= max) {
            cout << "找到符合数据，对应属性值为" << n->data << ",树中高度为" << n->height << ",位于表格中第" << n->line << "行。" << endl;
            FindRange(n->right, min, max);
            FindRange(n->left, min, max);
        }
    }

    //将二叉树结构存入索引文件，方便利用索引文件查询
     void SaveIndex(int file) {
        int64_t h = root->height;
        Node* n;
        vector<Node*> stack; //用vector模拟栈和队列的效果，该功能中为队列
         vector<int64_t> v;
        write(file,&h,sizeof(int64_t));
        v.push_back(h);
        stack.push_back(root);
        for (int i = 0;i < h;i++) {  //高度决定次数
            for (int j = 0;j<pow(2,i);j++) {  //写入数据数目依次为1，2，4，8...
               // cout << "1  ";
                n = stack.front();
                if (n != NULL) {      //节点存在，则写入节点的data          
                    write(file,&n->data,sizeof(int64_t));
                    v.push_back(n->data);
                    stack.push_back(n->left);
                    stack.push_back(n->right);
                }
                else {   //节点为空，写入0，代表空节点
                    int64_t i = 0;
                    write(file,&i, sizeof(int64_t));
                    v.push_back(i);
                    stack.push_back(NULL);
                    stack.push_back(NULL);
                }
                stack.erase(stack.begin(), stack.begin()+1); //队列弹出数据
            }           
        } 
    }
};

#endif