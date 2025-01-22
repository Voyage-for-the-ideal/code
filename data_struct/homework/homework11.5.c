#include <stdio.h>
#include<stdlib.h>

#define ERROR -1
#define MAXSIZE 100

typedef struct BiTNode{
    int data;//Ĭ������Ϊint���ͣ����޸ģ�
    struct BiNode *lchild,*rchild;
}BiNode,*BiTree;

typedef struct Queue{//����6.47���������Ķ���
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
int layer_tree(BiNode *bitree){//��α���������
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
//���˴𰸣����ǲ��ò�˵����д��̫���ˣ���ô�������ö��У��������У��������ǻ���
}

//6.48 �������ͬ����
BiNode *near_anceseter(BiNode *q,BiNode *p,BiNode *bitree){
    if(q==bitree) return bitree;//�պ���һ����ͷ���(�����ô��ʾ����)
    if(p==bitree)return bitree; 
    if(bitree==NULL) return bitree;
    BiNode* l_near_an = near_anceseter(p, q,bitree->lchild);
    BiNode* r_near_an = near_anceseter(p, q,bitree->rchild);
    
    // ��������������ҵ���p��q����ǰ�ڵ�Ϊ�������
    if (l_near_an && r_near_an) {
        return bitree;
    }
    
    // ���ֻ���������ҵ����򷵻���������LCA
    if (l_near_an) {
        return l_near_an;
    }
    
    // ���ֻ���������ҵ����򷵻���������LCA
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



//�������д����iΪ1
void CreateTree(Sqlist sa, BiTree *T, int i)//ǰ�򴴽�
{
	(*T) = (BiTree)malloc(sizeof(BiNode));//����ռ�,,��һ���һ���û�㶮���Ȼ�����ͬѧ/��ʦ
	(*T)->data = sa.elem[i];
	if(sa.length >= 2 * i)//�����ȴ���2i��֤���˽ڵ���������
		CreateTree(sa, &(*T)->lchild, 2*i);
	else
	{
		(*T)->lchild = NULL;
	}
	if(sa.length >= 2 * i + 1) //�����ȴ���2i+1��֤���˽ڵ���������
		CreateTree(sa, &(*T)->rchild, 2*i + 1);
	else
	{
		(*T)->rchild = NULL;
	}
}

