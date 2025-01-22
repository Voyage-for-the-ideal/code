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

//Kruskal
typedef struct edge { //������űߵ�Ȩ���Լ��ȴ�С
	int head;
	int dest;
	int num;
	int visited;
	Edge *next;
} Edge;

Edge *EnEdge(Edge *edge, int head, int dest, int num) { //��Ȩ��Ϊnum�ı߰��մ�С�������������
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

int DFS(ALGraph *graph, int first, int visited[]) { //��ȱ���,���ڼ����Ƿ��л�·
	int bool_1 = 0;
	visited[first - 1] = 1;
	ArcNode *current = graph->vertices[first - 1].firstarc;
	for (current = graph->vertices[first - 1].firstarc; current != NULL; current = current->nextarc) {
		if (!visited[current->adjvex]) {
			DFS(graph, (current->adjvex) + 1, visited);
		} else
			bool_1 = 1; //��ʾ�γ��˻�·
	}
	return bool_1;
}

void Kruskal(Edge *edge, int n, int e, int visited[]) {
	int total = 0;
	Edge *current = edge;
	ALGraph *graph = create_graph(n, e); //����һ����ͼ
	addedge(graph, current->head, current->dest, current->num);
	current = current->next;
	while (current != NULL) {
		addedge(graph, current->head, current->dest, current->num);
		addedge(graph, current->dest, current->head, current->num);
		for (int i = 0; i < n; i++)
			visited[i] = 0;
		if (DFS(graph, current->head, visited)) { //����γ���ͼ���򽫸�·ɾȥ
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
	int dest;//���������ߵ���һ�������˭����
} Close;

void Prim(ALGraph *graph, Close close[]) {
	int total = 0; //����Ȩֵ
	int n = graph->vexnum;
	int min = 1000; //����һ���ܴ����������Ƚ�
	int min_adj = 0; //����������С�ߵĽ��
	for (int i = 0; i < n; i++) {
		close[i].num = -1; //�����ʼ��
	}
	close[0].num = 0; //����һ������뵽��������
	ArcNode *current = graph->vertices[0].firstarc;
	while (current != NULL) { //����close�ıߵ�����
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
		for (int i = 0; i < n; i++) { //�ҳ���С����
			if (min >= close[i].num && close[i].num > 0) {
				min = close[i].num;
				min_adj = close[i].dest;
			}
		}
		total = total + min;
		close[min_adj].num = 0; //���õ������������

		current = graph->vertices[min_adj].firstarc;

		while (current != NULL) { //����close�ıߵ�����
			if (close[current->adjvex].num < 0) {
				close[current->adjvex].num = current->info;
				close[current->adjvex].dest = current->adjvex;
			} else if (current->info < close[current->adjvex].num && close[current->adjvex].num > 0) {
				close[current->adjvex].num = current->info;
				close[current->adjvex].dest = current->adjvex;
			}
			current = current->nextarc;
		}
		min = 1000; //��min��������
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

	printf("�����붥�����ͱ���");
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

		Prim(graph, close);//prim�������̸�������ʱ�����⣬���ø�
	}
	return 0;
}

