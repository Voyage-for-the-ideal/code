#include<stdio.h>
#include<stdlib.h>
//����utf8���±���

#define MAX_SIZE 100
#define CHUNKSIZE 4//4.23要用�?
/*说在最前面�?
1. 很抱歉，助教，之前我写的作业都不堪入目，因为我不知道怎么去写一个规范的算法�?
这导致我的作业一方面让我犯难，一方面我也不想去写，很多算法我跑不了，遑论改正错误，在这里
道歉qwq�?
2. 这次作业晚交太久了qwq，因为我昨天才去看bb平台上的作业，上次晚交也是这样qwq，抱歉抱歉，如果可以
助教之后可以在发布作业的时候在群里通知一下作业的ddl吗，这对我很有提醒作�?
3. 这次的三道题，第一道题我还是混沌，但是后面两道我都尽量让它能跑和实现，希望助教理解，实在是没时�?
改了，之后不会再有这样恶心的不能跑的算法交上来了
PB23151808 王玉�?*/

//4.17实现Replace(&S,T,V)
void Replace(&S,T,V){
    //用V替换S中出现的所有与T相等的不重叠的子�?
    int length1=StrLength(V);
    int length2=StrLength(T);
    int length3=StrLength(S);
    char Sub=NULL,temp1=NUll,temp2=NULL,temp3=NULL;//Sub存放长为T的用于比较，temp1,temp2用于存放前后面的�?,temp3用于存放修改后的�?
    int i=0;
    for(i=0;i<length3-length2;i++){
        Sub=SubString(&Sub,S,i,length2);
        if(StrCompare(Sub==V)==0){
            SubString(&temp1,S,1,i);//得到T前面的串
            SubString(&temp2,S,i+length2+1,length3-i-length2);//得到T后面的串
            Contact(&temp3,temp1,V);
            ClearString(&S);//准备修改S
            Contact(&S,temp3,temp2);//得到修改后的S
            i=i+length1;
            length3=length3-length2+length1;//修改i的位置和S的长�?
            ClearString(&Sub);
        }
        return S;
    }
}

//4.21编写基本操作
typedef struct string{
    char data;
    struct string *next;
}string;

string *creatchar(char data){//创建新结�?
    string *newchar=(string*)malloc(sizeof(string));
    if(newchar==NULL)return -1;
    newchar->data=data;
    newchar->next=NULL;
    return newchar;
}

string *initList(){//链表初始�?
    return createchar('\0');
}

void append(string *T,char data){//在链表末尾插入�?
    string* newchar=NULL;
    newchar=createchar(data);
    string *current=T;
    if(T==NULL)T=newchar;
    else{
        while(current->next!=NULL){
            current=current->next;
        }
        current->next=newchar;
    }
}
void freeString(string *T){
    string *current=T;
    string *next=NULL;
    while(current!=NULL){
    next=current->next;
    free(current);
    current=next;
    }
}

void StrAssign(string *T,char chars[]){//将char赋给T
    int length=0,i=0;
    string *current=NULL;//current用于赋给T字符
    current=(string*)malloc(sizeof(string));
    current=T;
    while(chars[length]!='\0') length++;//获取chars的长�?
    for(i=0;i<length;i++){
        current->data=chars[i];
        current=current->next;
    }
    current=NULL;
    return T;
}
void StrCopy(string *T,string *S){
    char data=NULL;
    string *current=S;
    string *newstring=NULL;
    freeString(T);
    while(current!=NULL){
        data=current->data;
        append(T,data);
        current=current->next;
    }
    return T;
}
int StrCompare(string *S,string *T){
    string *current1=T,*current2=S;
    while(current1!=NULL&&current2!=NULL){
        if(current1->data<current2->data)return -1;
        else if(current1->data>current2->data)return 1;
        else {
            current1=current1->next;
            current2=current2->next;
        }
    }
    if(current1==NULL&&current2==NULL)return 0;
    else if(current1==NULL&&current2!=NULL)return -1;
    else return 1;
}
int StrLength(string *S){
    int length=0;
    string *current=S;
    while(current!=NULL){
        length++;
        current=current->next;
    }
return length;
    }
void Contact(string *T,string *S1,string *S2){
    freeString(T);
    string *current1=S1,*current2=S2;
    while(current1!=NULL){
        append(T,current1->data);
    }
    while(current2!=NULL){
        append(T,current2->data);
    }
}
void SubString(string *Sub,string *S,int pos,int len){
    int i=0;
    string *current=S;
    freeString(Sub);
    for(i=0;i<(pos-1);i++){
        current=current->next;
    }
    for(i=0;i<len;i++)
    append(Sub,current->data);
}

//4.23链块结构储存串，编写判定对称性的算法
//使用栈进行储存并比较�?186正篇
typedef struct {
    char data[MAX_SIZE];
    int top; 
} Stack;

typedef struct Chunk{//定义块链
    char ch[CHUNKSIZE];//define在最上面
    struct Chunk *next;
}Chunk;
typedef struct{
    Chunk *head,*tail;
    int curlen;
}LString;

void initStack(Stack *s) {//栈初始化
    s->top = -1; 
}
int isFull(Stack *s) {//判断栈是否已�?
    return s->top == MAX_SIZE - 1;
}
int push(Stack *s, char element) {//入栈
    if (isFull(s)) {
        return 0; 
    }
    s->data[++s->top] = element; 
    return 1;
}
int pop(Stack *s) {//弹栈
    if (isEmpty(s)) {
        return 0; 
    }
    s->top--; 
    return 1; 
}
char peek(Stack *s) {//返回栈顶元素
    char element=NULL;
    if (isEmpty(s)) {
        printf("Stack is empty.\n");
        return 0; // 返回 0 表示获取失败
    }
    element = s->data[s->top]; // 返回栈顶元素
    return element; // 返回 1 表示获取成功
}

int Symmetric(LString *string){//判断对称�?
    int length1=0;//用于计算末尾�?#有多少个
    int length=0;
    int i=0,j=0;
    char element=NULL;
    Chunk *current=string->head;
    Stack *stack;//用于存放块链中数�?
    for(i=0;i<CHUNKSIZE;i++){
        if(string->tail->ch[i]=='#')length1++;
    }
    length=(string->curlen * CHUNKSIZE)-length1;//得到实际长度
    for(i=0;i<length/2;i++){
        push(stack,current->ch[i%CHUNKSIZE]);
        if(i!=0&&i%CHUNKSIZE==0)current=current->next;//换到下一个块�?
    }
    if(length%2==1&&CHUNKSIZE-(i%CHUNKSIZE+1)>=2)i=i+2;//将i往后移两个
    else if(length%2==1&&CHUNKSIZE-(i%CHUNKSIZE+1)<2){//中点在下一个块链中
        current=current->next;
        i=i+2;
    }
for(j=0;j<length/2;j++){
    element=peek(stack);
    pop(stack);
    if(element!=current->ch[i%CHUNKSIZE])return 0;//说明不对�?
}
return 1;//后半段全部和前半段对称，返回1
}