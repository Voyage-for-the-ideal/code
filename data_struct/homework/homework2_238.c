//2.38

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    int freq;
    struct Node *prior;
    struct Node *next;
} Node;

// 插入节点到链表头部
void InsertHead(Node **head, int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->freq = 0;
    newNode->prior = NULL;
    newNode->next = *head;
    if (*head != NULL) {
        (*head)->prior = newNode;
    }
    *head = newNode;
}

// 移动节点到链表头部
void MoveToHead(Node **head, Node *node) {
    if (node == *head) {
        return; // 已经是头部节点
    }
    if (node->prior != NULL) {
        node->prior->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prior = node->prior;
    }
    node->next = *head;
    node->prior = NULL;
    (*head)->prior = node;
    *head = node;
}

// LOCATE操作
void LOCATE(Node **head, int x) {
    Node *current = *head;
    Node *tail = NULL;

    // 查找x并更新频度
    while (current != NULL) {
        if (current->data == x) {
            current->freq++;
            break;
        }
        tail = current;
        current = current->next;
    }

    // 如果找到了x，调整链表
    if (current != NULL) {
        MoveToHead(head, current);
    }
}