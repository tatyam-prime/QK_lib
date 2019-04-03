#include <iostream>
#include <numeric>
#include <array>
#include <string>
#include <queue>
#include <unordered_set>
#include <boost/multiprecision/miller_rabin.hpp>
using namespace std;
using namespace boost::multiprecision;


const array<int, 14> nxt = {-1, 13, 1, 2, 3, 4, 5, 6, 7, 8, 0, 10, 11, 12};
const array<string, 14> to_s = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"};
struct SearchState{
    array<int, 14> cards;
    string s = "";
    int n, last, oned, eleven = 0;
    SearchState(array<int, 14> _cards) : cards(_cards){
        last = -1;
        oned = accumulate(cards.begin(), cards.begin() + 10 , 0);
        n = accumulate(cards.begin() + 10 , cards.end(), oned);
    }
    SearchState add(int a){
        auto ans = *this;
        ans.cards[a]--;
        ans.n--;
        ans.s += to_s[a];
        ans.last = a;
        if(a > 9) ans.eleven = (ans.eleven + a) % 11;
        else{
            ans.oned--;
            ans.eleven = (a - ans.eleven + 11) % 11;
        }
        return ans;
    }
    SearchState back(){
        auto ans = *this;
        ans.cards[last]++;
        ans.n++;
        ans.s.pop_back();
        ans.last = -1;
        if(last > 9){
            ans.s.pop_back();
            ans.eleven = (ans.eleven - last + 11) % 11;
        }
        else{
            ans.oned++;
            ans.eleven = (last - ans.eleven + 11) % 11;
        }
        return ans;
    }
    bool operator < (const SearchState &rhs) const {
        auto a = s.begin(), b = rhs.s.begin();
        while(1){
            if(b == rhs.s.end()) return 1;
            if(a == s.end()) return 0;
            if(*a != *b) return *a < *b;
            a++;
            b++;
        }
    }
    bool operator == (const SearchState &rhs) const {
        return s == rhs.s && cards == rhs.cards;
    }
};
void PermSearch(array<int, 14> cards, int k = 1){
    const function<uint32_t(SearchState)> hasher = [](const SearchState &a){
        string s = a.s;
        for(auto &i: a.cards) s += char(i);
        return hash<string>()(s);
    };
    unordered_set<SearchState, function<uint32_t(SearchState)>> used(1 << 12, hasher);
    priority_queue<SearchState> q;
    for(int i = 9; i >= 0; i = nxt[i]) if(cards[i]) {
        q.push(SearchState(cards).add(i));
        break;
    }
    while(q.size()){
        auto top = q.top();
        q.pop();
        for(int i = nxt[top.last]; i >= 0; i = nxt[i]) if(top.cards[i]) {
            q.push(top.back().add(i));
            break;
        }
        if(used.count(top)) continue;
        used.insert(top);
        if(!top.n){
            if(miller_rabin_test(cpp_int(top.s), 30)){
                cout << top.s << endl;
                if(!--k) return;
            }
            continue;
        }
        if(!top.oned){
            if((top.eleven - top.cards[10] + top.cards[12] + top.cards[13] + top.cards[13]) % 11 == 0) continue;
            top.oned = 1;
        }
        for(int i = 9; i >= 0; i = nxt[i]) if(top.cards[i]) {
            q.push(top.add(i));
            break;
        }
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
