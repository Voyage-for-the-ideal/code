#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0

//5.8
int solve(int i,int j){
    int k=0;
    if(i==j&&i%2==1)k=2*i-1;
    else if(i==j&&i%2==0)k=2*i;
    else if(i>j)k=2*i-1;
    else if(i<j)k=2*i;
    return k;
}
//5.26三元组输出十字链表
//为使之可运行，默认元素类型为int，可改
typedef struct OLNode {
  int i,j; 
  int e; 
  OLNode *right,*down; // 该非零元所在行表和列表的后继链域
}OLNode, *OLink;
 
typedef struct {
  OLink *rhead,*chead; // 行和列链表头指针向量基址
  int mu,nu,tu; // 稀疏矩阵的行数、列数和非零元个数
}CrossList;

void printmartix(CrossList crosslist)
/* 用函数Out3,依次以三元组格式输出十字链表表示的矩阵 */
{
   int i;
   OLink current=NULL;
   for(i = 1;i <= crosslist.mu; i++){
        current = crosslist.rhead[i]; //p指向每一行的基址
        while(current){
            printf("%d,%d,%d",current -> i,current -> j,current -> e);
            current = current -> right; //p向右移
        }
   }
}

//5.30,广义表深度的递归算法
//网上答案用了另外一种表达elemtype，遂借鉴
typedef char ElemType;
typedef struct Inode{
	int tag;
	union{
		ElemType data;
		struct Inode *head;
	}val; 
	struct Inode *tail;
}GLNode;


int GLDepth(GLNode *glnode){
	GLNode *current;
	int maxg = 0, dep;
    if(glnode->tag == 0)
	    return 0;
	current = glnode->val.head;
	if(current == NULL)
	    return 1;
	while(current != NULL){
		if(current->tag == 1){
			dep = GLDepth(current);
			if(dep > maxg)
			    maxg = dep;
		}
		current = current->tail;
	}
	return(maxg+1);
}

//5.32,判断两个广义表是否相等
//换成了另外一种链表
typedef enum {ATOM,LIST} ElemTag;
typedef struct GLNode{
     ElemTag tag;
     union {
       char atom;
       struct { 
         GLNode *hp, *tp;
       } ptr;
     }un;
} *GList;

int Equal(GList A, GList B)
/* 判断广义表A和B是否相等,是则返回TRUE,否则返回FALSE */
{
  if(A -> tag == ATOM && B -> tag == ATOM){
    //当都为原子节点ATOM时
        if(A -> un.atom == B -> un.atom)
            return TRUE;
        else
            return FALSE;
  }else if(A -> tag == LIST && B -> tag == LIST){
    //当都为广义表节点LIST时
        if(Equal(A -> un.ptr.hp,B -> un.ptr.hp) && Equal(A -> un.ptr.tp,B -> un.ptr.tp))
            //递归判断表头节点是否相等，表尾节点是否相等
            return TRUE;
        else
            return FALSE;    
  }  
}
