#include <iostream>
#include <array>
#include <unordered_map>
#include <vector>
#include <boost/multiprecision/miller_rabin.hpp>


using Array = std::array<int, 14>;
namespace std {
    template <>
    struct hash<std::array<int, 14>> {
        uint64_t operator()(const Array& key) const {
            uint64_t x = 0;
            for(int i : key) {
                x ^= i;
                x ^= (x << 13);
                x ^= (x >> 7);
                x ^= (x << 17);
            }
            return x;
        }
    };
}

struct DFSTree {
    using BigInt = boost::multiprecision::cpp_int;
    DFSTree* child[10] = {};
    BigInt num;
    int n, eleven;
    std::unordered_map<Array, int> cards;
    DFSTree(BigInt num = 0): num(num) {}
    DFSTree& at(int x) {
        if(child[x] == nullptr) {
            child[x] = new DFSTree(num * 10 + x);
            child[x]->n = n - 1;
            child[x]->eleven = x - eleven;
        }
        return *child[x];
    }
    bool is_valid(const Array& s, int x) {
        if(n == 0) return eleven % 11;
        if(x % 2 && x != 5) if(!s[1] && !s[3] && !s[7] && !s[9] && !s[11] && !s[13]) return false;
        if(x < 10) if(!s[0] && !s[1] && !s[2] && !s[3] && !s[4] && !s[5] && !s[6] && !s[7] && !s[8] && !s[9])
            if((eleven - s[10] + s[12] + s[13] * 2) % 11 == 0) return false;
        return true;
    }
    void insert(const Array& s, int x) {
        Array t = s;
        t[x]--;
        DFSTree& next = x < 10 ? at(x) : at(1).at(x - 10);
        if(next.is_valid(t, x)){
            if(!next.cards.count(t)) next.cards[t] = x;
            else next.cards[t] = std::min(next.cards[t], x);
        }
    }
    void search(std::vector<BigInt>& ans, int cnt = 1) {
        if(n == 0) {
            if(cards.size() && boost::multiprecision::miller_rabin_test(num, 50)) ans.push_back(num);
            return;
        }
        for(auto& i : cards) {
            if(i.second == 1) {
                for(int j = 0; j < 4; j++) if(i.first[j] && !i.first[j + 10]) insert(i.first, j);
                for(int j = 4; j < 11; j++) if(i.first[j]) insert(i.first, j);
                for(int j = 12; j < 14; j++) if(i.first[j]) insert(i.first, j);
            }
            else {
                for(int j = 0; j < 14; j++) if(i.first[j]) insert(i.first, j);
            }
        }
        for(int i = 9; i >= 0; i--) if(child[i] != nullptr) {
            if(ans.size() != cnt) child[i]->search(ans, cnt);
            delete child[i];
            child[i] = nullptr;
        }
    }
};
std::vector<boost::multiprecision::cpp_int> PermSearch(const std::vector<int>& hand, int n = 1) {
    if(hand.size() > 1 && accumulate(hand.begin(), hand.end(), 0) % 3 == 0) return {};
    Array cards;
    cards.fill(0);
    for(auto& i : hand) cards[i]++;
    DFSTree a;
    a.cards[cards] = 0;
    a.n = 0;
    for(int i = 0; i < 10; i++) a.n += cards[i];
    for(int i = 10; i < 14; i++) a.n += cards[i] * 2;
    std::vector<boost::multiprecision::cpp_int> ans;
    a.search(ans, n);
    return ans;
}

// 使用例

int main(){
    std::vector<int> hand = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    for(auto& i : PermSearch(hand, 10)) std::cout << i << std::endl;
    /*
     98765432131121011
     98765432121311101
     98765432121101311
     98765432121011311
     98765432111210113
     98765432111012113
     98765432111011213
     98765432110111213
     98765432101311121
     98765431321101211
    */

   hand = {1, 7, 7, 9, 13};
   for(auto& i : PermSearch(hand, -1)) std::cout << i << std::endl;
   /*
    971371
    917713
    913771
    913177
    797131
    791317
    779131
    719713
    717139
    711397
    197713
    177913
    139177
    137791
    137197
    131779
    113797
   */
}
