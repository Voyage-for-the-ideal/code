#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 100

//图的定义（邻接表）
typedef struct node { //储存边的结点
	int adjvex;
	struct node *nextarc;
	int info;
} ArcNode;

typedef struct Node { //储存点的结点（表头）
	int data;
	ArcNode *firstarc;
} VNode, AdjList[MAX_VERTEX_NUM];

typedef struct {
	AdjList vertices;
	int vexnum, arcnum;
	int kind;
} ALGraph;

typedef struct {
	int num;
	int visited;
} Route;

ALGraph *create_graph(int n, int e) {
	
	int i = 0;
	int temp1 = 0, temp2 = 0;
	ALGraph *graph = (ALGraph *)malloc(sizeof(ALGraph));
	graph->kind = 0; //表明为无向图
	graph->vexnum = n;
	graph->arcnum = e;
	for (i = 0; i < n; i++) {
		graph->vertices[i].firstarc = NULL;
	}
	return graph;
}

void addedge(ALGraph *graph, int i, int j, int num) { //添加从i到j的边,小的边在前面
	ArcNode *new_node = (ArcNode *)malloc(sizeof(ArcNode));
	new_node->adjvex = j - 1;
	new_node->info = num;
	new_node->nextarc = NULL;
	if (graph->vertices[i - 1].firstarc == NULL) {
		graph->vertices[i - 1].firstarc = new_node;
	} else {
		ArcNode *current = graph->vertices[i - 1].firstarc;
		ArcNode *past = graph->vertices[i - 1].firstarc;
		while (current != NULL && current->adjvex < j - 1) {
			past = current;
			current = current->nextarc;
		}
		if (current == past) {
			graph->vertices[i - 1].firstarc = new_node;
			new_node->nextarc = current;
		} else {

			past->nextarc = new_node;
			new_node->nextarc = current;
		}
	}
}

void deledge(ALGraph *graph, int i, int j) { //删除i,j间结点
	ArcNode *current = graph->vertices[i - 1].firstarc;
	ArcNode *past = graph->vertices[i - 1].firstarc;
	while (current->adjvex != j - 1) {
		past = current;
		current = current->nextarc;
	}
	if (current == past) {
		graph->vertices[i - 1].firstarc = current->nextarc;
		free(current);
	} else {
		past->nextarc = current->nextarc;
		free(current);
	}
}

void Dijkstra(ALGraph *graph, int begin, int end, Route route[]) {
	int n = graph->vexnum;
	int min = 1000, min_num = 0;
	for (int i = 0; i < n; i++) {
		route[i].num = -1; //没有无穷大，用负数代替好了
		route[i].visited = 0; //初始均为未访问
	}
	ArcNode *current = graph->vertices[begin - 1].firstarc;
	route[begin - 1].num = 0; //表示到自身最短路径为0
	route[begin - 1].visited = 1;
	while (current != NULL) {
		route[current->adjvex].num = current->info; //第一次更新
		current = current->nextarc;
	}
	for (int j = 1; j < n; j++) {
		for (int i = 0; i < n; i++) { //寻找最小的路径
			if (route[i].visited == 0 && route[i].num < min && route[i].num != -1) {
				min = route[i].num;
				min_num = i;
			}
		}
		current = graph->vertices[min_num].firstarc;
		while (current != NULL) {
			if (route[current->adjvex].visited == 0) {
				if (route[current->adjvex].num == -1)
					route[current->adjvex].num = current->info + route[min_num].num;
				else if ((route[min_num].num + current->info) < route[current->adjvex].num)
					route[current->adjvex].num = route[min_num].num + current->info;
			}
			current = current->nextarc;
		}
		route[min_num].visited = 1;
		min = 1000;
	}
	printf("%d", route[end - 1].num);
}

int main() {
	int n = 0, e = 0;
	int i = 0;
	int temp1 = 0, temp2 = 0, num = 0;
	int begin = 0, end = 0;
	printf("请输入顶点数和边数");
	scanf("%d %d", &n, &e);
	ALGraph *graph = create_graph(n, e);
	for (i = 0; i < e; i++) {
		scanf("%d %d %d", &temp1, &temp2, &num);
		addedge(graph, temp1, temp2, num);
		addedge(graph, temp2, temp1, num);
	}
	int visited[n];
	for (i = 0; i < n; i++) { //全部标记为未访问
		visited[i] = 0;
	}
	Route route[n];
	printf("起点和终点\n");
	scanf("%d %d", &begin, &end);
	Dijkstra(graph, begin, end, route);
	return 0;
}