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

//7.27�����ڽӱ���ṹ����дһ���б�����ͼ�������������������֮���Ƿ����һ������Ϊk�ļ�·�����㷨(��·�����䶥�������в��������ֵĶ���)
int tell_route(ALGraph graph,int p,int q,int k) {
    if(p==q&&k==0) return TRUE;
    int i;
    ArcNode *current;
    current=graph.vertices[p].firstarc;
    while(current){
        if(tell_route(graph,current->adjvex,q,k-1)) return TRUE;//һ���ҵ��˿��ԣ����������if��TRUE��������������ݹ�
        else if(k<=0)return FALSE;
        current=current->nextarc;
    }
    return FALSE;
}

//7.28��֪����ͼ��ͼ����������u��v���Ա�д�㷨������ͼ�д�u��v�����м�·����������ͼΪ���ֹ�ִ������㷨��������Ӧ����������ͼ
void DFS(ALGraph G, int i, int j, int on[])
{
	static int n = 0;//���ڱ�ʶ�����˽�����
	static char* vertex[MAX_VERTEX_NUM];//�洢�߹���·��
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
	on[i] = FALSE;//�����ݹ����������Ľ����Ϊδ���ʣ�ͬʱ�������н�����-1
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


//7.29��дһ���㷨�������ڽӾ���ʽ���������ͼG���󶥵�i������j�Ĳ�����·�ġ�����Ϊk��·������
typedef struct {//�ڽӾ�����
    int numVertices; // ͼ�ж��������
    int matrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // �ڽӾ���
} Graph;

int sim_route_num(Graph *graph,int i,int j,int visited[]){
	static int n=0;
	if(i==j){//������ʵ������
		n++;
		return TRUE;
		}
	int visited[MAX_VERTEX_NUM];
	visited[i]=1;
	int current=0;
	for(current=0;current<graph->numVertices;current++){
		if(graph->matrix[i][current]==1&&!visited[current])sim_route_num(graph,current,j,visited);
	}
	visited[i]=FALSE;//����������Ѿ�����������������ô�����Ϊδ���ʣ�ʹ�䲻����֮����ܵķ���
	return n;//ֻ��ԭ���������n�ᴫ��
}
int route_num(Graph *graph,int i,int j){
	int temp=0;
	int visited[MAX_VERTEX_NUM];
	for(temp=0;temp<graph->numVertices;temp++)visited[temp]=FALSE;
	sim_route_num(graph,i,j,&visited[MAX_VERTEX_NUM]);
}
