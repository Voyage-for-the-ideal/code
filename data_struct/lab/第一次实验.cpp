#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_POLYS 5

typedef struct PolyNode {
	float coef; // ϵ��
	int exp;    // ָ��
	struct PolyNode *next;
} PolyNode;

typedef struct {
	PolyNode *head;
	int count; // ����
} Polynomial;

// ��������ʽ�ڵ�
PolyNode *createNode(float coef, int exp) {
	PolyNode *node = (PolyNode *)malloc(sizeof(PolyNode));
	node->coef = coef;
	node->exp = exp;
	node->next = NULL;
	return node;
}

// ����ڵ㵽����ʽ
void insertNode(Polynomial *poly, float coef, int exp) {
	PolyNode *node = createNode(coef, exp);
	if (poly->head == NULL || exp < poly->head->exp) {
		node->next = poly->head;
		poly->head = node;
	} else {
		PolyNode *current = poly->head;
		PolyNode *prev = NULL;
		while (current != NULL && current->exp < exp) {
			prev = current;
			current = current->next;
		}
		if (current != NULL && current->exp == exp) {
			// �޸����н��
			current->coef += coef;
			if (current->coef == 0) {//�������Ϊ0
				if (prev) {
					prev->next = current->next;
				}
				//�����������else�Ǹ�ɶ�ģ���ע�͵�
				// else {
				//     poly->head = current->next;
				// }
				free(current);
			}
		} else {
			node->next = prev->next;
			prev->next = node;

			//node->next = current;
			//if (prev) {
			//	prev->next = node;
			//}
		}
	}
	poly->count++;
}

// �������ʽ
void printPolynomial(Polynomial *poly) {
	PolyNode *current = poly->head;
	printf("����ʽ����: %d\n", poly->count);
	while (current != NULL) {
		if (current->coef > 0 && current != poly->head) {//�����˸��ŵ�����
			printf("+");
		}
		if (current->exp != 0) {

			printf("%g*x^%d ", current->coef, current->exp);
		} else
			printf("%g ", current->coef);
		current = current->next;
	}
	printf("\n");
}

// ���ٶ���ʽ
void destroyPolynomial(Polynomial *poly) {
	PolyNode *current = poly->head;
	while (current != NULL) {
		PolyNode *temp = current;
		current = current->next;
		free(temp);
	}
	poly->head = NULL;
	poly->count = 0;
}

// ��ն���ʽ,�о�������𲻴�
void clearPolynomial(Polynomial *poly) {
	destroyPolynomial(poly);
}

// ���
Polynomial addPolynomial(Polynomial *poly1, Polynomial *poly2) {
	Polynomial result;
	result.head = NULL;
	result.count = 0;

	PolyNode *current1 = poly1->head;
	PolyNode *current2 = poly2->head;

	while (current1 != NULL || current2 != NULL) {
		if (current1 != NULL && (current2 == NULL || current1->exp > current2->exp)) {
			insertNode(&result, current1->coef, current1->exp);
			current1 = current1->next;
		} else if (current2 != NULL && (current1 == NULL || current2->exp > current1->exp)) {
			insertNode(&result, current2->coef, current2->exp);
			current2 = current2->next;
		} else if (current1 != NULL && current2 != NULL && current1->exp == current2->exp) {
			float sum = current1->coef + current2->coef;
			if (sum != 0) {
				insertNode(&result, sum, current1->exp);
			}
			current1 = current1->next;
			current2 = current2->next;
		}
	}
	return result;
}

// ���,������ֻҪ�������coef�ĳɸ��ľͿ�����
Polynomial subtractPolynomial(Polynomial *poly1, Polynomial *poly2) {
	Polynomial result;
	result.head = NULL;
	result.count = 0;

	PolyNode *current1 = poly1->head;
	PolyNode *current2 = poly2->head;

	while (current1 != NULL || current2 != NULL) {
		if (current1 != NULL && (current2 == NULL || current1->exp > current2->exp)) {
			insertNode(&result, current1->coef, current1->exp);
			current1 = current1->next;
		} else if (current2 != NULL && (current1 == NULL || current2->exp > current1->exp)) {
			insertNode(&result, -current2->coef, current2->exp);
			current2 = current2->next;
		} else if (current1 != NULL && current2 != NULL && current1->exp == current2->exp) {
			float diff = current1->coef - current2->coef;
			if (diff != 0) {
				insertNode(&result, diff, current1->exp);
			}
			current1 = current1->next;
			current2 = current2->next;
		}
	}
	return result;
}

