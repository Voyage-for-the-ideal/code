//用Huffman编码压缩文件
#include<bits/stdc++.h>
using namespace std;

const char Suffix_zip[6]=".huff";
#define MAX_VALUE 4294967295

typedef struct{
    unsigned int weight;
    unsigned char content;
    unsigned int parent,lchild,rchild;
}HTNode,*HuffmanTree;

typedef struct{

    string bit;
}HuffCode;

HTNode HuffmanTree(int weight[],int n){//建立huffman树
    HTNode huffTree[(2*n-1)];
    int count=0;//计数字符种类数
    int i=0,j=0;
    int temp_wei1,temp_wei2;//用以存放两个权重最小的数
    int temp_num1,temp_num2;
    for(i=0;i<256;i++){//将每个字符的权值输入进结点
        if(weight[i]!=0){//weight[i]代表ASCLL为i的字符的权重
            huffTree[count].weight = weight[i];
			huffTree[count].content = i;
			huffTree[count].parent = 0;
			huffTree[count].lchild = -1;
			huffTree[count].rchild = -1;
			count++;
        }
    }
    for(i=count;i<2*n-1;i++){//初始化非叶子结点
        huffTree[i].weight = 0;
		huffTree[i].parent = 0;
		huffTree[i].lchild = -1;
		huffTree[i].rchild = -1;
    }
    for(i=0;i<n-1;i++){//建立非叶子结点
        temp_wei1=MAX_VALUE;//最小
        temp_wei2=MAX_VALUE;//次小
        for(j=0;j<n;j++){
			if (huffTree[j].weight<temp_wei1 && huffTree[j].parent == 0)//查找最小权值m1和标号为x1；m2和x2为次最小
			{
				temp_wei2=temp_wei1;
				temp_num2 = temp_num1;
				temp_wei1 = huffTree[j].weight;
				temp_num1 = j;
			}
			else if (huffTree[j].weight<temp_wei2 && huffTree[j].parent == 0)
			{
				temp_wei2 = huffTree[j].weight;
				temp_num2 = j;
			}
		}
        huffTree[i+n].weight=temp_wei1+temp_wei2;
        huffTree[temp_num1].parent=i+n;
        huffTree[temp_num2].parent=i+n;
        huffTree[i+n].lchild=temp_num1;
        huffTree[i+n].rchild=temp_num2;
    }
    return huffTree[2*n-1];
}

HuffCode HuffmanCode(HTNode huffTree[],int n){//建立huffman编码
    HuffCode huffcode[n];
    int i;
    string s="";
    for(i=0;i<n;i++){
        int temp=i;
        auto parent=huffTree[temp].parent;
        while(parent!=-1){
            if(huffTree[parent].lchild==temp){
                s="0"+s;
            }else{
                s="1"+s;
            }
            temp=parent;
            parent=huffTree[parent].parent;
        }
    huffcode[]=s;
    }
    s="";
}

void Encode(char Huffmancode[],){//进行压缩

}
void Decode(){//进行解压

}

int main(){

}
