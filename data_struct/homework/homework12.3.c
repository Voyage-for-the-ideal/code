#include<stdio.h>
#include<stdlib.h>

#define ERROR -1

//9.26 ���۰���ҵ��㷨��д�ɵݹ��㷨��
//�ٶ����д�С���������������շ���x�ڱ��е�λ��
int find(int index[],int x,int low,int high){//low��ʼΪ0��high��ʼΪn-1
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

//�����������
typedef struct TreeNode {
    int data; // �ؼ���(Ĭ��Ϊint����)
    struct TreeNode *left; // ָ�����ӽڵ��ָ��
    struct TreeNode *right; // ָ�����ӽڵ��ָ��
} TreeNode;

//9.31дһ���б�����������Ƿ�Ϊ�������������㷨����˶������Զ�������������ṹ�������н��Ĺؼ��־���ͬ��
int tell_if_order(TreeNode *root){
    static int bull=1;
    if(root!=NULL){
        if(root->left->data>root->data||root->right->data<root->data) {
            bull=-1;//һ����һ�����������������������ͽ�����ֵ��Ϊ-1
            return ERROR;
        }
        else {
            tell_if_order(root->left);
            tell_if_order(root->right);
        }
    }
    return bull;
}

//9.33��д�ݹ��㷨���Ӵ�С����������������������йؼ��ֲ�С��x������Ԫ�ء�
void print_num(TreeNode *root,int x){//��������ö�������������
    if(root!=NULL){
        printf_num(root->left,x);
        if(root->data>=x)printf("%d",root->data);
        print_num(root->right,x);
    }
}
