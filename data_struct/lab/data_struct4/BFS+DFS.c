#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 100

//队列的定义
typedef struct QueueNode {
	int vertex;
	struct QueueNode *next;
} QueueNode;

// 队列
typedef struct Queue {
	QueueNode *front;
	QueueNode *rear;
} Queue;

//创建队列
Queue *createQueue() {
	Queue *queue = (Queue *)malloc(sizeof(Queue));
	queue->front = queue->rear = NULL;
	return queue;
}

// 队列是否为空
int isQueueEmpty(Queue *queue) {
	return queue->front == NULL;
}

// 入队
void enqueue(Queue *queue, int vertex) {
	QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
	newNode->vertex = vertex;
	newNode->next = NULL;

	if (isQueueEmpty(queue)) {
		queue->front = queue->rear = newNode;
		return;
	}

	queue->rear->next = newNode;
	queue->rear = newNode;
}

// 出队
int dequeue(Queue *queue) {
	if (isQueueEmpty(queue)) {
		return -1;
	}

	QueueNode *temp = queue->front;
	int vertex = temp->vertex;
	queue->front = queue->front->next;

	if (queue->front == NULL) {
		queue->rear = NULL;
	}

	free(temp);
	return vertex;
}

//图的定义（邻接表）
typedef struct node { //储存边的结点
	int adjvex;
	struct node *nextarc;
	int *info;
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

void addedge(ALGraph *graph, int i, int j) { //添加从i到j的边,小的边在前面
	ArcNode *new_node = (ArcNode *)malloc(sizeof(ArcNode));
	new_node->adjvex = j - 1;
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

void DFS(ALGraph *graph, int first, int visited[]) { //深度遍历
	visited[first - 1] = 1;
	printf("%d ", first);
	ArcNode *current = graph->vertices[first - 1].firstarc;
	for (current = graph->vertices[first - 1].firstarc; current != NULL; current = current->nextarc) {
		if (!visited[current->adjvex]) {
			DFS(graph, current->adjvex + 1, visited);
		}
	}

}

void BFS(ALGraph *graph, int first, int visited[]) { //广度遍历
	visited[first - 1] = 1;
	ArcNode *current = graph->vertices[first - 1].firstarc;
	Queue *queue = createQueue();
	enqueue(queue, first - 1);
	while (!isQueueEmpty(queue)) {
		int currentVertex = dequeue(queue);
		printf("%d ", currentVertex + 1);

		ArcNode *temp = graph->vertices[currentVertex].firstarc;

		while (temp) {
			int adjVertex = temp->adjvex;

			if (!visited[adjVertex]) {
				visited[adjVertex] = 1;
				enqueue(queue, adjVertex);
			}

			temp = temp->nextarc;
		}
	}
}

int main() {
	int n = 0, e = 0;
	int i = 0;
	int temp1 = 0, temp2 = 0;
	int choose = 0, first = 0;
	printf("请输入顶点数和边数");
	scanf("%d %d", &n, &e);
	ALGraph *graph = create_graph(n, e);
	for (i = 0; i < e; i++) {
		scanf("%d %d", &temp1, &temp2);
		addedge(graph, temp1, temp2);
		addedge(graph, temp2, temp1);
	}
	int visited[n];
	for (i = 0; i < n; i++) { //全部标记为未访问
		visited[i] = 0;
	}
	printf("DFS 1\nBFS 2\n");
	scanf("%d", &choose);
	printf("选择起始结点");
	scanf("%d", &first);
	if (choose == 1) {
		DFS(graph, first, visited);
	} else if (choose == 2) {
		BFS(graph, first, visited);
	}
	return 0;
}