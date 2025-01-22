#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 100

//ͼ�Ķ��壨�ڽӱ�
typedef struct node { //����ߵĽ��
	int adjvex;
	struct node *nextarc;
	int info;
} ArcNode;

typedef struct Node { //�����Ľ�㣨��ͷ��
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
	graph->kind = 0; //����Ϊ����ͼ
	graph->vexnum = n;
	graph->arcnum = e;
	for (i = 0; i < n; i++) {
		graph->vertices[i].firstarc = NULL;
	}
	return graph;
}

void addedge(ALGraph *graph, int i, int j, int num) { //��Ӵ�i��j�ı�,С�ı���ǰ��
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

void deledge(ALGraph *graph, int i, int j) { //ɾ��i,j����
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
		route[i].num = -1; //û��������ø����������
		route[i].visited = 0; //��ʼ��Ϊδ����
	}
	ArcNode *current = graph->vertices[begin - 1].firstarc;
	route[begin - 1].num = 0; //��ʾ���������·��Ϊ0
	route[begin - 1].visited = 1;
	while (current != NULL) {
		route[current->adjvex].num = current->info; //��һ�θ���
		current = current->nextarc;
	}
	for (int j = 1; j < n; j++) {
		for (int i = 0; i < n; i++) { //Ѱ����С��·��
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
	printf("�����붥�����ͱ���");
	scanf("%d %d", &n, &e);
	ALGraph *graph = create_graph(n, e);
	for (i = 0; i < e; i++) {
		scanf("%d %d %d", &temp1, &temp2, &num);
		addedge(graph, temp1, temp2, num);
		addedge(graph, temp2, temp1, num);
	}
	int visited[n];
	for (i = 0; i < n; i++) { //ȫ�����Ϊδ����
		visited[i] = 0;
	}
	Route route[n];
	printf("�����յ�\n");
	scanf("%d %d", &begin, &end);
	Dijkstra(graph, begin, end, route);
	return 0;
}