#include <stdio.h>
int cir(int n,int m)
{
	int p=0;
	for(int i=2;i<=n;i++)
	{
		p=(p+m)%i;
	}
	return p+1;
}
int main(){

int n,m;
scanf("%d %d", &n, &m);
int result=cir(n,m);
printf("%d",result);
return 0;
}
//��.ipynbЧ�ʸ���
//��������f(N,M)=(f(N-1,M)+M)%N
//(N,M)����һ�ֺ�����(N-1,M),f(N,M)��ʾ(N,M)��������ս�����±�
//https://blog.csdn.net/u011500062/article/details/72855826