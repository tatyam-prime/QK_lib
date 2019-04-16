#include <bits/stdc++.h>
#include <boost/multiprecision/miller_rabin.hpp>
using namespace std;
using namespace boost::multiprecision;


const int T = 10, J = 11, Q = 12, K = 13;
const vector<char> to_c = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
array<int, 10> card = {1,2,3,6,3,7,J,J,4,7}; // 途中から再開
vector<int> dig(11);
vector<uint128_t> tens(23), front(11), back(11);
inline uint128_t connect(uint128_t a, int b){
    if(b < 10) return a * 10 + b;
    return a * 100 + b;
}
int main(){
    bool flag = 1;
    tens[0] = 1;
    for(int i = 1; i < 23; i++) tens[i] = tens[i - 1] * 10;
    int three = accumulate(card.begin(), card.end(), 0) % 3;
    constexpr int i = 0;
    while(card[i] < 14){
        front[i + 1] = connect(front[i], card[i]);
        dig[i + 1] = dig[i] + 1 + (card[i] > 9);
        constexpr int i = 1;
        while(card[i] < 14){
            front[i + 1] = connect(front[i], card[i]);
            dig[i + 1] = dig[i] + 1 + (card[i] > 9);
            constexpr int i = 2;
            while(card[i] < 14){
                front[i + 1] = connect(front[i], card[i]);
                dig[i + 1] = dig[i] + 1 + (card[i] > 9);
                constexpr int i = 3;
                while(card[i] < 14){
                    front[i + 1] = connect(front[i], card[i]);
                    dig[i + 1] = dig[i] + 1 + (card[i] > 9);
                    constexpr int i = 4;
                    while(card[i] < 14){
                        front[i + 1] = connect(front[i], card[i]);
                        dig[i + 1] = dig[i] + 1 + (card[i] > 9);
                        constexpr int i = 5;
                        while(card[i] < 14){
                            front[i + 1] = connect(front[i], card[i]);
                            dig[i + 1] = dig[i] + 1 + (card[i] > 9);
                            constexpr int i = 6;
                            while(card[i] < 14){
                                front[i + 1] = connect(front[i], card[i]);
                                dig[i + 1] = dig[i] + 1 + (card[i] > 9);
                                constexpr int i = 7;
                                while(card[i] < 14){
                                    front[i + 1] = connect(front[i], card[i]);
                                    dig[i + 1] = dig[i] + 1 + (card[i] > 9);
                                    constexpr int i = 8;
                                    while(card[i] < 14){
                                        front[i + 1] = connect(front[i], card[i]);
                                        dig[i + 1] = dig[i] + 1 + (card[i] > 9);
                                        constexpr int i = 9;
                                        while(card[i] < 14){
                                            if(!(card[i] & 1) || card[i] == 5){
                                                card[i]++;
                                                three++;
                                                if(three == 3) three = 0;
                                                continue;
                                            }
                                            front[i + 1] = connect(front[i], card[i]);
                                            dig[i + 1] = dig[i] + 1 + (card[i] > 9);
                                            for(int i = 9; i >= 0; i--) back[i] = back[i + 1] + card[i] * tens[dig[10] - dig[i + 1]];
                                            for(int i = 0; i < 10; i++) {
                                                int three2 = three, cnt = 0, except = 0;
                                                uint128_t x;
                                                for(int j = 1; j < 10; j++) {
                                                    x += tens[dig[10] - dig[i]];
                                                    three2++;
                                                    if(three2 == 3){
                                                        three2 = 0;
                                                        continue;
                                                    }
                                                    if(miller_rabin_test(front[i] * tens[dig[10] - dig[i] + 1] + x + back[i], 30)) cnt++;
                                                    else if(!except) except = j;
                                                    else goto br;
                                                }
                                                for(int j = 10; j < 14; j++) {
                                                    x += tens[dig[10] - dig[i]];
                                                    three2++;
                                                    if(three2 == 3){
                                                        three2 = 0;
                                                        continue;
                                                    }
                                                    if(miller_rabin_test(front[i] * tens[dig[10] - dig[i] + 2] + x + back[i], 30)) cnt++;
                                                    else if(!except) except = j;
                                                    else goto br;
                                                }
                                                if(cnt > 7) {
                                                    for(int j = 0; j < i; j++) cout << to_c[card[j]];
                                                    cout << 'X';
                                                    for(int j = i; j < 10; j++) cout << to_c[card[j]];
                                                    if(except) cout << '\\' << to_c[except];
                                                    if(cnt == 9) cout << '!';
                                                    cout << endl;
                                                    if(flag) flag = 0; // 再開分飛ばす
                                                    else{
                                                        ofstream file("出力ファイル", ios::app);
                                                        for(int j = 0; j < i; j++) file << to_c[card[j]];
                                                        file << 'X';
                                                        for(int j = i; j < 10; j++) file << to_c[card[j]];
                                                        if(except) file << '\\' << to_c[except];
                                                        if(cnt == 9) file << '!';
                                                        file << endl;
                                                    }
                                                }
                                            br:;
                                            }
                                            card[i]++;
                                            three++;
                                            if(three == 3) three = 0;
                                        }
                                        card[i] = 1;
                                        card[i - 1]++;
                                    }
                                    card[i] = 1;
                                    card[i - 1]++;
                                }
                                card[i] = 1;
                                card[i - 1]++;
                            }
                            card[i] = 1;
                            card[i - 1]++;
                        }
                        card[i] = 1;
                        card[i - 1]++;
                    }
                    card[i] = 1;
                    card[i - 1]++;
                    if(card[3] % 3 == 0){
                        double a = 0;
                        for(int i = 3; i >= 0; i--){
                            a += card[i] - 1;
                            a /= 13;
                        }
                        printf("%.4f%%\n", a * 100);
                    }
                }
                card[i] = 1;
                card[i - 1]++;
            }
            card[i] = 1;
            card[i - 1]++;
        }
        card[i] = 1;
        card[i - 1]++;
    }
}
