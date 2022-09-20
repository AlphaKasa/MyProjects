#define _CRT_SECURE_NO_DEPRECATE 
#include<stdio.h>
#include <iostream>
#include<vector>
#include <iomanip>
#include<sstream>

using namespace std;

char digits[11] = { '0','1','2','3','4','5','6','7','8','9','.' };
char operators[4] = { '+','-','*','/' };

float Formula(string s);

bool IsDigit(char c) { //判断字符是否为数字
    for (int i = 0;i < 11;i++) {
        if (c == digits[i])
            return true;
    }
    return false;
}

bool IsOperators(char c) {    //判断字符是否为操作符
    for (int i = 0;i < 4;i++) {
        if (c == operators[i])
            return true;
    }
    return false;
}
float DealBrackets(string s) {   //处理掉算式里的所有括号
    int len = s.size();
    char noBracket[100] = "";
    char inBracket[10][100] = {""};
   
    int numBracket = 0;

    int i = 0;
    while (i < len) {

        if (s[i] == '(') {
            //leftBracket++;
            numBracket++;
        }
        else if (s[i] == ')') {
            // rightBracket--;
            sprintf(inBracket[numBracket - 1] + strlen(inBracket[numBracket - 1]), "%.2f", Formula(inBracket[numBracket]));
            numBracket--;
        }
        else {
            int l = strlen(inBracket[numBracket]);
            inBracket[numBracket][l] = s[i];
        }
        i++;
    }
    return Formula(inBracket[0]);    
}

float Formula(string s) {   //计算最终结果，传入的是不带括号的算式字符串
    int len = s.size();
    string arr = "";
    vector<float> number;
    vector<char> symbol;

    for (int i = 0;i < len;i++) {  //处理过后，只剩加减法
        if (s[i] == ' ') { //过滤空格符

        }
        else if (IsDigit(s[i]) && i != len - 1) {
            arr += s[i];
        }
       
        else { //压入数，并看是否是乘除运算
            if (i == len - 1) {  //到结尾，说明formula[i]是数字
                arr += s[i];
            }
            number.push_back(atof(arr.c_str())); //压入数

            if (!symbol.empty() && (symbol.back() == '*' || symbol.back() == '/')) {
                float a, b;
                a = number.back();
                number.pop_back();
                b = number.back();
                number.pop_back();
                if (symbol.back() == '*') {
                    number.push_back(a * b);
                }
                else if (symbol.back() == '/') {
                    number.push_back(b / a);
                }
                symbol.pop_back();
            }
            

            if (i != len - 1) { //不到结尾，说明formula[i]是字符
                symbol.push_back(s[i]);
            }

            arr = "";
        }
    }
    float sum = number.front();
    while (!symbol.empty()) {
        int a;
        a = number.back();
        if (symbol.back() == '+') {
            sum += a;
        }
        else if (symbol.back() == '-') {
            sum -= a;
        }
        number.pop_back();
        symbol.pop_back();

    }
    return sum;
    
}

int main()
{
    string s = "1/0.5+((2*5.5+1.5)-3.5)";
    cout << setprecision(2) << fixed << DealBrackets(s) << endl;
}

