#include <bits/stdc++.h>
#include <boost/multiprecision/miller_rabin.hpp>
using namespace std;
using namespace boost::multiprecision;


const array<int, 14> nxt = {-1, 13, 1, 2, 3, 4, 5, 6, 7, 8, 0, 10, 11, 12};
const array<string, 14> to_s = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"};
struct SearchState{
    array<int, 14> cards;
    string s = "";
    vector<int> perm;
    int n, last, last2, oned, odd, eleven = 0;
    SearchState(){}
    SearchState(array<int, 14> _cards) : cards(_cards){
        last = last2 = -1;
        oned = accumulate(cards.begin(), cards.begin() + 10 , 0);
        n = accumulate(cards.begin() + 10 , cards.end(), oned);
        odd = -cards[5];
        for(int i = 1; i < 14; i += 2) odd += cards[i];
    }
    SearchState add(int a){
        auto ans = *this;
        ans.cards[a]--;
        ans.n--;
        ans.s += to_s[a];
        ans.perm.push_back(a);
        ans.last = a;
        ans.last2 = last;
        if(a > 9) ans.eleven = (ans.eleven + a) % 11;
        else{
            ans.oned--;
            ans.eleven = (a - ans.eleven + 11) % 11;
        }
        if(a & 1 && a != 5) ans.odd--;
        return ans;
    }
    SearchState change(int a){
        auto ans = *this;
        ans.cards[last]++;
        ans.s.pop_back();
        if(last > 9){
            ans.s.pop_back();
            ans.eleven = (ans.eleven - last + 11) % 11;
        }
        else{
            ans.oned++;
            ans.eleven = (last - ans.eleven + 11) % 11;
        }
        if(last & 1 && last != 5) ans.odd++;
        ans.cards[a]--;
        ans.s += to_s[a];
        ans.perm.back() = a;
        ans.last = a;
        if(a > 9) ans.eleven = (ans.eleven + a) % 11;
        else{
            ans.oned--;
            ans.eleven = (a - ans.eleven + 11) % 11;
        }
        if(a & 1 && a != 5) ans.odd--;
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
pair<bool, SearchState> PermSearch(array<int, 14> cards, int k = 1){
    if(accumulate(cards.begin(), cards.end(), 0) > 1){ // 3の倍数は素数にならない
        int sum = 0;
        for(int i = 0; i < 14; i++) sum += cards[i] * i;
        if(sum % 3 == 0) return {0, {}};
    }
    const auto hasher = [](const SearchState &a){
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
        for(int i = nxt[top.last]; i >= 0; i = nxt[i]) if(top.cards[i]){ // 最後を変更する
            if(top.last2 == 1) {
                if(i < 4 && top.cards[10 + i]) continue; // TJQKがあるなら1の後に0123はダメ
                if(i == 11) continue; // 1の後にJはダメ
            }
            q.push(top.change(i));
            break;
        }
        if(used.count(top)) continue; // 重複
        used.insert(top);
        if(!top.n){
            if(miller_rabin_test(cpp_int(top.s), 30)){ // 素数判定
                return {1, top};
            }
            continue;
        }
        if(!top.oned){
            if((top.eleven - top.cards[10] + top.cards[12] + top.cards[13] + top.cards[13]) % 11 == 0) continue; // 11の倍数
            top.oned = 1;
        }
        if(!top.odd) continue; // 2/5の倍数
        for(int i = 9; i >= 0; i = nxt[i]) if(top.cards[i]){ // 1つ繋げる
            if(top.last == 1) {
                if(i < 4 && top.cards[10 + i]) continue; // TJQKがあるなら1の後に0123はダメ
                if(i == 11) continue; // 1の後にJはダメ
            }
            q.push(top.add(i));
            break;
        }
    }
    return {0, {}};
}

vector<vector<int>> cnt = {{0}, {10, 5}, {20, 10, 5}, {30, 15, 10, 5}, {40, 20, 10, 10, 5}}; //パラメータ
const vector<char> to_c = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
int main(){
    random_device rr;
    mt19937 rand(rr());
    vector<set<pair<cpp_int, vector<int>>>> ans(5);
    for(int i = 1; i < 5; i++) for(int c2 = 0; c2 <= i; c2++){
        int fail = 100;
        int c1 = i - c2;
        while(fail && cnt[i][c2]){
            fail--;
            array<int, 14> cards;
            cards.fill(0);
            for(int i = 0; i < c1; i++) cards[rand() % 9 + 1]++;
            for(int i = 0; i < c2; i++) cards[rand() % 4 + 10]++;
            auto a = PermSearch(cards);
            if(a.first && ans[i].insert({cpp_int(a.second.s), a.second.perm}).second){
                fail = 10;
                cnt[i][c2]--;
            }
        }
    }
    for(auto &i : ans) for(auto &j : i){
        for(auto &k : j.second) cout << to_c[k];
        cout << endl;
    }
}


