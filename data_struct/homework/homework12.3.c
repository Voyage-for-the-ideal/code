#include<stdio.h>
#include<stdlib.h>

#define ERROR -1

//9.26 将折半查找的算法改写成递归算法。
//假定序列从小到大存放整数，最终返回x在表中的位置
int find(int index[],int x,int low,int high){//low初始为0，high初始为n-1
    static int bull=0;
    if(low>high){ 
        bull=-1;
        return 0;
        }
    else if(index[(low+high)/2]<x)find(index,x,((low+high)/2)+1,high);
    else if(index[(low+high)/2]>x)find(index,x,low,((low+high)/2)-1);
    else if(index[(low+high)/2]=x){
        bull=(low+high)/2;
        return 0;
        }
    return bull;
}

//定义二叉链表
typedef struct TreeNode {
    int data; // 关键字(默认为int类型)
    struct TreeNode *left; // 指向左子节点的指针
    struct TreeNode *right; // 指向右子节点的指针
} TreeNode;

//9.31写一个判别给定二叉树是否为二叉排序树的算法，设此二叉树以二叉链表作储存结构。且树中结点的关键字均不同。
int tell_if_order(TreeNode *root){
    static int bull=1;
    if(root!=NULL){
        if(root->left->data>root->data||root->right->data<root->data) {
            bull=-1;//一旦有一个不符合排序树的条件，就将布尔值调为-1
            return ERROR;
        }
        else {
            tell_if_order(root->left);
            tell_if_order(root->right);
        }
    }
    return bull;
}

//9.33编写递归算法，从大到小输出给定二叉排序树中所有关键字不小于x的数据元素。
void print_num(TreeNode *root,int x){//中序遍历该二叉排序树即可
    if(root!=NULL){
        printf_num(root->left,x);
        if(root->data>=x)printf("%d",root->data);
        print_num(root->right,x);
    }
}
