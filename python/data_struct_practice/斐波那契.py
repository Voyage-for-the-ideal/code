def F(N):
    if N == 0:
        return 0
    elif N == 1:
        return 1
    prev, curr = 0, 1
    for _ in range(2, N + 1):
        next_val = prev + curr
        prev, curr = curr, next_val
    return curr
N = int(input())
result = F(N)
print(result)