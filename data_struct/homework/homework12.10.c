//hash表
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 11
#define ERROR -1

int search_bool;//表示查找成功or失败

typedef struct hash { //线性探测法的hash表
	int *elem;//储存基址
	int count;//储存的值的个数
	int length;//hash表长度
} Hash;

typedef struct HashNode { //hashtable的结点
	int data;
	struct HashNode *next;
} HashNode;

typedef struct HashTable { //拉链法的hash表
	HashNode **table;
	int size;
} HashTable;

int Function_hash(int x, int p) { //除留余数法
	int result = 0;
	result = x % p;
	return result;
}

Hash *create_hash(int length) { //创建hash表
	Hash *hash = (Hash *)malloc(sizeof(Hash));
	hash->length = length;
	hash->count = 0;
	hash->elem = (int *)malloc(length * sizeof(int));
	for (int i = 0; i < length; i++) {
		hash->elem[i] = -1; //初始化
	}
	return hash;
}

void insert_hash(int key, int p, Hash *hash) { //将x插入hash
	int result = Function_hash(key, p);
	if (hash->elem[result] == -1) {
		hash->elem[result] = key;
	} else {
		result = (result + 1) % hash->length;
		while (hash->elem[result] != -1)
			result = (result + 1) % hash->length;
		hash->elem[result] = key;
	}
}

int search_hash(int key, int p, Hash *hash) { //在hash中查找key,返回查找次数
	int freq = 0; //查找次数
	int f_key = Function_hash(key, p);
	int temp = hash->elem[f_key];
	while (hash->elem[f_key] != -1 && hash->elem[f_key] != key) {
		f_key = f_key + 1;
		freq = freq + 1;
	}
	freq = freq + 1;
	if (hash->elem[f_key] == -1)
		search_bool = 0; //查找失败
	else if (hash->elem[f_key] == key)
		search_bool = 1; //查找成功
	return freq;
}

float ASL_true_hash(Hash *hash, int p, int num[], int n) { //hash表成功查找的平均长度
	float total = 0;
	int temp = 0;
	for (int i = 0; i < n; i++) {
		temp = search_hash(num[i], p, hash);
		if (search_bool)
			total = total + temp;
	}
	total = total / n;
	return total;
}

float ASL_false_hash(Hash *hash) { //hash表失败查找的平均长度
	float total = 0;
	int temp = 1; //至少查找一次
	for (int i = 0; i < hash->length; i++) {
		while (hash->elem[(i + temp) % hash->length] != -1 && temp != hash->length) {
			temp = temp + 1;
		}
		total = total + temp;
		temp = 1;
	}
	total = total / hash->length;
	return total;
}

void initHashTable(HashTable *table, int size) { //hashtable初始化
	table->size = size;
	table->table = (HashNode **)malloc(size * sizeof(HashNode *));
	for (int i = 0; i < size; i++) {
		table->table[i] = NULL; // 初始化所有桶为NULL
	}
}

