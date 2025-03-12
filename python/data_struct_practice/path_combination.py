input_str=input()
n,m,q=map(int,input_str.split())
factorial=[1]*(10000+1)
for i in range(1,10000+1):
    factorial[i]=factorial[i-1]*i%1000000007
inv_factorial=[1]*(10000+1)
inv_factorial[10000]=pow(factorial[10000],1000000005,1000000007)
for i in range(10000-1,0,-1):
    inv_factorial[i]=inv_factorial[i+1]*(i+1)%1000000007
for _ in range(0,q):
    input_coordinate=input()
    x1,y1,x2,y2=map(int,input_coordinate.split())
    answer=factorial[x2-x1+y2-y1]*inv_factorial[x2-x1]%1000000007*inv_factorial[y2-y1]%1000000007
    print(answer)

#Brute Force ���
#Combinatorics ���
#pow(a,b,c) a^b%c
#factorial=[1]*(10000+1)����Ϊ10001�����飬��ʼ��Ϊ1

#ֱ�������鴢��׳ˣ�������Ҫ��������׳�
'''
������׳�
ʹ���˷���С������a^(p-1) �� 1 (mod p) ������pΪ����
a^(p-2) �� a^(-1) (mod p)
�ȼ���(10000)!����Ԫ��Ȼ����ǰ������Ԫ
���У�(x!)^(p-2)*x=(x-1)!^(p-2)*x*(x-1)^(p-1) �� (x-1)!^(p-2) (mod p)
'''

'''
Ϊʲô��Ҫ����Ԫ�������ĳɳ˷���
��Ϊ������һ�������Ĳ��������˷���һ���ܿ�Ĳ���
���ҳ������ܵ��´�����Ϊ�������ľ�������
'''