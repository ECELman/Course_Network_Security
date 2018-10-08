// 超有創意之百慕達加解密系統 - 解密
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cstring>
#include <cmath>
#include <utility>
using namespace std;

const int N = 105;
const int INDEX[N] = {
     91,  78,  66,  92,  55,  79,  45,  67,  93,  36,  56,  80,  28,  46,  68,
     94,  21,  37,  57,  81,  15,  29,  47,  69,  95,  10,  22,  38,  58,  82,
      6,  16,  30,  48,  70,  96,   3,  11,  23,  39,  59,  83,   1,   7,  17,
     31,  49,  71,  97,   0,   4,  12,  24,  40,  60,  84,   2,   8,  18,  32, 
     50,  72,  98,   5,  13,  25,  41,  61,  85,   9,  19,  33,  51,  73,  99,
     14,  26,  42,  62,  86,  20,  34,  52,  74, 100,  27,  43,  63,  87,  35,
     53,  75, 101,  44,  64,  88,  54,  76, 102,  65,  89,  77, 103,  90, 104};

/*
    建表 如下:
    eng[a] = 0 , ... , eng[z]=25
    num[0] = a , ... , num[25]=z
*/
map<char, int> eng;
map<int, char> num;
void init(){
    for(char i='a'; i<='z'; i++)
        eng[i] = i-'a';
    for(int i=0; i<26; i++)
        num[i] = 'a'+i;
}

// sort compare
bool comp(const pair<int, char> &a, const pair<int, char> &b){
    return a.first < b.first;
}
// Transposition ciphers 換位
string de_transpos(string E){
    int Elen = E.length();
    string ret = "";
    /*
        把三角形表會用到的位置依序存起來
        e.g. E = "lpaep", v = {3,1,0,4,2}

        利用pair<int, char>把 E 和 v 綁在一起
        {l,3}, {p,1}, {a,0}, {e,4}, {p,2}
        然後針對數字(second)排序
        {a,0}, {p,1}, {p,2}, {l,3}, {e,4}
        最後存進ret
    */
    vector<pair<int, char> > v;
    for(int i=0, j=0; i<N; i++){
        if(INDEX[i] < Elen)
            v.push_back(make_pair(INDEX[i], E[j++]));
    }
    sort(v.begin(), v.end(), comp);
    for(int i=0; i<Elen; i++)
        ret += v[i].second;

    return ret;
}

// 同加密換位 en_transpos, 直向存取三角形table
inline string transpos(string s){
    int len = s.length();  
    string ret = "";

    vector<int> v;
    for(int i=0; i<N; i++){
        if(INDEX[i] < len)
            v.push_back(INDEX[i]);
    }
    for(int i=0; i<len; i++)
        ret += s[ v[i] ];

    return ret;
}

// Substitution ciphers 代換
string de_substitu(string E, string K){
    int Elen = E.length(), Klen = K.length();   // Mlen:明文長度, Klen:金鑰長度
    string ret = "";    // 代換結果

    string table = K;
    int idx = 0;        // 目前解密位置
    for(; idx<Klen && idx<Elen; idx++){
        // 因為 eng[a]=0 若實際進行減法等於"位移0項", 所以要-1
        // 另外避免存取負值index, 必須另外+26
        char r = num[ (eng[E[idx]] - eng[K[idx]] - 1 + 26)%26 ];
        table += E[idx];        // 和加密作法不同, table要新增的是密文
        ret += r;
    }
    while(idx < Elen){
        string tmp = transpos(table);
        for(int j=0; idx<Elen && j<tmp.length(); idx++, j++){
            char r = num[ (eng[E[idx]] - eng[tmp[j]] - 1 + 26)%26 ];
            table += E[idx];    // 和加密作法不同, table要新增的是密文
            ret += r;
        }
    }
    return ret;
}

string decode(string E, string K){
    return de_substitu( de_transpos( de_substitu(E,K) ), K);
}

int main()
{
    init();
    string E, K;
    while(cin >> E >> K){
        int n = ceil(E.length()/(double)N);
        string M = "";
        for(int i=0; i<n; i++){
            if(i == n-1)
                M += decode( E.substr(i*N, E.length()-i*N+1), K );
            else
                M += decode( E.substr(i*N, N), K );
        }
        cout << M << endl;
    }

    return 0;
}

/*
// draw triangle table
int idx = 0, cnt = 0, spc = 12;
for(int j=0; j<spc+1; j++)
    cout << setw(2)<<" ";
for(int i=0; i<105; i++){
    if(idx == cnt){
        cout << setw(3) << i << endl;
        cnt++;
        idx = 0;
        for(int j=0; j<spc; j++)
            cout << setw(2)<<" ";
        spc--;
    }else{
        idx++;
        cout << setw(3) << i << " ";
    }
}
                            0
                          1   2
                        3   4   5
                      6   7   8   9
                   10  11  12  13  14
                 15  16  17  18  19  20
               21  22  23  24  25  26  27
             28  29  30  31  32  33  34  35
           36  37  38  39  40  41  42  43  44
         45  46  47  48  49  50  51  52  53  54
       55  56  57  58  59  60  61  62  63  64  65
     66  67  68  69  70  71  72  73  74  75  76  77
   78  79  80  81  82  83  84  85  86  87  88  89  90
 91  92  93  94  95  96  97  98  99 100 101 102 103 104
*/