p = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]
def suspect(a, s, d, n):
    x = pow(a, d, n)
    if x == 1:
        return True
    for r in range(s):
        if x == n - 1:
            return True
        x = x * x % n
    return False

def miller_rabin_test(n):
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

class DFSTree:
    def __init__(self, num = 0):
        self.child = [None] * 10
        self.num = num
        self.n = 0
        self.eleven = 0
        self.cards = dict()
    def at(self, x): # 1桁進める
        if self.child[x] == None:
            self.child[x] = DFSTree(self.num * 10 + x)
            self.child[x].n = self.n - 1
            self.child[x].eleven = x - self.eleven
        return self.child[x]
    def is_valid(self, s, x): # 2, 5, 11 の倍数チェック
        if self.n == 0:
            return self.eleven % 11
        if x % 2 and x != 5:
            if not (s[1] or s[3] or s[7] or s[9] or s[11] or s[13]):
                return False
        if x < 10:
            if not (s[0] or s[1] or s[2] or s[3] or s[4] or s[5] or s[6] or s[7] or s[8] or s[9]):
                if (self.eleven - s[10] + s[12] + s[13] * 2) % 11 == 0:
                    return False
        return True
    def insert(self, s, x): # s : 残っているカード, x: 次に使うカード
        t = list(s)
        t[x] -= 1
        nxt = self.at(x) if x < 10 else self.at(1).at(x - 10)
        if nxt.is_valid(t, x):
            t = tuple(t)
            if t not in nxt.cards:
                nxt.cards[t] = 14
            if nxt.cards[t] > x:
                nxt.cards[t] = x
    def search(self, ans, cnt = 1): # ans : 0~Kの枚数の配列, cnt : 何個探すか(-1で全部)
        if self.n == 0:
            if self.cards and miller_rabin_test(self.num):
                ans.append(self.num)
            return
        for s, last in self.cards.items():
            if last == 1:
                for i in range(4):
                    if s[i] and not s[i + 10]:
                        self.insert(s, i)
                for i in range(4, 11):
                    if s[i]:
                        self.insert(s, i)
                for i in range(12, 14):
                    if s[i]:
                        self.insert(s, i)
            else:
                for i in range(14):
                    if s[i]:
                        self.insert(s, i)
        for i in reversed(range(10)):
            if self.child[i] != None:
                if len(ans) != cnt:
                    self.child[i].search(ans, cnt)
                self.child[i] = None # destruct

def PermSearch(hand, n = 1):  # -> list<Int>
    if len(hand) > 1 and sum(hand) % 3 == 0:
        return []
    cards = [0] * 14
    for i in hand:
        cards[i] += 1
    a = DFSTree()
    a.cards[tuple(cards)] = 0
    a.n = 0
    for i in range(10):
        a.n += cards[i]
    for i in range(10, 14):
        a.n += cards[i] * 2
    ans = []
    a.search(ans, n)
    return ans

# 使用例

hands = [2, 3, 4, 5, 6]
for i in PermSearch(hands, -1):
    print(i)
'''
65423
54623
46523
45263
25643
25463
'''

hands = [i for i in range(1, 14)] * 4
hands.append(12)
hands.append(13)
for i in PermSearch(hands):
    print(i)
'''
999988887777666655554444333322221313131313121212121211111111101110110101
'''
