#include <iostream>
#include <numeric>
#include <array>
#include <string>
#include <queue>
#include <unordered_set>
#include <boost/multiprecision/miller_rabin.hpp>
using namespace std;
using namespace boost::multiprecision;



const array<string, 14> to_s = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"};

struct SearchState{
    int n, eleven = 0, oned, last = 0;
    array<int, 14> cards;
    string s;
    vector<int> v;
    SearchState(array<int, 14> cards) : n(accumulate(cards.begin(), cards.end(), 0)), oned(accumulate(cards.begin(), cards.begin() + 10, 0)), cards(cards){}
    void add(int a){
        n--;
        cards[a]--;
        s += to_s[a];
        v.push_back(a);
        if(a > 9)eleven += a;
        else{
            eleven = a - eleven;
            oned--;
        }
    }
    void prev(){
        int a = v.back();
        n++;
        cards[a]++;
        s.pop_back();
        v.pop_back();
        if(a > 9){
            eleven -= a;
            s.pop_back();
        }
        else{
            eleven = a - eleven;
            oned++;
        }
    }
    const bool operator <(const SearchState &rhs) const noexcept {
        auto a = s.begin(), b = rhs.s.begin();
        while(true){
            if(a == s.end()) return false;
            if(b == rhs.s.end()) return true;
            if(*a != *b) return *a < *b;
            a++; b++;
        }
    }
};

void PermSearch(array<int, 14> cards, int k = 1){
    if(!k)return;
    int n = accumulate(cards.begin(), cards.end(), 0);
    if(n == 1){
        for(int i = 0; i < 14; i++) if(cards[i]) {
            if(miller_rabin_test(i, 10)) cout << i << endl;
            return;
        }
    }
    int sum = 0;
    for(int i = 0; i < 14; i++) sum += i * cards[i];
    if(sum % 3 == 0) return;
    if(n == accumulate(cards.begin() + 10, cards.end(), 0) && sum % 11 == 0) return;
    const auto hash = [](pair<array<int, 14>, vector<int>> a){
        unsigned ans = 0;
        for(auto &i : a.first) ans = ans * 10007 + i;
        for(auto &i : a.second) ans = ans * 10007 + i;
        return ans;
    };
    unordered_set<pair<array<int, 14>, vector<int>>, decltype(hash)> checked({}, hash);
    priority_queue<SearchState> q;
    q.push(cards);
    const auto isValid = [&checked](const SearchState &a) -> bool{
        if(a.n == 0) return checked.insert({a.cards, a.v}).second;
        if(a.oned == 0){
            int eleven = a.eleven;
            for(int i = 10; i < 14; i++) eleven += a.cards[i] * i;
            if(eleven % 11 == 0) return false;
        }
        if(!(a.cards[1] || a.cards[3] || a.cards[7] || a.cards[9] || a.cards[11] || a.cards[13])) return false;
        if(a.v.size() > 1 && a.v[a.v.size() - 2] == 1){
            if(a.v.back() == 11) return false;
            if(a.v.back() < 4 && a.cards[a.v.back() + 10]) return false;
        }
        return checked.insert({a.cards, a.v}).second;
    };
    const auto push_next = [&q, &isValid](SearchState a) -> void{
        const array<int, 14> next = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 10, 11, 12};
        int i = 9;
        while(true){
            for(; i != -1; i = next[i]){
                if(i == 1){
                    bool flag = 0;
                    for(int i = 13; i > 9; i--) if(a.cards[i]){
                        a.add(i);
                        if(isValid(a)){
                            q.push(a);
                            flag = 1;
                            a.prev();
                            break;
                        }
                        a.prev();
                    }
                    if(a.cards[1]){
                        a.add(1);
                        if(isValid(a)){
                            if(flag) a.last = a.v.size();
                            q.push(a);
                            flag = 1;
                        }
                    }
                    if(flag) return;
                }
                else if(a.cards[i]){
                    a.add(i);
                    if(isValid(a)){
                        q.push(a);
                        return;
                    }
                    a.prev();
                }
            }
            if(a.v.empty()) return;
            i = next[a.v.back()];
            a.prev();
            if(a.v.size() < a.last) return;
        }
    };
    while(q.size()){
        auto &a = q.top();
        push_next(a);
        if(a.n == 0 && miller_rabin_test(cpp_int(a.s), 50)){
            cout << a.s << endl;
            if(!--k) return;
        }
        q.pop();
    }
}


// 使い方

int main(){
    array<int, 14> cards;
    
    cards.fill(0);
    cards[1] = cards[11] = cards[13] = 1;
    PermSearch(cards, -1);
    // 11311
    // 11131
    // 11113
    
    cards.fill(0);
    cards[1] = cards[9] = cards[13] = 1;
    cards[7] = 2;
    PermSearch(cards, 3);
    // 971713
    // 971371
    // 917713
    
    cards.fill(4);
    cards[0] = 0;
    cards[12] = cards[13] = 5;
    PermSearch(cards);
    // 999988887777666655554444333322221313131313121212121211111111101110110101
}