// ��ֵ
float evaluatePolynomial(Polynomial *poly, float x) {
	float result = 0;
	PolyNode *current = poly->head;
	if (current == NULL) {
		printf("û�ж���ʽ\n");
		return 0;
	}
	while (current != NULL) {
		result += current->coef * pow(x, current->exp);
		current = current->next;
	}
	return result;
}

// ΢��
Polynomial differentiatePolynomial(Polynomial *poly) {
	Polynomial result;
	result.head = NULL;
	result.count = 0;

	PolyNode *current = poly->head;
	while (current != NULL) {
		if (current->exp > 0) {
			insertNode(&result, current->coef * current->exp, current->exp - 1);
		}
		current = current->next;
	}
	return result;
}

// ��������
Polynomial indefiniteIntegralPolynomial(Polynomial *poly) {
	Polynomial result;
	result.head = NULL;
	result.count = 0;

	PolyNode *current = poly->head;
	while (current != NULL) {
		if (current->exp >= 0) {
			insertNode(&result, current->coef / (current->exp + 1), current->exp + 1);
		}
		current = current->next;
	}
	return result;
}

// ������
int main() {
	Polynomial poly1, poly2, result;
	poly1.head = NULL, poly2.head = NULL;
	poly1.count = 0;
	int exp = 0, exp1 = 0, exp2, k = 0, i = 0, j = 0, n = 0,
	    l = 0, m = 0; //j����switch�жϣ�l����8�еĶ����˵�,m��8�е������˵�,exp1���ڲ���ָ��,exp2�����޸�ָ��
	int a[5];
	for (i = 0; i < 5; i++) {
		a[i] = 0;
	}
	double coef = 0, x = 0, up = 0, down = 0, coef1 = 0,
	       coef2 = 0; //up�ǻ������ޣ�down�ǻ������ޣ�x��δ֪��,coef1���ڲ���ϵ��,coef2�����޸�ϵ��

//�������һ�´����ʽ�Ĺ���
	Polynomial *polyArray[MAX_POLYS];
	for (i = 0; i < MAX_POLYS; i++) {
		polyArray[i] = (Polynomial *)malloc(sizeof(Polynomial));
		if (polyArray[i] == NULL) {
			fprintf(stderr, "�ڴ����ʧ��\n");
			return 1;
		}
		polyArray[i]->head = NULL;  // ��ʼ��ͷָ��ΪNULL
		polyArray[i]->count = 0;    // ��ʼ������Ϊ0
	}


	while (j != 12) {
		//printf("��ѡ����в����Ķ���ʽ���������֣�\n");
		//scanf("%d", &i);
		//polyArray[i]->head = poly1.head;
		printf("��ѡ����:\n1.��������ʽ\n2. �������ʽ\n3. ���\n4. ���\n5. ��ֵ\n6. ����\n7. ���\n8. �޸�\n9. ΢��\n10. ��������\n11. ������\n12. �˳�\n");
		scanf("%d", &j);
		switch (j) {
			case 1:
				printf("��ѡ����д�ŵĶ���ʽ��ţ���������1~5��\n");
				scanf("%d", &i);
				if (polyArray[i - 1]->head != NULL) {
					printf("�Ѵ�������ʽ\n");
					break;
				}
				printf("���������ʽ����:");//������Ե������⣬�Ҿ�����Ҫ�ĵ�
				scanf("%d", &k);
				for (n = 0; n < k; n++) {
					printf("\n�������%d��ϵ��:", (n + 1));
					scanf("%lf", &coef);
					printf("\n�������%d�������", (n + 1));
					scanf("%d", &exp);
					insertNode(polyArray[i - 1], coef, exp);
					printf("\n");
				}

				break;
			case 2://���ﻹû�а��޶���ʽ�����������
				printf("��ѡ���������Ķ���ʽ���");
				scanf("%d", &i);
				if (polyArray[i - 1]->count == 0) {
					printf("û�ж���ʽ\n");
					break;//��֪������Բ���
				}

				printPolynomial(polyArray[i - 1]);
				break;
			case 3:
				printf("��ѡ��\n1.�����ж���ʽ���\n2.����������ʽ���\n");
				m = 0;
				k = 0;
				n = 0;
				scanf("%d", &m);
				switch (m) {
					case 1: {
						printf("��ѡ����Ӹ���(2~5)");
						scanf("%d", &k);
						for (n = 0; n < k; n++) {
							printf("��%d����", n + 1);
							scanf("%d", &a[n]);
						}
						result = addPolynomial(polyArray[a[0] - 1], polyArray[a[1] - 1]);
						for (n = 2; n < k; n++) {
							result = addPolynomial(&result, polyArray[a[n]]);
						}
						printf("��ͽ��: ");
						printPolynomial(&result);
						destroyPolynomial(&result);
						break;
					}
					case 2: {
						printf("��ѡ�����ʽ��1~5��");
						scanf("%d", &i);


						printf("��������Ӷ���ʽ����:");//�Ƚ�����ӵĶ���ʽ
						scanf("%d", &k);
						for (n = 0; n < k; n++) {
							printf("\n�������%d��ϵ��:", (n + 1));
							scanf("%lf", &coef);
							printf("\n�������%d�������", (n + 1));
							scanf("%d", &exp);
							insertNode(&poly2, coef, exp);
							printf("\n");
						}
						result = addPolynomial(polyArray[i - 1], &poly2);
						printf("��ͽ��: ");
						printPolynomial(&result);
						destroyPolynomial(&result);
						break;
					}
				}
				break;
			case 4:
				printf("��ѡ��\n1.���ж���ʽ���\n2.��������ʽ���\n");
				m = 0;
				k = 0;
				n = 0;
				scanf("%d", &m);
				switch (m) {
					case 1: {
						printf("���Ķ���ʽ��");
						scanf("%d", &a[0]);
						printf("�����Ķ���ʽ��");
						scanf("%d", &a[1]);
						result = subtractPolynomial(polyArray[a[0] - 1], polyArray[a[1] - 1]);
						printf("�����: ");
						printPolynomial(&result);
						destroyPolynomial(&result);
						break;
					}
					case 2: {
						printf("��ѡ�����ʽ��1~5��");
						scanf("%d", &i);
						printf("�������������ʽ����:");//�Ƚ�������Ķ���ʽ
						scanf("%d", &k);
						for (n = 0; n < k; n++) {
							printf("\n�������%d��ϵ��:", (n + 1));
							scanf("%lf", &coef);
							printf("\n�������%d�������", (n + 1));
							scanf("%d", &exp);
							insertNode(&poly2, coef, exp);
							printf("\n");
						}
						result = subtractPolynomial(polyArray[i - 1], &poly2);
						printf("�����: ");
						printPolynomial(&result);
						destroyPolynomial(&result);
						break;
					}
					break;
				}
				break;
			case 5://��ֵ
				printf("��ѡ����в����Ķ���ʽ���");
				scanf("%d", &i);

				if (polyArray[i - 1]->count == 0) {
					printf("û�ж���ʽ");
					return 0;
				}
				printf("������x��ֵ");
				scanf("%lf", &x);
				printf("����ʽ��x=%g����ֵ: %g\n", x, evaluatePolynomial(polyArray[i - 1], x));
				break;
			case 6:
				printf("��ѡ����в����Ķ���ʽ���");
				scanf("%d", &i);
				destroyPolynomial(polyArray[i - 1]);
				printf("����ʽ������\n");
				break;
			case 7:
				printf("��ѡ����в����Ķ���ʽ���");
				scanf("%d", &i);
				destroyPolynomial(polyArray[i - 1]);
				printf("����ʽ�����\n");
				break;
			case 8://�޸Ķ���ʽ
				printf("��ѡ���޸Ĺ���\n1. �����½ڵ�\n2. ɾ�����нڵ�\n3. �޸����нڵ�ϵ��\n4.�޸����нڵ�ָ��");
				scanf("%d", &l);
				switch (l) {
					case 1: { //�����½ڵ�
						printf("��ѡ����в����Ķ���ʽ���");
						scanf("%d", &i);
						printf("\n����������½ڵ�ϵ����");
						scanf("%lf", &coef);
						printf("\n����������½ڵ�ָ����");
						scanf("%d", &exp);
						insertNode(polyArray[i - 1], coef, exp);
						printf("\n");
						break;
					}
					case 2: { //ɾ�����нڵ�
						printf("��ѡ����в����Ķ���ʽ���");
						scanf("%d", &i);
						if (polyArray[i - 1]->count == 0) {
							printf("û�ж���ʽ\n");
							break;
						}
						printPolynomial(polyArray[i - 1]);
						printf("������ɾ������ָ����");
						scanf("%d", &exp1);
						PolyNode *current = polyArray[i - 1]->head; //������ʱָ�����
						PolyNode *prev = NULL;
						while (current != NULL && current->exp != exp1) {
							prev = current;
							current = current->next;
						}
						if (current != NULL && current->exp == exp1) {
							if (prev == NULL) { // �����ͷ�ڵ�
								polyArray[i - 1]->head = current->next;
							} else {
								prev->next = current->next;
							}
							free(current);
							printf("�޸ĳɹ�\n");
							polyArray[i - 1]->count--;
						} else {
							printf("�Ҳ����ýڵ�\n");
						}
						break;
					}
					case 3: { //ϵ���޸�
						printf("��ѡ����в����Ķ���ʽ���");
						scanf("%d", &i);
						if (polyArray[i - 1]->count == 0) {
							printf("û�ж���ʽ\n");
						} else {
							printPolynomial(polyArray[i - 1]);
							printf("�������޸ĵ���ָ����");
							scanf("%d", &exp1);
							PolyNode *current = polyArray[i - 1]->head; //������ʱָ�����
							PolyNode *prev = NULL;
							while (current != NULL && current->exp != exp1) {
								prev = current;
								current = current->next;
							}
							if (current == NULL) {
								printf("���޴˽ڵ�\n");
							} else {
								printf("��ϵ���޸�Ϊ:");
								scanf("%lf", &coef2);
								if (coef2 == 0) { //�൱��ɾ������
									if (prev == NULL) { // �����ͷ�ڵ�
										polyArray[i - 1]->head = current->next;
									} else {
										prev->next = current->next;
									}
									free(current);
									printf("�޸ĳɹ�\n");
									poly1.count--;
								} else {
									current->coef = coef2;
									printf("�޸ĳɹ�\n");
								}
							}
						}
						break;
					}
					case 4: { //ָ���޸�
						printf("��ѡ����в����Ķ���ʽ���");
						scanf("%d", &i);
						if (polyArray[i - 1]->count == 0) {
							printf("û�ж���ʽ\n");
							break;
						}
						printPolynomial(polyArray[i - 1]);
						printf("�������޸ĵ���ϵ����");
						scanf("%lf", &coef1);
						PolyNode *current1 = polyArray[i - 1]->head; //������ʱָ�����
						PolyNode *prev1 = NULL;
						while (current1 != NULL && current1->coef != coef1) {
							prev1 = current1;
							current1 = current1->next;
						}
						if (current1 == NULL) {
							printf("���޴˽ڵ�\n");
							break;
						} else {
							printf("��ָ���޸�Ϊ:");
							scanf("%d", &exp2);
							if (exp2 == current1->exp) {
								printf("��û�������޸�\n");
								break;
							}
							prev1->next = current1->next;
							free(current1);
							insertNode(polyArray[i - 1], coef1, exp2);
							printf("�޸ĳɹ�");
						}
						break;
					}
				}
				break;//����case8
			case 9://΢��
				j = 0;
				printf("��ѡ����в����Ķ���ʽ���");
				scanf("%d", &i);
				if (polyArray[i - 1]->count == 0) {
					printf("û�ж���ʽ");
					return 0;
				}
				printf("�󵼴���:");
				scanf("%d", &j);
				result = differentiatePolynomial(polyArray[i - 1]);
				for (i = 1; i < j; i++)
					result = differentiatePolynomial(&result);
				printf("΢�ֽ��: ");
				printPolynomial(&result);
				destroyPolynomial(&result);;
				break;
			case 10://��������
				printf("��ѡ����в����Ķ���ʽ���");
				scanf("%d", &i);
				result = indefiniteIntegralPolynomial(polyArray[i - 1]);
				printf("�������ֽ��: ");
				printPolynomial(&result);
				destroyPolynomial(&result);
				break;
			case 11://������
				printf("��ѡ����в����Ķ���ʽ���");
				scanf("%d", &i);
				result = indefiniteIntegralPolynomial(polyArray[i - 1]);
				printf("�������������:");
				scanf("%lf", &up);
				printf("\n�������������:");
				scanf("%lf", &down);
				printf("\n�����ֽ��:%lf ", evaluatePolynomial(&result, up) - evaluatePolynomial(&result, down));
				break;
			case 12:
				exit(0);

		}
	}
}
//�������ʵ�������¹��ܣ�
//1.��������ʽ�ڵ�
//2.����ڵ㵽����ʽ
//3.�������ʽ
//4.���ٶ���ʽ
//5.��ն���ʽ
//6.���
//7.���
//8.���
//9.΢��
//10.��������
// ��ע�⣬���������һ���򻯵�ʾ����ʵ��Ӧ���п����A?Ҫ����Ĵ���?��͹���ʵ��?1?7

