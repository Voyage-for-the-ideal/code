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
1. range(a,b)是从a到b-1的列表
    range(b)是从0到b-1的列表
    range(a,b,c)是从a到b-1的列表，步长为c
2. 只需要验证到根号n就可以了
3. python中不可以用n++，只能用n+=1
'''