#include<stdio.h>
#include<stdlib.h>

#define MAX_VERTEX_NUM 100
#define TRUE 1
#define FALSE -1

//�ڽӱ���
typedef struct node{//����ߵĽ��
	int adjvex;
	struct node* nextarc;
}ArcNode;
typedef struct Node{//�����Ľ�㣨��ͷ��
	char data;//Ĭ��Ϊ��ĸ
	ArcNode *firstarc;
}VNode,AdjList[MAX_VERTEX_NUM];
typedef struct{
	AdjList vertices;
	int vexnum,arcnum;
	int kind;
}ALGraph;

/*���º�����֪��
��ʼ��ջInitStack()
��ջPush(S,i)
��ջPop(S,i)
��ͼ�н��ĶȲ�������������FindInDegree()
ջ�п�StackEmpty() */

//7.34
//�Ա�дһ���㷨���������޻�ͼG��ÿ�����㸳��һ��������ţ��������������������Ӷ���i������j��һ��������Ӧʹi<j��

//���������򣬵õ���������������ż���
void int_graph(ALGraph graph,int n){
    int indegegree[n]=FindInDegree(graph);//�Ը���������ȣ����浽������
    InitStack(S);
    int i=0;
    for(i=0;i<graph.vexnum;i++)
    if(!indegegree[i])Push(S,i);
    int count=0;
    int k=0;
    i=0;
    while(!StackEmpty(S)){
        Pop(S,i);
        count++;
        printf("%d,%c ",count,graph.vertices[i].data);
        for(ArcNode* current=graph.vertices[i].firstarc;current==NULL;current=current->nextarc){
            k=current->adjvex;
            if(!(--indegegree[k]))Push(S,k);
        }
    }
}

//7.37
//�����һ���������޻�ͼ���·�����㷨����������ʱ�临�Ӷ�

//�·�������һ�������Ϊ0�Ķ��㣬�ҵ���Щ��->��ÿ��������ȱ���������
int DFS(ALGraph graph,int v,int visited[]){
    static int depth=0;
    int i=0;
    depth++;
    visited[v]=TRUE;
    ArcNode* current;
    for(current=graph.vertices[i].firstarc;current==NULL;current=current->nextarc)
    depth--;//���˵��������������Ļ���
    if(!visited[current->adjvex])DFS(graph,current->adjvex,visited);
    return depth;
}

int longest_route(ALGraph graph,int n){
    int indegegree[n]=FindInDegree(graph);//�Ը���������ȣ����浽������
    InitStack(S);
    int i=0,n=graph.vexnum
    for(i=0;i<graph.vexnum;i++)
    if(!indegegree[i])Push(S,i);   
    int visited[n];
    for(i=0;i<n;i++)visited[i]=0;
    int longest_route[n];
    for(i=0;i<n;i++)longest_route[i]=0;
    i=0;
    while(!StackEmpty(S)){
        Pop(S,i);
        longest_route[i]=DFS(graph,i,visited[n]);//���󵽵�����ȴ�������
        for(i=0;i<n;i++)visited[i]=0;//�������������
    }
    int max=0;
    for(i=0;i<n;i++){
        if(longest_route[i]>max)max=longest_route[i];
    }
    return max;
}