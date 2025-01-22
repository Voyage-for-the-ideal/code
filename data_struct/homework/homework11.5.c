#include <stdio.h>
#include<stdlib.h>

#define ERROR -1
#define MAXSIZE 100

typedef struct BiTNode{
    int data;//默认数据为int类型（可修改）
    struct BiNode *lchild,*rchild;
}BiNode,*BiTree;

typedef struct Queue{//用于6.47，定义树的队列
    BiNode *tree;
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


void enqueue(Queue *q, BiNode *node) {
    QueueNode *newNode = createQueueNode(node);
    if (isQueueEmpty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

BiNode* dequeue(Queue *q) {
    if (isQueueEmpty(q)) {
        return NULL;
    }
    QueueNode *temp = q->front;
    BiNode *node = temp->tree;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return node;
}


//6.47
int layer_tree(BiNode *bitree){//层次遍历二叉树
    if (bitree == NULL) {
        return;
    }
    Queue q;
    initQueue(&q);
    enqueue(&q, bitree);

    while (!isQueueEmpty(&q)) {
        BiNode *current = dequeue(&q);
        printf("%d ", current->data);

        if (current->lchild != NULL) {
            enqueue(&q, current->lchild);
        }
        if (current->rchild != NULL) {
            enqueue(&q, current->rchild);
        }
    }
//抄了答案，但是不得不说，答案写的太好了，怎么会有人用队列！！！队列，本质上是缓存
}

//6.48 求最近共同祖先
BiNode *near_anceseter(BiNode *q,BiNode *p,BiNode *bitree){
    if(q==bitree) return bitree;//刚好有一个是头结点(这个怎么表示报错啊)
    if(p==bitree)return bitree; 
    if(bitree==NULL) return bitree;
    BiNode* l_near_an = near_anceseter(p, q,bitree->lchild);
    BiNode* r_near_an = near_anceseter(p, q,bitree->rchild);
    
    // 如果左右子树都找到了p和q，则当前节点为最近祖先
    if (l_near_an && r_near_an) {
        return bitree;
    }
    
    // 如果只在左子树找到，则返回左子树的LCA
    if (l_near_an) {
        return l_near_an;
    }
    
    // 如果只在右子树找到，则返回右子树的LCA
    return r_near_an;
}

//6.54

BiNode *CreateNode(int data){
    BiNode *node=(BiNode *)malloc(sizeof(BiNode));
    node->data=data;
    node->rchild=NULL;
    node->lchild=NULL;
    return node;
}



//主函数中传入的i为1
void CreateTree(Sqlist sa, BiTree *T, int i)//前序创建
{
	(*T) = (BiTree)malloc(sizeof(BiNode));//分配空间,,这一句我还是没搞懂，等会问问同学/老师
	(*T)->data = sa.elem[i];
	if(sa.length >= 2 * i)//若长度大于2i，证明此节点有左子树
		CreateTree(sa, &(*T)->lchild, 2*i);
	else
	{
		(*T)->lchild = NULL;
	}
	if(sa.length >= 2 * i + 1) //若长度大于2i+1，证明此节点有右子树
		CreateTree(sa, &(*T)->rchild, 2*i + 1);
	else
	{
		(*T)->rchild = NULL;
	}
}

