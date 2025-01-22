//��Huffman����ѹ���ļ�
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

HTNode HuffmanTree(int weight[],int n){//����huffman��
    HTNode huffTree[(2*n-1)];
    int count=0;//�����ַ�������
    int i=0,j=0;
    int temp_wei1,temp_wei2;//���Դ������Ȩ����С����
    int temp_num1,temp_num2;
    for(i=0;i<256;i++){//��ÿ���ַ���Ȩֵ��������
        if(weight[i]!=0){//weight[i]����ASCLLΪi���ַ���Ȩ��
            huffTree[count].weight = weight[i];
			huffTree[count].content = i;
			huffTree[count].parent = 0;
			huffTree[count].lchild = -1;
			huffTree[count].rchild = -1;
			count++;
        }
    }
    for(i=count;i<2*n-1;i++){//��ʼ����Ҷ�ӽ��
        huffTree[i].weight = 0;
		huffTree[i].parent = 0;
		huffTree[i].lchild = -1;
		huffTree[i].rchild = -1;
    }
    for(i=0;i<n-1;i++){//������Ҷ�ӽ��
        temp_wei1=MAX_VALUE;//��С
        temp_wei2=MAX_VALUE;//��С
        for(j=0;j<n;j++){
			if (huffTree[j].weight<temp_wei1 && huffTree[j].parent == 0)//������СȨֵm1�ͱ��Ϊx1��m2��x2Ϊ����С
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

HuffCode HuffmanCode(HTNode huffTree[],int n){//����huffman����
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

void Encode(char Huffmancode[],){//����ѹ��

}
void Decode(){//���н�ѹ

}

int main(){

}
