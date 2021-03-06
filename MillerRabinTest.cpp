#include <vector>
using namespace std;
using uint = unsigned;
using u64 = uint64_t;
using u128 = __uint128_t;

u128 modpow(u128 b, u64 e, u64 p){
    u128 ans = 1;
    for(u64 i = 1; e; i *= 2) {
        if(e & i){
            ans = ans * b % p;
            e ^= i;
        }
        b = b * b % p;
    }
    return ans;
}

bool suspect(uint a, uint s, u64 d, u64 n){
    u128 x = modpow(a, d, n);
    if (x == 1) return true;
    for (uint r = 0; r < s; ++r) {
        if (x == n - 1) return true;
        x = x * x % n;
    }
    return false;
}

bool miller_rabin(u64 n){
    if(n <= 1) return false;
    const vector<uint> test = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29}; // n < 2^64 なら十分
    if(n <= 29){
        for(uint i : test) if(n == i) return true;
        return false;
    }
    if(n % 2 == 0) return false;
    u64 d = n - 1;
    uint s = 0;
    while(d % 2 == 0){
        s++;
        d /= 2;
    }
    for(uint i : test) if(!suspect(i, s, d, n)) return false;
    return true;
}


// 検証
#include <iostream>
#include <random>
#include <boost/multiprecision/miller_rabin.hpp>
using namespace boost::multiprecision;
int main(){
    random_device rr;
    mt19937_64 rand(rr());
    for(uint i = 0; i < 10000; i++){
        bool ans = rand() & 1;
        u64 p = rand();
        while(miller_rabin_test(p, 50) != ans) p = rand();
        cout << p << " : " << miller_rabin(p) << endl;
        assert(miller_rabin(p) == ans);
    }
}
