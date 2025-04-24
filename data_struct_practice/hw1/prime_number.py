def prime_number(N):
    n=0
    for prev in range(2,N+1):
        is_prime=True
        for prev_1 in range(2,int(prev**0.5)+1):
            if prev%prev_1==0:
                is_prime=False
                break
        if is_prime:
            n += 1
    return n
N=int(input())
print(prime_number(N))

'''
1. range(a,b)�Ǵ�a��b-1���б�
    range(b)�Ǵ�0��b-1���б�
    range(a,b,c)�Ǵ�a��b-1���б�����Ϊc
2. ֻ��Ҫ��֤������n�Ϳ�����
3. python�в�������n++��ֻ����n+=1
'''