#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_POLYS 5

typedef struct PolyNode {
	float coef; // 系数
	int exp;    // 指数
	struct PolyNode *next;
} PolyNode;

typedef struct {
	PolyNode *head;
	int count; // 项数
} Polynomial;

// 创建多项式节点
PolyNode *createNode(float coef, int exp) {
	PolyNode *node = (PolyNode *)malloc(sizeof(PolyNode));
	node->coef = coef;
	node->exp = exp;
	node->next = NULL;
	return node;
}

// 插入节点到多项式
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
			// 修改已有结点
			current->coef += coef;
			if (current->coef == 0) {//处理相加为0
				if (prev) {
					prev->next = current->next;
				}
				//不懂下面这个else是干啥的，遂注释掉
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

// 输出多项式
void printPolynomial(Polynomial *poly) {
	PolyNode *current = poly->head;
	printf("多项式项数: %d\n", poly->count);
	while (current != NULL) {
		if (current->coef > 0 && current != poly->head) {//考虑了负号的情形
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

// 销毁多项式
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

// 清空多项式,感觉两个差别不大
void clearPolynomial(Polynomial *poly) {
	destroyPolynomial(poly);
}

// 求和
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

// 求差,本质上只要将求和中coef改成负的就可以了
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

// 求值
float evaluatePolynomial(Polynomial *poly, float x) {
	float result = 0;
	PolyNode *current = poly->head;
	if (current == NULL) {
		printf("没有多项式\n");
		return 0;
	}
	while (current != NULL) {
		result += current->coef * pow(x, current->exp);
		current = current->next;
	}
	return result;
}

// 微分
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

// 不定积分
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

// 主函数
int main() {
	Polynomial poly1, poly2, result;
	poly1.head = NULL, poly2.head = NULL;
	poly1.count = 0;
	int exp = 0, exp1 = 0, exp2, k = 0, i = 0, j = 0, n = 0,
	    l = 0, m = 0; //j用于switch判断，l用于8中的二级菜单,m是8中的三级菜单,exp1用于查找指数,exp2用于修改指数
	int a[5];
	for (i = 0; i < 5; i++) {
		a[i] = 0;
	}
	double coef = 0, x = 0, up = 0, down = 0, coef1 = 0,
	       coef2 = 0; //up是积分上限，down是积分下限，x是未知量,coef1用于查找系数,coef2用于修改系数

//在这里搞一下存多项式的功能
	Polynomial *polyArray[MAX_POLYS];
	for (i = 0; i < MAX_POLYS; i++) {
		polyArray[i] = (Polynomial *)malloc(sizeof(Polynomial));
		if (polyArray[i] == NULL) {
			fprintf(stderr, "内存分配失败\n");
			return 1;
		}
		polyArray[i]->head = NULL;  // 初始化头指针为NULL
		polyArray[i]->count = 0;    // 初始化计数为0
	}


	while (j != 12) {
		//printf("请选择进行操作的多项式（输入数字）\n");
		//scanf("%d", &i);
		//polyArray[i]->head = poly1.head;
		printf("请选择功能:\n1.创建多项式\n2. 输出多项式\n3. 求和\n4. 求差\n5. 求值\n6. 销毁\n7. 清空\n8. 修改\n9. 微分\n10. 不定积分\n11. 定积分\n12. 退出\n");
		scanf("%d", &j);
		switch (j) {
			case 1:
				printf("请选择进行存放的多项式序号（输入数字1~5）\n");
				scanf("%d", &i);
				if (polyArray[i - 1]->head != NULL) {
					printf("已创建多项式\n");
					break;
				}
				printf("请输入多项式项数:");//这里调试的有问题，我觉得我要寄掉
				scanf("%d", &k);
				for (n = 0; n < k; n++) {
					printf("\n请输入第%d项系数:", (n + 1));
					scanf("%lf", &coef);
					printf("\n请输入第%d项次数：", (n + 1));
					scanf("%d", &exp);
					insertNode(polyArray[i - 1], coef, exp);
					printf("\n");
				}

				break;
			case 2://这里还没有把无多项式的情况讨论了
				printf("请选择进行输出的多项式序号");
				scanf("%d", &i);
				if (polyArray[i - 1]->count == 0) {
					printf("没有多项式\n");
					break;//不知道这里对不对
				}

				printPolynomial(polyArray[i - 1]);
				break;
			case 3:
				printf("请选择：\n1.与现有多项式相加\n2.与新增多项式相加\n");
				m = 0;
				k = 0;
				n = 0;
				scanf("%d", &m);
				switch (m) {
					case 1: {
						printf("请选择相加个数(2~5)");
						scanf("%d", &k);
						for (n = 0; n < k; n++) {
							printf("第%d项是", n + 1);
							scanf("%d", &a[n]);
						}
						result = addPolynomial(polyArray[a[0] - 1], polyArray[a[1] - 1]);
						for (n = 2; n < k; n++) {
							result = addPolynomial(&result, polyArray[a[n]]);
						}
						printf("求和结果: ");
						printPolynomial(&result);
						destroyPolynomial(&result);
						break;
					}
					case 2: {
						printf("请选择多项式（1~5）");
						scanf("%d", &i);


						printf("请输入相加多项式项数:");//先建立相加的多项式
						scanf("%d", &k);
						for (n = 0; n < k; n++) {
							printf("\n请输入第%d项系数:", (n + 1));
							scanf("%lf", &coef);
							printf("\n请输入第%d项次数：", (n + 1));
							scanf("%d", &exp);
							insertNode(&poly2, coef, exp);
							printf("\n");
						}
						result = addPolynomial(polyArray[i - 1], &poly2);
						printf("求和结果: ");
						printPolynomial(&result);
						destroyPolynomial(&result);
						break;
					}
				}
				break;
			case 4:
				printf("请选择：\n1.现有多项式相减\n2.新增多项式相减\n");
				m = 0;
				k = 0;
				n = 0;
				scanf("%d", &m);
				switch (m) {
					case 1: {
						printf("减的多项式是");
						scanf("%d", &a[0]);
						printf("被减的多项式是");
						scanf("%d", &a[1]);
						result = subtractPolynomial(polyArray[a[0] - 1], polyArray[a[1] - 1]);
						printf("求差结果: ");
						printPolynomial(&result);
						destroyPolynomial(&result);
						break;
					}
					case 2: {
						printf("请选择多项式（1~5）");
						scanf("%d", &i);
						printf("请输入相减多项式项数:");//先建立相减的多项式
						scanf("%d", &k);
						for (n = 0; n < k; n++) {
							printf("\n请输入第%d项系数:", (n + 1));
							scanf("%lf", &coef);
							printf("\n请输入第%d项次数：", (n + 1));
							scanf("%d", &exp);
							insertNode(&poly2, coef, exp);
							printf("\n");
						}
						result = subtractPolynomial(polyArray[i - 1], &poly2);
						printf("求差结果: ");
						printPolynomial(&result);
						destroyPolynomial(&result);
						break;
					}
					break;
				}
				break;
			case 5://求值
				printf("请选择进行操作的多项式序号");
				scanf("%d", &i);

				if (polyArray[i - 1]->count == 0) {
					printf("没有多项式");
					return 0;
				}
				printf("请输入x的值");
				scanf("%lf", &x);
				printf("多项式在x=%g处的值: %g\n", x, evaluatePolynomial(polyArray[i - 1], x));
				break;
			case 6:
				printf("请选择进行操作的多项式序号");
				scanf("%d", &i);
				destroyPolynomial(polyArray[i - 1]);
				printf("多项式已销毁\n");
				break;
			case 7:
				printf("请选择进行操作的多项式序号");
				scanf("%d", &i);
				destroyPolynomial(polyArray[i - 1]);
				printf("多项式已清空\n");
				break;
			case 8://修改多项式
				printf("请选择修改功能\n1. 插入新节点\n2. 删除已有节点\n3. 修改已有节点系数\n4.修改已有节点指数");
				scanf("%d", &l);
				switch (l) {
					case 1: { //插入新节点
						printf("请选择进行操作的多项式序号");
						scanf("%d", &i);
						printf("\n请输入插入新节点系数：");
						scanf("%lf", &coef);
						printf("\n请输入插入新节点指数：");
						scanf("%d", &exp);
						insertNode(polyArray[i - 1], coef, exp);
						printf("\n");
						break;
					}
					case 2: { //删除已有节点
						printf("请选择进行操作的多项式序号");
						scanf("%d", &i);
						if (polyArray[i - 1]->count == 0) {
							printf("没有多项式\n");
							break;
						}
						printPolynomial(polyArray[i - 1]);
						printf("请输入删除的项指数：");
						scanf("%d", &exp1);
						PolyNode *current = polyArray[i - 1]->head; //建立临时指针查找
						PolyNode *prev = NULL;
						while (current != NULL && current->exp != exp1) {
							prev = current;
							current = current->next;
						}
						if (current != NULL && current->exp == exp1) {
							if (prev == NULL) { // 如果是头节点
								polyArray[i - 1]->head = current->next;
							} else {
								prev->next = current->next;
							}
							free(current);
							printf("修改成功\n");
							polyArray[i - 1]->count--;
						} else {
							printf("找不到该节点\n");
						}
						break;
					}
					case 3: { //系数修改
						printf("请选择进行操作的多项式序号");
						scanf("%d", &i);
						if (polyArray[i - 1]->count == 0) {
							printf("没有多项式\n");
						} else {
							printPolynomial(polyArray[i - 1]);
							printf("请输入修改的项指数：");
							scanf("%d", &exp1);
							PolyNode *current = polyArray[i - 1]->head; //建立临时指针查找
							PolyNode *prev = NULL;
							while (current != NULL && current->exp != exp1) {
								prev = current;
								current = current->next;
							}
							if (current == NULL) {
								printf("查无此节点\n");
							} else {
								printf("将系数修改为:");
								scanf("%lf", &coef2);
								if (coef2 == 0) { //相当于删除操作
									if (prev == NULL) { // 如果是头节点
										polyArray[i - 1]->head = current->next;
									} else {
										prev->next = current->next;
									}
									free(current);
									printf("修改成功\n");
									poly1.count--;
								} else {
									current->coef = coef2;
									printf("修改成功\n");
								}
							}
						}
						break;
					}
					case 4: { //指数修改
						printf("请选择进行操作的多项式序号");
						scanf("%d", &i);
						if (polyArray[i - 1]->count == 0) {
							printf("没有多项式\n");
							break;
						}
						printPolynomial(polyArray[i - 1]);
						printf("请输入修改的项系数：");
						scanf("%lf", &coef1);
						PolyNode *current1 = polyArray[i - 1]->head; //建立临时指针查找
						PolyNode *prev1 = NULL;
						while (current1 != NULL && current1->coef != coef1) {
							prev1 = current1;
							current1 = current1->next;
						}
						if (current1 == NULL) {
							printf("查无此节点\n");
							break;
						} else {
							printf("将指数修改为:");
							scanf("%d", &exp2);
							if (exp2 == current1->exp) {
								printf("你没有做出修改\n");
								break;
							}
							prev1->next = current1->next;
							free(current1);
							insertNode(polyArray[i - 1], coef1, exp2);
							printf("修改成功");
						}
						break;
					}
				}
				break;//跳出case8
			case 9://微分
				j = 0;
				printf("请选择进行操作的多项式序号");
				scanf("%d", &i);
				if (polyArray[i - 1]->count == 0) {
					printf("没有多项式");
					return 0;
				}
				printf("求导次数:");
				scanf("%d", &j);
				result = differentiatePolynomial(polyArray[i - 1]);
				for (i = 1; i < j; i++)
					result = differentiatePolynomial(&result);
				printf("微分结果: ");
				printPolynomial(&result);
				destroyPolynomial(&result);;
				break;
			case 10://不定积分
				printf("请选择进行操作的多项式序号");
				scanf("%d", &i);
				result = indefiniteIntegralPolynomial(polyArray[i - 1]);
				printf("不定积分结果: ");
				printPolynomial(&result);
				destroyPolynomial(&result);
				break;
			case 11://定积分
				printf("请选择进行操作的多项式序号");
				scanf("%d", &i);
				result = indefiniteIntegralPolynomial(polyArray[i - 1]);
				printf("请输入积分上限:");
				scanf("%lf", &up);
				printf("\n请输入积分下限:");
				scanf("%lf", &down);
				printf("\n定积分结果:%lf ", evaluatePolynomial(&result, up) - evaluatePolynomial(&result, down));
				break;
			case 12:
				exit(0);

		}
	}
}
//这个代码实现了以下功能：
//1.创建多项式节点
//2.插入节点到多项式
//3.输出多项式
//4.销毁多项式
//5.清空多项式
//6.求和
//7.求差
//8.求和
//9.微分
//10.不定积分
// 请注意，这个代码是一个简化的示例，实际应用中可能霢?要更多的错误棢?查和功能实现?1?7

