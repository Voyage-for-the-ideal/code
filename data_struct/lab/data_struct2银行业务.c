//银行业务模拟
#include <stdio.h>
#include <stdlib.h>

#define OVERFLOW -1
#define ERROR -1

typedef struct Customer {
	double time;//用户到来时间
	double wait;//用户等待时间
	int finish;//判断是否完成
	struct Customer *next;
} Customer;

typedef struct QNode { //定义队列
	double money;//需要的钱（有正负）
	double time;//用户到来的时间
	double wait;
	int finish;//判断操作是否完成
	struct QNode *next;
} QNode, * QueuePtr;

typedef struct {
	QueuePtr front;//头
	QueuePtr rear;//尾
} LinkQueue;

void InitQueue(LinkQueue *queue) { //队列初始化
	queue->front = queue->rear = (QNode *)malloc(sizeof(QNode));
	if (!queue)
		exit(OVERFLOW);
	queue->front->next = NULL;
}

void EnQueue(LinkQueue *queue, double money, double time) { //将客户插入队列
	QNode *qnode = (QueuePtr)malloc(sizeof(QNode));
	if (!qnode)
		exit(OVERFLOW);
	qnode->money = money;
	qnode->time = time;
	qnode->next = NULL;
	queue->rear->next = qnode;
	queue->rear = qnode;
}

void Initcustomer(Customer *custmer) { //链表初始化
	custmer = (Customer *)malloc(sizeof(Customer));
	custmer->next = NULL;
}

void Encustomer(Customer **customer, double time) { //插入客户进链表
	Customer *temp = (Customer *)malloc((sizeof(Customer)));
	Customer *current = *customer;
	temp->time = time;
	temp->finish = 0;
	temp->wait = 0;
	temp->next = NULL;

	if (*customer == NULL) {
		*customer = temp;

	} else {
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = temp;
	}
}

void addtime(Customer **customer, double average_time, double real_time) { //增加客户的等待时间
	Customer *current = *customer;

	while (current->next != NULL) {

		if (current->finish == 0 && (current->time <= (real_time + average_time))) { //在客户办理结束之前来都要等待
			current->wait = current->wait + (average_time - (current->time + current->wait - real_time));
		}
		current = current->next;
	}
}

void addtime1(Customer **customer, double time,
              double real_time) { //用于在real_time<time时计算等待时间（这个时候不用等）
	Customer *current = *customer;
	while (current->time != time) {
		current = current->next;
	}
	current->wait = 0;
}

void addtime2(Customer **customer, double time,
              double real_time) { //用于在real_time>time时计算等待时间（这个时候不用等）
	Customer *current = *customer;
	while (current->time != time) {
		current = current->next;
	}
	current->wait = real_time - time;
}

void finish_cust1(Customer *customer, double time) { //将某客户状态标为完成
	Customer *current = customer;

	while (current->time != time && current != NULL) {
		current = current->next;
	}
	current->finish = 1;
}

double finish_cust(Customer *customer, double time, double real_time,
                   double average_time) { //将某个客户的状态标为完成,返回该用户等待时间
	Customer *current = customer;

	while (current->time != time && current != NULL) {
		current = current->next;
	}
	if (current == NULL)
		return ERROR;
	current->finish = 1;
	current->wait = real_time  - current->time;
	return current->wait;
}

double DeQueue(LinkQueue *queue) { //弹出队首的钱
	QNode *current = NULL;
	double e;
	if (queue->front == queue->rear)
		return ERROR;
	current = queue->front->next;
	e = current->money;
	queue->front->next = current->next;
	if (queue->rear == current)
		queue->rear = queue->front; //防止刚好只有一个元素时current和rear重合，此时将rear指向front
	free(current);
	return e;
}