HashNode *createNode(int data) {//hashtable创建结点
	HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
	if (!newNode) {
		return NULL; // 内存分配失败
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

void insertHashTable(HashTable *table, int data, int p) { //插入hashtable表
	unsigned int index = Function_hash(data, p);
	HashNode *newNode = createNode(data);
	HashNode *current = table->table[index];
	if (current == NULL)
		table->table[index] = newNode;
	else {
		while (current->next != NULL) {
			current = current->next;
		}
		// 将新节点插入到链表尾部
		current->next = newNode;
		newNode->next = NULL;
	}
}

int search_hashtable(int key, int p, HashTable *hashtable) { //hashtable中查找key，返回查找次数
	int freq = 0; //查找次数
	int f_key = Function_hash(key, p);
	HashNode *current = hashtable->table[f_key];
	if (hashtable->table[f_key] == NULL)
		return freq;
	else {
		while (current != NULL && current->data != key) {
			current = current->next;
			freq = freq + 1;
		}
		freq = freq + 1;
		if (current->data == key)
			return freq;
		else
			return ERROR;
	}
}

float ASL_true_hashtable(HashTable *hashtable, int p, int num[], int n) {
	float total = 0;
	int temp = 0;
	for (int i = 0; i < n; i++) {
		temp = search_hashtable(num[i], p, hashtable);
		if (temp != -1)
			total = total + temp;
	}
	total = total / n;
	return total;
}

float ASL_false_hashtable(HashTable *hashtable) { //hash表失败查找的平均长度
	float total = 0;
	int temp = 1;
	HashNode *current = hashtable->table[0];
	for (int i = 0; i < hashtable->size; i++) {
		current = hashtable->table[i];
		while (current != NULL) {
			temp = temp + 1;
			current = current->next;
		}
		total = total + temp;
		temp = 1;
	}
	total = total / hashtable->size;
	return total;
}


int main() {
	int n = 0, p = 0;
	float result = 0;
	int choose = 0;
	int i = 0, temp = 0;
	printf("请输入n:");
	scanf("%d", &n);
	int num[n];
	int *rigth = (int *)malloc(TABLE_SIZE * sizeof(int));
	int *false_1 = (int *)malloc(TABLE_SIZE * sizeof(int));
	for (i = 0; i < TABLE_SIZE; i++) {
		rigth[i] = 0;
		false_1[i] = 0;
	}
	for (int i = 0; i < n; i++) {
		scanf("%d", &num[i]);
		getchar();
	}
	printf("请输入p");
	scanf("%d", &p);
	printf("1. 线性探测法解决冲突\n2. 拉链法解决冲突");
	scanf("%d", &choose);
	if (choose == 1) {
		Hash *hash = create_hash(TABLE_SIZE);
		for (i = 0; i < n; i++) {
			insert_hash(num[i], p, hash);
		}
		printf("哈希表的地址：      ");
		for (i = 0; i < TABLE_SIZE; i++) {
			printf("%d ", i);
		}
		printf("\n表中的关键字：      ");
		for (i = 0; i < TABLE_SIZE; i++) {
			if (hash->elem[i] == -1)
				printf("- ");
			else
				printf("%d ", hash->elem[i]);
		}
		printf("\n成功查找次数：      ");
		for (i = 0; i < TABLE_SIZE; i++) {
			temp = search_hash(hash->elem[i], p, hash);
			if (search_bool == 1)
				rigth[i] = temp;
			else
				rigth[i] = 0;
		}
		for (i = 0; i < TABLE_SIZE; i++) {
			printf("%d ", rigth[i]);
		}
		printf("\n失败查找次数：      ");
		temp = 0; //重置temp值
		for (i = 0; i < hash->length; i++) {
			while (hash->elem[(i + temp) % hash->length] != -1 && temp != hash->length) {
				temp = temp + 1;
			}
			temp = temp + 1;
			//false_1[i] = temp;
			printf("%d ", temp);
			temp = 0;

		}
		result = ASL_true_hash(hash, p, num, n);
		printf("\n查找成功的平均查找长度：%f", result);
		result = ASL_false_hash(hash);
		printf("\n查找失败的平均查找长度：%f", result);
	} else {
		HashTable *hashtable = (HashTable *)malloc(sizeof(HashTable));
		initHashTable(hashtable, TABLE_SIZE);
		for (i = 0; i < n; i++) {
			insertHashTable(hashtable, num[i], p);
		}
		HashNode *current = hashtable->table[0];
		HashNode *current_1 = hashtable->table[0];
		printf("地址  关键字  成功查找次数  失败查找次数\n");
		for (i = 0; i < TABLE_SIZE; i++) {
			printf("%d ", i);
			current = hashtable->table[i];
			if (hashtable->table[i] == NULL)
				printf("-   0   1");//打印关键字
			else {
				while (current != NULL) {
					printf("%d   ", current->data);
					temp = search_hashtable(current->data, p, hashtable); //成功查找次数
					if (temp != -1)
						printf("%d   ", temp);
					temp = 1;
					current_1 = hashtable->table[i]; //失败查找次数
					while (current_1 != NULL) {
						temp = temp + 1;
						current_1 = current_1->next;
					}
					printf("%d ", temp);
					current = current->next;
					if (current != NULL)
						printf("\n  ");
				}
			}
			printf("\n");
		}
		result = ASL_true_hashtable(hashtable, p, num, n);
		printf("查找成功的平均查找长度：%f", result);
		result = ASL_false_hashtable(hashtable);
		printf("\n查找失败的平均查找长度：%f", result);
	}
	return 0;
}