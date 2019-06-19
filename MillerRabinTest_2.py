# coding: utf-8
p = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]
def suspect(a, s, d, n):
 
    x = pow(a, d, n)
 
    if x == 1:
        return True
 
    for r in xrange(s):
 
        if x == n - 1:
            return True
 
        x = x * x % n
 
    return False;
 
def is_prime(n):
 
    if n <= 1:
        return False
 
    if n <= 100:
        return n in p
 
    if (n & 1) == 0:
        return False
 
    d = n - 1
    s = 0
 
    while (d & 1) == 0:
 
        s += 1
        d >>= 1
 
    for i in p:
 
        if not suspect(i, s, d, n):
            return False
 
    return True


# テスト
 
n = 99998888777766665555444433332222131313131313121212121111111011010101111
print(is_prime(n)) # True