double check_queue(LinkQueue *queue, double total, double past_total, Customer *customer, double average_time,

                   double *real_time) { //检查第二个队列,顺便修改total的值
	if (queue->front == queue->rear)
		return total;//如果还没有队列就不检查了

	else {
		QNode *current = NULL;
		double money;
		double user_time;
		double wait;
		double first_time;//用于记录第一个无法取钱的（用于结束的判断）
		current = queue->front->next;
		money = current->money;
		user_time = current->time;

		while ((money + total) >= 0 && (money + total) >= past_total) { //先得到第一个被插入队尾的客户

			total = total + money;
			if (*real_time <= user_time) {
				addtime1(&customer, user_time, *real_time);
				*real_time = user_time + average_time;
			} else {
				addtime2(&customer, user_time, *real_time);
				*real_time = *real_time + average_time;
			}
			//wait = finish_cust(customer, user_time, *real_time, average_time); //标记为完成并增加等待时间
			//*real_time == user_time + wait;
			//addtime(&customer, average_time, *real_time);

			if (queue->rear == current) { //处理只有一个节点的情况
				queue->rear = queue->front;
				free(current);
				return total;
			}

			current = current->next;
			money = current->money;
			user_time = current->time;
		}

		if ((money + total) < 0) {
			first_time = user_time;
			current = current->next;
			DeQueue(queue);//弹出再插入队尾
			EnQueue(queue, money, user_time);
			money = current->money;
			user_time = current->time;
			//DeQueue(queue);//弹出再插入队尾
			//EnQueue(queue, money, user_time);
		}

		while ((queue->front != queue->rear) && (current->time != first_time)) {

			if (((money + total) >= 0) && (money + total) < past_total) { //满足停止检查的第二个条件,释放完这个结点就停止
				//queue->front->next = current->next; //手动将front往后调
				//if (queue->rear == current)
				//	queue->rear = queue->front;
				//free(current);
				DeQueue(queue);
				total = total + money;
				if (*real_time <= user_time) {
					addtime1(&customer, user_time, *real_time);
					*real_time = user_time + average_time;
				} else {
					addtime2(&customer, user_time, *real_time);
					*real_time = *real_time + average_time;
				}
				//wait = finish_cust(customer, user_time, *real_time, average_time); //标记为完成并增加等待时间
				//	*real_time == user_time + wait;
				//addtime(&customer, average_time, *real_time);

				return total;

			} else if (((money + total) < 0)) {
				current = current->next;
				DeQueue(queue);//弹出再插入队尾
				EnQueue(queue, money, user_time);
				money = current->money;
				user_time = current->time;
				//DeQueue(queue);//弹出再插入队尾
				//EnQueue(queue, money, user_time);

			} else if (((money + total) >= 0) && (money + total) >= past_total) {

				total = total + money;
				if (*real_time <= user_time) {
					addtime1(&customer, user_time, *real_time);
					*real_time = user_time + average_time;
				} else {
					addtime2(&customer, user_time, *real_time);
					*real_time = *real_time + average_time;
				}
				//*real_time == user_time + wait;
				//wait = finish_cust(customer, user_time, *real_time, average_time); //标记为完成并增加等待时间
				//*real_time == user_time + wait;
				//addtime(&customer, average_time, *real_time);

				//if (queue->rear == current) { //处理只有一个节点的情况
				//	queue->rear = queue->front;
				//	free(current);
				//	return total;
				//}

				current = current->next;
				money = current->money;
				user_time = current->time;

				DeQueue(queue);
				//EnQueue(queue, money, user_time);
			}
		}
		return total;
	}
}

double check_queue_last(LinkQueue *queue, double total, double past_total, Customer *customer, double average_time,

                        double *real_time) { //检查第二个队列,顺便修改total的值
	if (queue->front == queue->rear)
		return 0;//如果还没有队列就不检查了

	else {
		QNode *current = NULL;
		double money;
		double user_time;
		double wait;
		double first_time;//用于记录第一个无法取钱的（用于结束的判断）
		current = queue->front->next;
		money = current->money;
		user_time = current->time;

		while ((money + total) >= 0 && (money + total) >= past_total) { //先得到第一个被插入队尾的客户

			total = total + money;
			if (*real_time <= user_time) {
				addtime1(&customer, user_time, *real_time);
				*real_time = user_time + average_time;
			} else {
				addtime2(&customer, user_time, *real_time);
				*real_time = *real_time + average_time;
			}
			//wait = finish_cust(customer, user_time, *real_time, average_time); //标记为完成并增加等待时间
			//*real_time == user_time + wait;
			//addtime(&customer, average_time, *real_time);

			if (queue->rear == current) { //处理只有一个节点的情况
				queue->rear = queue->front;
				free(current);
				return total;
			}

			current = current->next;
			money = current->money;
			user_time = current->time;
		}

		if ((money + total) < 0) {
			first_time = user_time;
			current = current->next;
			DeQueue(queue);//弹出再插入队尾
			EnQueue(queue, money, user_time);
			if (queue->front->next == queue->rear) { //防止此时仅剩一个元素而使得current被释放了
				current = queue->rear;
			}

		}

		while ((queue->front != queue->rear) && (current->time != first_time)) {

			if (((money + total) < 0)) {
				DeQueue(queue);//弹出再插入队尾
				EnQueue(queue, money, user_time);
			} else if (((money + total) >= 0)) {

				total = total + money;
				if (*real_time <= user_time) {
					addtime1(&customer, user_time, *real_time);
					*real_time = user_time + average_time;
				} else {
					addtime2(&customer, user_time, *real_time);
					*real_time = *real_time + average_time;
				}
				//wait = finish_cust(customer, user_time, *real_time, average_time); //标记为完成并增加等待时间
				//*real_time == user_time + wait;
				//addtime(&customer, average_time, *real_time);

				if (queue->rear == current) { //处理只有一个节点的情况
					queue->rear = queue->front;
					free(current);
					return total;
				}

				current = current->next;
				money = current->money;
				user_time = current->time;
			}
		}
		return 0;
	}
}

