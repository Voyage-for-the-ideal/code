#include<stdio.h>
#include<stdlib.h>

#define EXIT_FAILURE -1
#define ERROR -1
#define true 1
#define false -1
#define MAXSIZE 100
#define OK 1

//GBK

typedef struct TreeNode {
    char data; // �ڵ�洢�����ݣ��������Ϊ�ַ�����
    struct TreeNode *firstChild; // ָ���һ���ӽڵ��ָ��
    struct TreeNode *nextSibling; // ָ����һ���ֵܽڵ��ָ��
} TreeNode;

// ����һ���µ����ڵ�
TreeNode* createTreeNode(char data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

void insertChild(TreeNode* parent, TreeNode* child) {
    if (parent == NULL) {
        return;
    }
    // ���parentû���ӽڵ㣬child��Ϊ��һ���ӽڵ�
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        // �����ҵ����ұߵ��ֵܽڵ㣬�����������½ڵ�
        TreeNode* current = parent->firstChild;
        while (current->nextSibling != NULL) {
            current = current->nextSibling;
        }
        current->nextSibling = child;
    }
}

// �����ֵܽڵ�
void insertSibling(TreeNode* prev, TreeNode* next) {
    if (prev == NULL || next == NULL) {
        return;
    }
    prev->nextSibling = next;
}

typedef struct Queue{//����6.68����������Ķ���
    TreeNode *tree;
    struct Queue *next;
}QueueNode;

typedef struct{
    QueueNode *front;
    QueueNode *rear;
}Queue;

typedef struct Sqlist{
    int elem[MAXSIZE];
    int length;
    }Sqlist;

void initQueue(Queue *queue) {
    queue->front = queue->rear = NULL;
}

int isQueueEmpty(Queue *q) {
    return q->front == NULL;
}


void enqueue(Queue *q, TreeNode *node) {
    QueueNode *newNode = createQueueNode(node);
    if (isQueueEmpty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

TreeNode* dequeue(Queue *q) {
    if (isQueueEmpty(q)) {
        return NULL;
    }
    QueueNode *temp = q->front;
    TreeNode *node = temp->tree;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return node;
}

//6.68��֪һ�������ɸ���Ҷ�ӽ�㰴�������Ľ�����м�ÿ�����Ķȣ�ÿ���������������룩����д����������ĺ���-�ֵ�������㷨
typedef struct List{//�����������������
    int number;
    char data;
    List *next;
}List;

TreeNode* createtree(List *list){//��ʼiΪ0
    if(list==NULL)exit;
    TreeNode *root=createTreeNode(list->data);//�ȴ������ڵ�
    TreeNode *parent;
    Queue *queue=(Queue *)malloc(sizeof(Queue));//�������ڴ�Ž��
    initQueue(queue);
    int number=list->number;
    int i;
    char data;
    List *current=list->next;
    List *current1;
    TreeNode *current2;

    enqueue(queue,root);
    while(current!=NULL){
        parent=dequeue(queue);
        data=parent->data;
        current1=list;
        while(current1!=NULL&&current1->data!=data){
            current1=current1->next;
        }
        i=current1->number;
        for(i=0;i<number;i++){
            current2=createTreeNode(current->data);
            insertChild(parent,current2);
            enqueue(queue,current2);
            current=current->next;
        }//����Ҫ����һ��current->next����Ϊ�����forѭ��������
    }    
    return root;
}

//6.69�����Զ���������Ķ������У�ÿ�������������Ԫ�ؾ�Ϊ����ĸ���Ա�д�㷨������״��ӡ���������㷨�������ӻ���ʾ��

typedef struct BiTNode{
    char data;
    struct BiNode *lchild,*rchild;
}BiNode,*BiTree;
void printBitree_1(BiNode *root,int layer){//�������layerΪ1����ʾ���ڵ��ڵ�һ��
    //������->��->���˳������������������ʱ���Զ����У������Ϊ�����
    if(root==NULL)return OK;
    int i=0;
    printBitree(root->rchild,layer+1);
    for(i=0;i<layer-1;i++){
        printf(" ");
    }
    printf("%c\n",root->data);
    printfBitree(root->lchild,layer+1);
}

//6.71��������ÿ���������������Ԫ��Ϊһ����ĸ�������Ժ���-�ֵ�����Ϊ���Ĵ���ṹ����дһ��������귽ʽ��ӡһ�������㷨�������ӻ���һ�ֱ�ʾ��
void printBitree_2(TreeNode *root,int layer){//��ʼlayerΪ1
    //���մ������£��������ҵ�˳���������ĸ�������Ϊ����
    if(root==NULL)return OK;
    int i=0;
    for(i=0;i<layer-1;i++){
        printf(" ");
    }
    printf("%c\n",root->data);
    if(root->firstChild!=NULL)printBitree_2(root->firstChild,layer+1);//�к��Ӿʹ�ӡ����
    else if(root->firstChild==NULL&&root->nextSibling!=NULL)printBitree_2(root->nextSibling,layer);//û�������ֵܾʹ�ӡ�ֵ�
}