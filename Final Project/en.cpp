// 超有創意之百慕達加解密系統 - 加密
#include <iostream>
#include <iomanip>      // setw
#include <map>          // map
#include <vector>       // vector
#include <cstring>      // s.length
#include <cmath>        // ceil
#include <utility>      // pair
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

// Transposition ciphers 換位
string en_transpos(string M){
    int Mlen = M.length();  // 密文長度
    string ret = "";        // 換位結果

    /*
        把三角形表會用到的位置依序存起來
        e.g. M = "apple", v = {3,1,0,4,2}
    */
    vector<int> v;          
    for(int i=0; i<N; i++){
        if(INDEX[i] < Mlen)
            v.push_back(INDEX[i]);
    }

    /*
        再根據 v 裡面的index把密文存進 ret
        e.g. M = "apple", v = {3,1,0,4,2}, ret = {l,p,a,e,p}
    */
    for(int i=0; i<Mlen; i++)
        ret += M[ v[i] ];

    return ret;
}

// Substitution ciphers 代換
string en_substitu(string M, string K){
    int Mlen = M.length(), Klen = K.length();   // Mlen:密文長度, Klen:金鑰長度
    string ret = "";    // 代換結果
    string table = K;   // 如下圖示
    int idx = 0;    // 目前加密位置
    /*
        e.g. M = "apple", K = "key"
        先針對已知的 key 進行加密, 如下:
            a p p l e    << M
        +)  k e y        << table
        ---------------
            l u o        << ret
    */
    for(; idx<Klen && idx<Mlen; idx++){
        // 因為 eng[a]=0 若實際進行加法等於"位移0項", 所以要+1
        char r = num[ (eng[M[idx]] + eng[K[idx]] + 1)%26 ];
        table += r;
        ret += r;
    }

    /*
        若尚未完成加密, 把剛剛加密的 l u o 放入 table
        此時的table:
            k
          e   y
        l   u   o
        依照 en_transpos(table) 可以得到 tmp = "lekuyo" 再繼續加密~
            a p p l e    << M
        +)  k e y l e    << tmp
        ---------------
            l u o x j    << ret
    */
    while(idx < Mlen){
        string tmp = en_transpos(table);
        for(int j=0; idx<Mlen && j<tmp.length(); idx++, j++){
            char r = num[ (eng[M[idx]] + eng[tmp[j]] + 1)%26 ];
            table += r;
            ret += r;
        }
    }

    return ret;
}

// 超有創意之百慕達加密法
string encode(string M, string K){
    // 使用範例: 代換-換位-代換
    return en_substitu( en_transpos( en_substitu(M,K) ), K);
/*
    // 下面就只是把"代換", "換位"的執行結果顯示出來方便debug
    string E = "";  // 加密結果
    E = en_transpos(M);
    cout << E << endl;
    E = en_substitu(M, K);
    cout << E << endl;
    return en_substitu( en_transpos( en_substitu(M,K) ), K);
*/
}

int main()
{
    init();          // 建表( 字母<->數字 )
    string M, K;     // M:密文, K:金鑰
    while(cin >> M >> K){

        /*
            請參考code至底的三角形表, 可知加密最大長度到105位元
            考量到密文長度可能超過105, 必須將密文進行切割
            e.g. 密文長度為211, 則拆解成 105+105+1 分別進行加密
        
            如果不考慮超過長度超過105的密文, 可以直接用encode(M, K)加密
        */
        int n = ceil(M.length()/(double)N); // 211 = 105+105+1 共加密3次
        string E = "";    // 把所有加密結果 加起來
        for(int i=0; i<n; i++){
            if(i == n-1)
                E += encode( M.substr(i*N, M.length()-i*N+1), K );
            else
                E += encode( M.substr(i*N, N), K );
        }
        cout << E << endl;

        // 不考慮密文長度的話...
        //cout << encode(M, K) << endl;
    }

    return 0;
}

/*
// draw triangle table
int main(){
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
    return 0;
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