void print_cust(Customer *customer) { //输出链表
	Customer *current = customer;
	double total_time = 0;
	int N = 0;

	while (current != NULL) {
		printf("%lf\n", current->wait);
		total_time = total_time + current->wait;
		N++;
		current = current->next;
	}
	total_time = total_time / N;
	printf("%lf", total_time);
}

void finfish_queue(LinkQueue *queue, double close_time, Customer *customer) { //将表中剩下的客户全部标为完成
	QNode *current = queue->front->next;
	Customer *current1 = customer;
	double time;
	time = current->time;

	while (current != NULL) {

		while (current1->time != time && current1 != NULL) {
			current1 = current1->next;
		}
		current1->wait = close_time - current1->time;
		current1 = customer;
		current = current->next;
	}

}

int main() {
	double close_time = 600, time = 0, average_time, user_time;
	double wait;
	double real_time = 0; //time用作临时变量，cost为当前耗费的时间,user_time为用户到来时间,real_time用于记录系统时间
	double money;
	int N, i; //来银行的人数;用以计数
	double total = 10000, past_total = 10000; //total是银行一开始有的钱，past_total用于判断是否结束第二队列的审查
	LinkQueue *queue1 = (LinkQueue *)malloc(sizeof(LinkQueue)),
	           *queue2 = (LinkQueue *)malloc(sizeof(LinkQueue)); //代表队列1和队列2以及所有的客户队列
	Customer *customer = NULL;
	InitQueue(queue1);
	InitQueue(queue2);
	//Initcustomer(customer);

	printf("请输入N total close_time average_time\n");
	scanf("%d %lf %lf %lf", &N, &total, &close_time, &average_time);
//	N = 4;
//	total = 10000;
//	close_time = 600;
//	average_time = 10;
//	past_total = total;
	printf("请输入“客户办理的款额”以及“用户到来的时间”(用空格分开)\n");

	for (i = 0; i < N; i++) { //将客户读入第一个队列和客户链表
		scanf("%lf %lf", &money, &user_time);

		if (user_time <= close_time) { //超出营业时间的不受理
			Encustomer(&customer, user_time);
			EnQueue(queue1, money, user_time);
		} else
			exit(ERROR);
		getchar();
	}

//	Encustomer(&customer, 0);
//	EnQueue(queue1, -2000, 0);
//	Encustomer(&customer, 10);
//	EnQueue(queue1, -11000, 10);
//	Encustomer(&customer, 30);
//	EnQueue(queue1, -10000, 30);
//	Encustomer(&customer, 50);
//	EnQueue(queue1, 2000, 50);

	if ((queue1->front->next->money) > 0) { //如果第一个客户存钱，先给他存完(避免检查)
		time = queue1->front->time;
		money = DeQueue(queue1);
		//wait = finish_cust(customer, time, real_time, average_time);
		//addtime(&customer, average_time, real_time);
		addtime1(&customer, time, 0);
		finish_cust1(customer, time);
		real_time = time + average_time; //系统时间增加
		N = N - 1; //修改客户数量
	}

	for (i = 0; i < N; i++) {
		time = queue1->front->next->time;
		money = DeQueue(queue1);

		if ((money) >= 0) { //接下来判断取/存钱
			past_total = total;
			total = total + money;
			if (real_time <= time) {
				addtime1(&customer, time, real_time);
				real_time = time + average_time;
			} else {
				addtime2(&customer, time, real_time);
				real_time = real_time + average_time;
			}
			//real_time = time;
			//wait = finish_cust(customer, time, real_time, average_time);
			//addtime(&customer, average_time, real_time); //增加所有人等待时间
			//real_time = time + wait;
			total = check_queue(queue2, total, past_total, customer, average_time, &real_time); //检查第二个队列

		} else {
			if ((total + money) >= 0) {
				total = total + money;
				if (real_time <= time) {
					addtime1(&customer, time, real_time);
					real_time = time + average_time;
				} else {
					addtime2(&customer, time, real_time);
					real_time = real_time + average_time;
				}
				//real_time = time;
				//wait = finish_cust(customer, time, real_time, average_time);
				//addtime(&customer, average_time, real_time); //增加所有人等待时间
				//real_time = time + wait;
			} else {
				//DeQueue(queue1);
				EnQueue(queue2, money, time);
				//real_time = time;
			}
		}
	}
	//这个时候第一列已经没人了，最后一次检查第二列，第二列都是取钱的，一旦取不了钱，直接结算等待时间，出队列后不用再入队列
	check_queue_last(queue2, total, past_total, customer, average_time, &real_time);
	if (queue2->front == queue2->rear)
		print_cust(customer);

	else {
		finfish_queue(queue2, close_time, customer);
		print_cust(customer);
	}
}