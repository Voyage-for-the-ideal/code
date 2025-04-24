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

#Brute Force 打表
#Combinatorics 组合
#pow(a,b,c) a^b%c
#factorial=[1]*(10000+1)长度为10001的数组，初始化为1

#直接用数组储存阶乘，这样需要反复计算阶乘
'''
计算逆阶乘
使用了费马小定理，即a^(p-1) ≡ 1 (mod p) ，其中p为质数
a^(p-2) ≡ a^(-1) (mod p)
先计算(10000)!的逆元，然后往前计算逆元
其中：(x!)^(p-2)*x=(x-1)!^(p-2)*x*(x-1)^(p-1) ≡ (x-1)!^(p-2) (mod p)
'''

'''
为什么需要用逆元将除法改成乘法？
因为除法是一个很慢的操作，而乘法是一个很快的操作
并且除法可能导致错误，因为浮点数的精度问题
'''