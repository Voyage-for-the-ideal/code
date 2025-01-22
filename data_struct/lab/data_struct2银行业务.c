//����ҵ��ģ��
#include <stdio.h>
#include <stdlib.h>

#define OVERFLOW -1
#define ERROR -1

typedef struct Customer {
	double time;//�û�����ʱ��
	double wait;//�û��ȴ�ʱ��
	int finish;//�ж��Ƿ����
	struct Customer *next;
} Customer;

typedef struct QNode { //�������
	double money;//��Ҫ��Ǯ����������
	double time;//�û�������ʱ��
	double wait;
	int finish;//�жϲ����Ƿ����
	struct QNode *next;
} QNode, * QueuePtr;

typedef struct {
	QueuePtr front;//ͷ
	QueuePtr rear;//β
} LinkQueue;

void InitQueue(LinkQueue *queue) { //���г�ʼ��
	queue->front = queue->rear = (QNode *)malloc(sizeof(QNode));
	if (!queue)
		exit(OVERFLOW);
	queue->front->next = NULL;
}

void EnQueue(LinkQueue *queue, double money, double time) { //���ͻ��������
	QNode *qnode = (QueuePtr)malloc(sizeof(QNode));
	if (!qnode)
		exit(OVERFLOW);
	qnode->money = money;
	qnode->time = time;
	qnode->next = NULL;
	queue->rear->next = qnode;
	queue->rear = qnode;
}

void Initcustomer(Customer *custmer) { //�����ʼ��
	custmer = (Customer *)malloc(sizeof(Customer));
	custmer->next = NULL;
}

