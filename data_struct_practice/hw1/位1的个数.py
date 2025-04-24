def cifang(i):#求2的i次方
    result=1
    for _ in range(0,i):
        result=result*2
    return result
n=int(input())
temp=1
total_1=0#记录1的个数
for i in range(0,32):
    if(temp<=n and (temp*2)>n):
        break
    temp*=2
temp_of_2=cifang(i)
for j in range(0,i+2):
    if(n>=temp_of_2):
        n-=temp_of_2
        total_1+=1
    temp_of_2=temp_of_2/2
print(total_1)