#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // for isalpha, isdigit

// 定义节点结构
typedef struct Node {
    char data;
    struct Node* next;
} Node;

// 创建新节点
Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return NULL;
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 判断字符类型
int charType(char c) {
    if (isalpha(c)) return 0; // 字母
    if (isdigit(c)) return 1; // 数字
    return 2; // 其他
}

// 向循环链表添加节点
void addToCircularList(Node** head, Node** tail, char data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        *tail = newNode;
    } else {
        (*tail)->next = newNode;
        *tail = newNode;
    }
    newNode->next = *head; // 形成循环
}

// 分割链表
void splitList(Node* head, Node** alphaList, Node** numList, Node** otherList) {
    Node* current = head;
    Node* alphaHead = NULL, *alphaTail = NULL;
    Node* numHead = NULL, *numTail = NULL;
    Node* otherHead = NULL, *otherTail = NULL;

    while (current != NULL) {
        switch (charType(current->data)) {
            case 0: addToCircularList(&alphaHead, &alphaTail, current->data); break;
            case 1: addToCircularList(&numHead, &numTail, current->data); break;
            case 2: addToCircularList(&otherHead, &otherTail, current->data); break;
        }
        current = current->next;
    }

    *alphaList = alphaHead;
    *numList = numHead;
    *otherList = otherHead;
}

// 打印循环链表
void printCircularList(Node* head) {
    if (head == NULL) return;
    Node* current = head;
    do {
        printf("%c ", current->data);
        current = current->next;
    } while (current != head);
    printf("\n");
}

// 主函数
int main() {
    // 创建原始链表,随便创建的，这么多next没关系吧
    Node* head = createNode('a');
    head->next = createNode('1');
    head->next->next = createNode('!');
    head->next->next->next = createNode('b');
    head->next->next->next->next = createNode('2');

    Node* alphaList = NULL;
    Node* numList = NULL;
    Node* otherList = NULL;

    // 分割链表
    splitList(head, &alphaList, &numList, &otherList);

    // 打印结果
    printf("Alpha List: ");
    printCircularList(alphaList);

    printf("Number List: ");
    printCircularList(numList);

    printf("Other List: ");
    printCircularList(otherList);

    return 0;
}