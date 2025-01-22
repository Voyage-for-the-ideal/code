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

//Kruskal
typedef struct edge { //用来存放边的权重以及比大小
	int head;
	int dest;
	int num;
	int visited;
	Edge *next;
} Edge;

Edge *EnEdge(Edge *edge, int head, int dest, int num) { //将权重为num的边按照从小到大插入链表中
	Edge *temp = (Edge *)malloc(sizeof(Edge));
	temp->head = head;
	temp->dest = dest;
	temp->num = num;
	temp->next = NULL;
	temp->visited = 0;
	if (edge == NULL) {
		edge = temp;
	} else {
		Edge *current = edge;
		Edge *past = edge;
		while (current != NULL && current->num < num) {
			past = current;
			current = current->next;
		}
		if (current == past) {
			edge = temp;
			temp->next = current;
		} else {
			past->next = temp;
			temp->next = current;
		}
	}
	return edge;
}

int DFS(ALGraph *graph, int first, int visited[]) { //深度遍历,用于检验是否有回路
	int bool_1 = 0;
	visited[first - 1] = 1;
	ArcNode *current = graph->vertices[first - 1].firstarc;
	for (current = graph->vertices[first - 1].firstarc; current != NULL; current = current->nextarc) {
		if (!visited[current->adjvex]) {
			DFS(graph, (current->adjvex) + 1, visited);
		} else
			bool_1 = 1; //表示形成了回路
	}
	return bool_1;
}

void Kruskal(Edge *edge, int n, int e, int visited[]) {
	int total = 0;
	Edge *current = edge;
	ALGraph *graph = create_graph(n, e); //创建一个新图
	addedge(graph, current->head, current->dest, current->num);
	current = current->next;
	while (current != NULL) {
		addedge(graph, current->head, current->dest, current->num);
		addedge(graph, current->dest, current->head, current->num);
		for (int i = 0; i < n; i++)
			visited[i] = 0;
		if (DFS(graph, current->head, visited)) { //如果形成了图，则将该路删去
			current->visited = 1;
			deledge(graph, current->head, current->dest);
			deledge(graph, current->dest, current->head);
		}
		current = current->next;
	}
	current = edge;
	while (current != NULL) {
		if (current->visited != 1)
			total = total + current->num;
		current = current->next;
	}
	printf("%d", total);
}

//Prim
typedef struct close {
	int num;
	int dest;//表明该条线的另一个顶点和谁相连
} Close;

void Prim(ALGraph *graph, Close close[]) {
	int total = 0; //储存权值
	int n = graph->vexnum;
	int min = 1000; //设置一个很大的数，方便比较
	int min_adj = 0; //用来储存最小边的结点
	for (int i = 0; i < n; i++) {
		close[i].num = -1; //数组初始化
	}
	close[0].num = 0; //将第一个点加入到生成树中
	ArcNode *current = graph->vertices[0].firstarc;
	while (current != NULL) { //更新close的边的数据
		if (close[current->adjvex].num < 0) {
			close[current->adjvex].num = current->info;
			close[current->adjvex].dest = current->adjvex;
		} else if (current->info < close[current->adjvex].num) {
			close[current->adjvex].num = current->info;
			close[current->adjvex].dest = current->adjvex;
		}
		current = current->nextarc;
	}
	for (int j = 1; j < n; j++) {
		for (int i = 0; i < n; i++) { //找出最小的数
			if (min >= close[i].num && close[i].num > 0) {
				min = close[i].num;
				min_adj = close[i].dest;
			}
		}
		total = total + min;
		close[min_adj].num = 0; //将该点加入生成树中

		current = graph->vertices[min_adj].firstarc;

		while (current != NULL) { //更新close的边的数据
			if (close[current->adjvex].num < 0) {
				close[current->adjvex].num = current->info;
				close[current->adjvex].dest = current->adjvex;
			} else if (current->info < close[current->adjvex].num && close[current->adjvex].num > 0) {
				close[current->adjvex].num = current->info;
				close[current->adjvex].dest = current->adjvex;
			}
			current = current->nextarc;
		}
		min = 1000; //将min重新设置
	}
	printf("%d", total);
}


int main() {
	int n = 0, e = 0, num = 0;
	int i = 0;
	int temp1 = 0, temp2 = 0;
	int choose = 0, first = 0;
	int visited[n];
	Close close[n];

	printf("请输入顶点数和边数");
	scanf("%d %d", &n, &e);
	for (int i = 0; i < n; i++)
		visited[i] = 0;
	ALGraph *graph = create_graph(n, e);
	Edge *edge = NULL;
	for (i = 0; i < e; i++) {
		scanf("%d %d %d", &temp1, &temp2, &num);
		addedge(graph, temp1, temp2, num);
		addedge(graph, temp2, temp1, num);
		edge = EnEdge(edge, temp1, temp2, num);
	}
	printf("Kruskal 1\nPrim 2\n");
	scanf("%d", &choose);
	if (choose == 1) {
		Kruskal(edge, n, e, visited);
	} else if (choose == 2) {

		Prim(graph, close);//prim输入助教给的输入时有问题，还得改
	}
	return 0;
}

