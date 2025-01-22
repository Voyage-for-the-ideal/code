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

//7.27采用邻接表储存结构，编写一个判别无向图中任意给定的两个顶点之间是否存在一条长度为k的简单路径的算法(简单路径：其顶点序列中不含有重现的顶点)
int tell_route(ALGraph graph,int p,int q,int k) {
    if(p==q&&k==0) return TRUE;
    int i;
    ArcNode *current;
    current=graph.vertices[p].firstarc;
    while(current){
        if(tell_route(graph,current->adjvex,q,k-1)) return TRUE;//一旦找到了可以，可以用这个if和TRUE的组合跳出函数递归
        else if(k<=0)return FALSE;
        current=current->nextarc;
    }
    return FALSE;
}

//7.28已知有向图和图中两个顶点u和v，试编写算法求有向图中从u到v的所有简单路径，并以右图为例手工执行你的算法，画出相应的搜索过程图
void DFS(ALGraph G, int i, int j, int on[])
{
	static int n = 0;//用于标识访问了结点个数
	static char* vertex[MAX_VERTEX_NUM];//存储走过的路径
	int m;
	ArcNode *p;
	on[i] = TRUE;
	n++;
	*vertex[n] = G.vertices[i].data;
	if(i == j)
	{
		for(m = 1; m <= n; m++)
			printf("%c",*vertex[m]);
		printf("\n");
	}

	p = G.vertices[i].firstarc;
	while(p)
	{
		if(!on[p->adjvex])
			DFS(G, p->adjvex, j, on);
		p = p->nextarc;
	}
	on[i] = FALSE;//将回溯过程中遇到的结点标记为未访问，同时将数组中结点个数-1
	n--;
}
void SimplePath(ALGraph G, int i, int j)
{
	int m;
	int on[MAX_VERTEX_NUM];
	for(m = 1; m <= G.vexnum; m++)
		on[m] = FALSE;
	DFS(G, i, j, &on[MAX_VERTEX_NUM]);
}


//7.29试写一个算法，在以邻接矩阵方式储存的有向图G中求顶点i到顶点j的不含回路的、长度为k的路径数。
typedef struct {//邻接矩阵定义
    int numVertices; // 图中顶点的数量
    int matrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 邻接矩阵
} Graph;

int sim_route_num(Graph *graph,int i,int j,int visited[]){
	static int n=0;
	if(i==j){//在这里实现自增
		n++;
		return TRUE;
		}
	int visited[MAX_VERTEX_NUM];
	visited[i]=1;
	int current=0;
	for(current=0;current<graph->numVertices;current++){
		if(graph->matrix[i][current]==1&&!visited[current])sim_route_num(graph,current,j,visited);
	}
	visited[i]=FALSE;//如果这个结点已经遍历完其子树，那么将其标为未访问，使其不妨碍之后可能的访问
	return n;//只有原函数输出的n会传出
}
int route_num(Graph *graph,int i,int j){
	int temp=0;
	int visited[MAX_VERTEX_NUM];
	for(temp=0;temp<graph->numVertices;temp++)visited[temp]=FALSE;
	sim_route_num(graph,i,j,&visited[MAX_VERTEX_NUM]);
}