void Encustomer(Customer **customer, double time) { //����ͻ�������
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

void addtime(Customer **customer, double average_time, double real_time) { //���ӿͻ��ĵȴ�ʱ��
	Customer *current = *customer;

	while (current->next != NULL) {

		if (current->finish == 0 && (current->time <= (real_time + average_time))) { //�ڿͻ��������֮ǰ����Ҫ�ȴ�
			current->wait = current->wait + (average_time - (current->time + current->wait - real_time));
		}
		current = current->next;
	}
}

void addtime1(Customer **customer, double time,
              double real_time) { //������real_time<timeʱ����ȴ�ʱ�䣨���ʱ���õȣ�
	Customer *current = *customer;
	while (current->time != time) {
		current = current->next;
	}
	current->wait = 0;
}

void addtime2(Customer **customer, double time,
              double real_time) { //������real_time>timeʱ����ȴ�ʱ�䣨���ʱ���õȣ�
	Customer *current = *customer;
	while (current->time != time) {
		current = current->next;
	}
	current->wait = real_time - time;
}

void finish_cust1(Customer *customer, double time) { //��ĳ�ͻ�״̬��Ϊ���
	Customer *current = customer;

	while (current->time != time && current != NULL) {
		current = current->next;
	}
	current->finish = 1;
}

double finish_cust(Customer *customer, double time, double real_time,
                   double average_time) { //��ĳ���ͻ���״̬��Ϊ���,���ظ��û��ȴ�ʱ��
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

double DeQueue(LinkQueue *queue) { //�������׵�Ǯ
	QNode *current = NULL;
	double e;
	if (queue->front == queue->rear)
		return ERROR;
	current = queue->front->next;
	e = current->money;
	queue->front->next = current->next;
	if (queue->rear == current)
		queue->rear = queue->front; //��ֹ�պ�ֻ��һ��Ԫ��ʱcurrent��rear�غϣ���ʱ��rearָ��front
	free(current);
	return e;
}

double check_queue(LinkQueue *queue, double total, double past_total, Customer *customer, double average_time,

                   double *real_time) { //���ڶ�������,˳���޸�total��ֵ
	if (queue->front == queue->rear)
		return total;//�����û�ж��оͲ������

	else {
		QNode *current = NULL;
		double money;
		double user_time;
		double wait;
		double first_time;//���ڼ�¼��һ���޷�ȡǮ�ģ����ڽ������жϣ�
		current = queue->front->next;
		money = current->money;
		user_time = current->time;

		while ((money + total) >= 0 && (money + total) >= past_total) { //�ȵõ���һ���������β�Ŀͻ�

			total = total + money;
			if (*real_time <= user_time) {
				addtime1(&customer, user_time, *real_time);
				*real_time = user_time + average_time;
			} else {
				addtime2(&customer, user_time, *real_time);
				*real_time = *real_time + average_time;
			}
			//wait = finish_cust(customer, user_time, *real_time, average_time); //���Ϊ��ɲ����ӵȴ�ʱ��
			//*real_time == user_time + wait;
			//addtime(&customer, average_time, *real_time);

			if (queue->rear == current) { //����ֻ��һ���ڵ�����
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
			DeQueue(queue);//�����ٲ����β
			EnQueue(queue, money, user_time);
			money = current->money;
			user_time = current->time;
			//DeQueue(queue);//�����ٲ����β
			//EnQueue(queue, money, user_time);
		}

		while ((queue->front != queue->rear) && (current->time != first_time)) {

			if (((money + total) >= 0) && (money + total) < past_total) { //����ֹͣ���ĵڶ�������,�ͷ����������ֹͣ
				//queue->front->next = current->next; //�ֶ���front�����
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
				//wait = finish_cust(customer, user_time, *real_time, average_time); //���Ϊ��ɲ����ӵȴ�ʱ��
				//	*real_time == user_time + wait;
				//addtime(&customer, average_time, *real_time);

				return total;

			} else if (((money + total) < 0)) {
				current = current->next;
				DeQueue(queue);//�����ٲ����β
				EnQueue(queue, money, user_time);
				money = current->money;
				user_time = current->time;
				//DeQueue(queue);//�����ٲ����β
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
				//wait = finish_cust(customer, user_time, *real_time, average_time); //���Ϊ��ɲ����ӵȴ�ʱ��
				//*real_time == user_time + wait;
				//addtime(&customer, average_time, *real_time);

				//if (queue->rear == current) { //����ֻ��һ���ڵ�����
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

                        double *real_time) { //���ڶ�������,˳���޸�total��ֵ
	if (queue->front == queue->rear)
		return 0;//�����û�ж��оͲ������

	else {
		QNode *current = NULL;
		double money;
		double user_time;
		double wait;
		double first_time;//���ڼ�¼��һ���޷�ȡǮ�ģ����ڽ������жϣ�
		current = queue->front->next;
		money = current->money;
		user_time = current->time;

		while ((money + total) >= 0 && (money + total) >= past_total) { //�ȵõ���һ���������β�Ŀͻ�

			total = total + money;
			if (*real_time <= user_time) {
				addtime1(&customer, user_time, *real_time);
				*real_time = user_time + average_time;
			} else {
				addtime2(&customer, user_time, *real_time);
				*real_time = *real_time + average_time;
			}
			//wait = finish_cust(customer, user_time, *real_time, average_time); //���Ϊ��ɲ����ӵȴ�ʱ��
			//*real_time == user_time + wait;
			//addtime(&customer, average_time, *real_time);

			if (queue->rear == current) { //����ֻ��һ���ڵ�����
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
			DeQueue(queue);//�����ٲ����β
			EnQueue(queue, money, user_time);
			if (queue->front->next == queue->rear) { //��ֹ��ʱ��ʣһ��Ԫ�ض�ʹ��current���ͷ���
				current = queue->rear;
			}

		}

		while ((queue->front != queue->rear) && (current->time != first_time)) {

			if (((money + total) < 0)) {
				DeQueue(queue);//�����ٲ����β
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
				//wait = finish_cust(customer, user_time, *real_time, average_time); //���Ϊ��ɲ����ӵȴ�ʱ��
				//*real_time == user_time + wait;
				//addtime(&customer, average_time, *real_time);

				if (queue->rear == current) { //����ֻ��һ���ڵ�����
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

void print_cust(Customer *customer) { //�������
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

void finfish_queue(LinkQueue *queue, double close_time, Customer *customer) { //������ʣ�µĿͻ�ȫ����Ϊ���
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
	double real_time = 0; //time������ʱ������costΪ��ǰ�ķѵ�ʱ��,user_timeΪ�û�����ʱ��,real_time���ڼ�¼ϵͳʱ��
	double money;
	int N, i; //�����е�����;���Լ���
	double total = 10000, past_total = 10000; //total������һ��ʼ�е�Ǯ��past_total�����ж��Ƿ�����ڶ����е����
	LinkQueue *queue1 = (LinkQueue *)malloc(sizeof(LinkQueue)),
	           *queue2 = (LinkQueue *)malloc(sizeof(LinkQueue)); //�������1�Ͷ���2�Լ����еĿͻ�����
	Customer *customer = NULL;
	InitQueue(queue1);
	InitQueue(queue2);
	//Initcustomer(customer);

	printf("������N total close_time average_time\n");
	scanf("%d %lf %lf %lf", &N, &total, &close_time, &average_time);
//	N = 4;
//	total = 10000;
//	close_time = 600;
//	average_time = 10;
//	past_total = total;
	printf("�����롰�ͻ�����Ŀ��Լ����û�������ʱ�䡱(�ÿո�ֿ�)\n");

	for (i = 0; i < N; i++) { //���ͻ������һ�����кͿͻ�����
		scanf("%lf %lf", &money, &user_time);

		if (user_time <= close_time) { //����Ӫҵʱ��Ĳ�����
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

	if ((queue1->front->next->money) > 0) { //�����һ���ͻ���Ǯ���ȸ�������(������)
		time = queue1->front->time;
		money = DeQueue(queue1);
		//wait = finish_cust(customer, time, real_time, average_time);
		//addtime(&customer, average_time, real_time);
		addtime1(&customer, time, 0);
		finish_cust1(customer, time);
		real_time = time + average_time; //ϵͳʱ������
		N = N - 1; //�޸Ŀͻ�����
	}

	for (i = 0; i < N; i++) {
		time = queue1->front->next->time;
		money = DeQueue(queue1);

		if ((money) >= 0) { //�������ж�ȡ/��Ǯ
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
			//addtime(&customer, average_time, real_time); //���������˵ȴ�ʱ��
			//real_time = time + wait;
			total = check_queue(queue2, total, past_total, customer, average_time, &real_time); //���ڶ�������

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
				//addtime(&customer, average_time, real_time); //���������˵ȴ�ʱ��
				//real_time = time + wait;
			} else {
				//DeQueue(queue1);
				EnQueue(queue2, money, time);
				//real_time = time;
			}
		}
	}
	//���ʱ���һ���Ѿ�û���ˣ����һ�μ��ڶ��У��ڶ��ж���ȡǮ�ģ�һ��ȡ����Ǯ��ֱ�ӽ���ȴ�ʱ�䣬�����к����������
	check_queue_last(queue2, total, past_total, customer, average_time, &real_time);
	if (queue2->front == queue2->rear)
		print_cust(customer);

	else {
		finfish_queue(queue2, close_time, customer);
		print_cust(customer);
	}
}