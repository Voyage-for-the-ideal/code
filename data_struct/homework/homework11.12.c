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
    char data; // 节点存储的数据，这里假设为字符类型
    struct TreeNode *firstChild; // 指向第一个子节点的指针
    struct TreeNode *nextSibling; // 指向下一个兄弟节点的指针
} TreeNode;

// 创建一个新的树节点
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
    // 如果parent没有子节点，child成为第一个子节点
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        // 否则，找到最右边的兄弟节点，并在其后插入新节点
        TreeNode* current = parent->firstChild;
        while (current->nextSibling != NULL) {
            current = current->nextSibling;
        }
        current->nextSibling = child;
    }
}

// 插入兄弟节点
void insertSibling(TreeNode* prev, TreeNode* next) {
    if (prev == NULL || next == NULL) {
        return;
    }
    prev->nextSibling = next;
}

typedef struct Queue{//用于6.68，定义链表的队列
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

//6.68已知一棵树的由根至叶子结点按层次输入的结点序列及每个结点的度（每层中自左至右输入），试写出构造此树的孩子-兄弟链表的算法
typedef struct List{//假设该序列是链表储存
    int number;
    char data;
    List *next;
}List;

TreeNode* createtree(List *list){//初始i为0
    if(list==NULL)exit;
    TreeNode *root=createTreeNode(list->data);//先创建根节点
    TreeNode *parent;
    Queue *queue=(Queue *)malloc(sizeof(Queue));//队列用于存放结点
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
        }//不需要再来一个current->next，因为这个在for循环里做了
    }    
    return root;
}

//6.69假设以二叉链表储存的二叉树中，每个结点所含数据元素均为单字母，试编写算法，按树状打印二叉树的算法。（可视化表示）

typedef struct BiTNode{
    char data;
    struct BiNode *lchild,*rchild;
}BiNode,*BiTree;
void printBitree_1(BiNode *root,int layer){//最初输入layer为1，表示根节点在第一层
    //按照右->中->左的顺序遍历二叉树，输出的时候自动空行，输出列为其层数
    if(root==NULL)return OK;
    int i=0;
    printBitree(root->rchild,layer+1);
    for(i=0;i<layer-1;i++){
        printf(" ");
    }
    printf("%c\n",root->data);
    printfBitree(root->lchild,layer+1);
}

//6.71假设树上每个结点所含的数据元素为一个字母，并且以孩子-兄弟链表为树的储存结构，试写一个按凹入标方式打印一颗树的算法。（可视化另一种表示）
void printBitree_2(TreeNode *root,int layer){//初始layer为1
    //按照从上往下，从左往右的顺序遍历，字母的输出列为层数
    if(root==NULL)return OK;
    int i=0;
    for(i=0;i<layer-1;i++){
        printf(" ");
    }
    printf("%c\n",root->data);
    if(root->firstChild!=NULL)printBitree_2(root->firstChild,layer+1);//有孩子就打印孩子
    else if(root->firstChild==NULL&&root->nextSibling!=NULL)printBitree_2(root->nextSibling,layer);//没孩子有兄弟就打印兄弟
}