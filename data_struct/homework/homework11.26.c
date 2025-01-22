#include<stdio.h>
#include<stdlib.h>

#define MAX_VERTEX_NUM 100
#define TRUE 1
#define FALSE -1

//邻接表储存
typedef struct node{//储存边的结点
	int adjvex;
	struct node* nextarc;
}ArcNode;
typedef struct Node{//储存点的结点（表头）
	char data;//默认为字母
	ArcNode *firstarc;
}VNode,AdjList[MAX_VERTEX_NUM];
typedef struct{
	AdjList vertices;
	int vexnum,arcnum;
	int kind;
}ALGraph;

/*如下函数已知：
初始化栈InitStack()
入栈Push(S,i)
弹栈Pop(S,i)
求图中结点的度并储存在数组里FindInDegree()
栈判空StackEmpty() */

//7.34
//试编写一个算法，给有向无环图G中每个顶点赋以一个整数序号，并满足以下条件：若从顶点i至顶点j有一条弧，则应使i<j。

//用拓扑排序，得到的排序建立整数序号即可
void int_graph(ALGraph graph,int n){
    int indegegree[n]=FindInDegree(graph);//对各顶点求入度，并存到数组里
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
//试设计一个求有向无环图中最长路径的算法，并估计其时间复杂度

//最长路径的起点一定是入度为0的顶点，找到这些点->对每个点用深度遍历求最深
int DFS(ALGraph graph,int v,int visited[]){
    static int depth=0;
    int i=0;
    depth++;
    visited[v]=TRUE;
    ArcNode* current;
    for(current=graph.vertices[i].firstarc;current==NULL;current=current->nextarc)
    depth--;//过滤掉访问其他子树的环节
    if(!visited[current->adjvex])DFS(graph,current->adjvex,visited);
    return depth;
}

int longest_route(ALGraph graph,int n){
    int indegegree[n]=FindInDegree(graph);//对各顶点求入度，并存到数组里
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
        longest_route[i]=DFS(graph,i,visited[n]);//将求到的最长长度存入数组
        for(i=0;i<n;i++)visited[i]=0;//将访问情况重置
    }
    int max=0;
    for(i=0;i<n;i++){
        if(longest_route[i]>max)max=longest_route[i];
    }
    return max;
}