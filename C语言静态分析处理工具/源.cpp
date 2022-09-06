#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<vector>
using namespace std;
string KEYWORD[32] = {                   //1-32
    "short", "int", "double", "char", "long", 
    "float","default", "volatile", "case", "else",
    "enum", "extern","continue", "void", "goto", 
    "if", "break", "const","register", "return",
    "auto", "signed", "sizeof", "static", "struct",
    "switch", "typedef", "union", "unsigned", "for",
    "do", "while"
};
string DANGER_FUC[4] = {  //201开始 201：gets 202:strcpy
    "gets","strcpy","scanf","strcat"
};

char SEPARATER[8] = { ';',',','{','}','[',']','(',')' };  //分隔符  51-60
char OPERATOR[8] = { '+','-','*','/','>','<','=','!' };   //运算符  61-70
char FILTER[4] = { ' ','\t','\r','\n' };          //过滤符
const int dangerfucNum = 4;
const int keywordNum = 32;
const int CONSTANT = 100;      //常数值
const int IDENTIFIER_v = 101;     //标识符-变量
const int IDENTIFIER_f = 102;     //标识符-函数
const int FILTER_VALUE = 103;    //过滤字符值

//bool in_fuc = false;
int state = 0;  //状态 1:标识符出现 
                       //11:1的状态下，下个字符为‘（’，此时（11）说明标识符为函数 
                           //111:11的状态下，出现 ‘)’ 
                                //1111:111的状态下，下个字符为‘{’ ，此时（1111）说明函数为声明状态,置in_fuc=true *** 之后看其中是否有过大局部变量，或多重递归
                                //1112:111的状态下，下个字符不是 ‘{’，（可能为运算符，括号或；),此时说明函数为调用状态 *** 判断其是否为特殊函数，若是就修改，不是看in_fuc是否为true，若是，看是否为递归
                      //12:说明标识符为变量，此时看in_fuc是否为true,若是，看下个有效字符是否为[
//float last_num; //出现运算时，记录前一个数字
//string last_identifier; //记录上一个标识符，若为之后查明其为函数，则判断其是否为标记函数

FILE* fpin, * fout;
//判断是否为危险函数
int IsDangerFuc(string word) {
    for (int i = 0;i < dangerfucNum;i++) {
        if (DANGER_FUC[i] == word) {
            return i + 1;
        }
    }
    return dangerfucNum + 1;
}

/**判断是否为关键字**/ //arr
int IsKeyword(string word) {
    for (int i = 0;i < keywordNum;i++) {
        if (KEYWORD[i] == word) {
            return i + 1;
        }
    }
    return keywordNum + 1;
}


/**判断是否为分隔符**/
int IsSeparater(char ch) {
    for (int i = 0;i < 8;i++) {
        if (SEPARATER[i] == ch) {
            return 50 + i + 1;
        }
    }
    return 0;
}

/**判断是否为运算符**/
int IsOperator(char ch) {
    for (int i = 0;i < 8;i++) {
        if (OPERATOR[i] == ch) {
            return 60 + i + 1;
        }
    }
    return 0;
}
/**判断是否为过滤符**/
bool IsFilter(char ch) {
    for (int i = 0;i < 4;i++) {
        if (FILTER[i] == ch) {
            return true;
        }
    }
    return false;
}
/**判断是否为大写字母**/
bool IsUpLetter(char ch) {
    if (ch >= 'A' && ch <= 'Z') return true;
    return false;
}
/**判断是否为小写字母**/
bool IsLowLetter(char ch) {
    if (ch >= 'a' && ch <= 'z') return true;
    return false;
}
/**判断是否为数字**/
bool IsDigit(char ch) {
    if (ch >= '0' && ch <= '9') return true;
    return false;
}
/**返回每个字的值**/
template <class T>
int value(T* a, int n, T str) {
    for (int i = 0;i < n;i++) {
        if (a[i] == str) return i + 1;
    }
    return -1;
}

bool fuc_state(FILE* fpin) { //判断函数状态，声明or调用 格子退回
    int left = 1;
    int right = 0;
    char ch;

    int posi = 0;
    while (left != right) {
        ch = fgetc(fpin);
        posi++;
        if (ch == '(')
            left++;
        else if (ch == ')')
            right++;
    }
    //右括号结束，看下一个字符
    ch = fgetc(fpin);
    posi++;
    while (IsFilter(ch)) {
        ch = fgetc(fpin);
        posi++;
    }
    fseek(fpin, -posi, SEEK_CUR); //格子退回：函数名到 { 之间的内容继续处理。 
    if (ch == '{') {
        return true;
    } 
    return false;
}

int deal_formula(string formula) {  // 传入的是无过滤符的算式字符串，格子不退回
    int len = formula.size();
    string arr = "";
    for (int i = 0;i < len;i++) {
        if (formula[i] == '(') {         
            i++;
            while (formula[i] != '(' && formula[i] != ')') {

            }
        }
        arr += formula[i];

    }
    return 300;
}
int deal_nokh_formula(string formula) { //无括号无小数点 1+2*3-4/5
    int len = formula.size();
    string arr = "";
    vector<int> number;
    vector<char> symbol;

    for (int i = 0;i < len;i++) {  //处理过后，只剩加减法
        if (IsDigit(formula[i]) && i != len - 1) {
            arr += formula[i];          
        }
        else { //压入数，并看是否是乘除运算
            if (i == len - 1) {  //到结尾，说明formula[i]是数字
                arr += formula[i];           
            }
            number.push_back(atoi(arr.c_str())); //压入数
           
            if (!symbol.empty()&&(symbol.back() == '*' || symbol.back() == '/')) {
                int a, b;
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
                symbol.push_back(formula[i]);
            }
         
            arr = "";
        }
    }
    int sum = number.front();
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

/**词法分析**/
void analyse(FILE* fpin) {
    int arr_num = 0; //关键字数字
    int ch_num = 0; //字符数字
    int dangerf_num = 0; //危险函数数字

    char ch = ' ';
    string arr = "";  //保存一个关键字或标识符
    string filter_str = ""; //保存一段过滤符

    bool in_fuc = false; //是否在函数中
    string in_fuc_name = ""; //当前所在函数名字，用于判断是否有递归
    int left_kh_num = 0; //大括号，检测函数是否结束
    int right_kh_num = 0; 

    bool in_state = false; //是否在char int等声明状态，到出现;结束。 用于记录可能出现的指针名字
    bool in_state_p = false;
    vector<string> pointer;
    vector<string> pointer_pname;

    int left_xkh_num = 0;//小括号，检测
    int right_xkh_num = 0;

    string last_state;//记录上个声明类型，int char 等等... 用于把栈内容转到堆上时，字符串后添加声明：int a[2000]-->int *a=new int[2000]
    vector<string> shift_var_name; //记录当前函数所有被转移到堆上的变量，用于return前将其delete
    bool fuc_has_ret = false;
    while ((ch = fgetc(fpin)) != EOF) {
        arr = "";
        filter_str = "";

        if (IsFilter(ch)) {
            fprintf(fout, "%c", ch);
        }       //判断是否为过滤符

        else if (IsLowLetter(ch)) {    //判断是否为关键字
            while (IsUpLetter(ch) || IsLowLetter(ch) || ch == '_'|| IsDigit(ch)) {  //大小写，数字，或_则继续
                arr += ch;
                ch = fgetc(fpin);
            }
            //到下个不为过滤符的字母，得到整个token
            fseek(fpin,-1L,SEEK_CUR); //退回一格，此时得到的为一串小写字母
            arr_num = IsKeyword(arr);

            if (arr_num <= keywordNum) {  //标识符为关键字
                //1 cout << arr_num << " " << arr << " 关键字" << endl;            //数字注释，表示注释掉屏幕上非关键信息,只保留程序修改内容
                
                if (arr_num <= 6) {  //记录声明类型int char..  用于后面栈移堆
                    last_state = arr.c_str();
                    in_state = true;
                }
              
                else if (arr_num == 20) { //return,在写入return之前先delete所有堆上变量
                    fuc_has_ret = true; //函数有return
                    if (shift_var_name.size() >= 1) {   //todo因为格式问题写的很复杂，且为处理没return时的delete
                        fprintf(fout, "%s", "delete[]");
                        fprintf(fout, "%s", shift_var_name[0].c_str());
                        fprintf(fout, "%c\n", ';');
                        for (int i = 1;i < shift_var_name.size();i++) {
                            fprintf(fout, "%s", "    delete[]");
                            fprintf(fout, "%s", shift_var_name[i].c_str());
                            fprintf(fout, "%c\n", ';');
                        }
                        shift_var_name.clear();
                        fprintf(fout, "%s", "    ");
                    }                 
                }               
                fprintf(fout, "%s", arr.c_str());
            }
            else //标识符为变量或函数 或指针
            {
                ch = fgetc(fpin); //持续到下个不为过滤符的字母
                while (IsFilter(ch)) {
                    filter_str += ch;
                    ch = fgetc(fpin);
                }
                //cout << endl << "当前不为过滤符的字符为：" << ch << endl;
                if (ch == '(') {  //标识符为函数
                    //state = 11; 
                    dangerf_num = IsDangerFuc(arr);
                    if (dangerf_num <= dangerfucNum) { //若是危险函数，必为调用 1:gets 2:strcpy
                        cout << IDENTIFIER_v << " " << arr << " 标识符-函数(危险）" ; //危险函数必为调用，所以后面是括号

                        //arr += ch; //函数名字符串加上（ ，即直接处理到函数后面的）
                        left_xkh_num++;
                        string identifier_inkh_name = ""; //记录危险函数内变量名字

                        switch (dangerf_num) {
                            case 1:  //gets(s)-->fgets(s,sizeof(s),stdin)
                            {                               
                                arr = "fgets("; //将前面的arr="gets("  可能有空格，消除空格，改为新函数
                                while(left_xkh_num != right_xkh_num) {
                                    ch = fgetc(fpin);
                                    identifier_inkh_name += ch;
                                    if (ch == '(') {
                                        left_xkh_num++;
                                    }
                                    else if (ch == ')') {
                                        right_xkh_num++;
                                    }
                                }
                                //到此arr="fgets("
                                // identifier_inkh_name = "s)"
                                arr += identifier_inkh_name;
                                identifier_inkh_name.erase(identifier_inkh_name.length() - 1, 1);
                                
                                for (int i = 0;i < pointer.size();i++) {
                                    if (pointer[i].compare(identifier_inkh_name) == 0) {
                                        identifier_inkh_name = pointer_pname[i];
                                    }
                                }

                                arr.insert(arr.length() - 1, ",sizeof("+identifier_inkh_name+"),stdin");
                                fseek(fpin, 1L, SEEK_CUR); //前进一格，因为已经把）都处理完了，之后还会退回一格，所以这里保持平衡
                                left_xkh_num = 0;
                                right_xkh_num = 0;

                                break;
                            }
                            case 2:  //strcpy(s,s1)-->strcpy_s(s,sizeof(s),s1)
                            {
                                arr = "strcpy_s("; 
                                while ((ch = fgetc(fpin)) != ',') {
                                    identifier_inkh_name += ch;
                                    arr += ch;
                                    if (ch == '(') {
                                        left_xkh_num++;
                                    }
                                    else if (ch == ')') {
                                        right_xkh_num++;
                                    }
                                }
                                arr += ch; //arr加上逗号
                                while (left_xkh_num != right_xkh_num) {
                                    ch = fgetc(fpin);        
                                    arr += ch;
                                    if (ch == '(') {
                                        left_xkh_num++;
                                    }
                                    else if (ch == ')') {
                                        right_xkh_num++;
                                    }
                                }
                                //到此arr="strcpy_s(s,s1)"
                                // identifier_inkh_name = "s"
                              
                                for (int i = 0;i < pointer.size();i++) {
                                    if (pointer[i].compare(identifier_inkh_name) == 0) {
                                        identifier_inkh_name = pointer_pname[i];
                                    }
                                }
                                arr.insert(arr.find(",") + 1, "sizeof(" + identifier_inkh_name + "),");
                                fseek(fpin, 1L, SEEK_CUR); //前进一格，因为已经把）都处理完了，之后还会退回一格，所以这里保持平衡
                                left_xkh_num = 0;
                                right_xkh_num = 0;

                                break;
                            }
                            case 3:  //scanf("%s",&s)-->strcpy_s("%s",&s,sizeof(s))
                            {
                                arr = "scanf_s(";
                                while ((ch = fgetc(fpin)) != '&') {                                   
                                    arr += ch;
                                    if (ch == '(') {
                                        left_xkh_num++;
                                    }
                                    else if (ch == ')') {
                                        right_xkh_num++;
                                    }
                                }
                                arr += ch; //arr加上&
                              
                                while (left_xkh_num != right_xkh_num) {                                  
                                    ch = fgetc(fpin);
                                    identifier_inkh_name += ch; //写在ch = fgetc(fpin)后，因为不加&
                                    arr += ch;
                                    if (ch == '(') {
                                        left_xkh_num++;
                                    }
                                    else if (ch == ')') {
                                        right_xkh_num++;
                                    }
                                }
                                //到此arr="scanf_s("s%",&s)"
                                // identifier_inkh_name = "s)"
                                arr.erase(arr.length() - 1, 1); //删去右括号
                                identifier_inkh_name.erase(identifier_inkh_name.length() - 1, 1); //删去右括号     

                                for (int i = 0;i < pointer.size();i++) {
                                    if (pointer[i].compare(identifier_inkh_name) == 0) {
                                        identifier_inkh_name = pointer_pname[i];
                                    }
                                }

                                arr += ",sizeof(" + identifier_inkh_name + "))";
                                fseek(fpin, 1L, SEEK_CUR); //前进一格，因为已经把）都处理完了，之后还会退回一格，所以这里保持平衡
                                left_xkh_num = 0;
                                right_xkh_num = 0;

                                break;
                            }
                            case 4:  //strcpy(s,s1)-->strncpy(s,s1,sizeof(s)-length(s)-1)  //这里用length因为
                            {
                                arr = "strcat_s(";
                                while ((ch = fgetc(fpin)) != ',') {
                                    identifier_inkh_name += ch;
                                    arr += ch;
                                    if (ch == '(') {
                                        left_xkh_num++;
                                    }
                                    else if (ch == ')') {
                                        right_xkh_num++;
                                    }
                                }
                                arr += ch; //arr加上逗号
                                while (left_xkh_num != right_xkh_num) {
                                    ch = fgetc(fpin);
                                    arr += ch;
                                    if (ch == '(') {
                                        left_xkh_num++;
                                    }
                                    else if (ch == ')') {
                                        right_xkh_num++;
                                    }
                                }
                                //到此arr="strcat_s(s,s1)"
                                // identifier_inkh_name = "s"                              
                                //arr.erase(arr.length() - 1, 1); //删去右括号                              

                                for (int i = 0;i < pointer.size();i++) {
                                    if (pointer[i].compare(identifier_inkh_name) == 0) {
                                        identifier_inkh_name = pointer_pname[i];
                                    }
                                }
                                arr.insert(arr.find(",") + 1, "sizeof(" + identifier_inkh_name + ") - strlen(" + identifier_inkh_name + ") - 1,");
                                //arr += ",sizeof(" + identifier_inkh_name + ") - strlen(" + identifier_inkh_name + ") - 1)";
                                fseek(fpin, 1L, SEEK_CUR); //前进一格，因为已经把）都处理完了，之后还会退回一格，所以这里保持平衡
                                left_xkh_num = 0;
                                right_xkh_num = 0;

                                break;
                            }
                        }
                        cout << "  ; 已修改为 :" << arr << endl;
                        fprintf(fout, "%s", arr.c_str());//arr此时为函数名+其后括号内内容直到）
                    }
                    else {   //若是其他函数，判断其为声明或是调用
                        if (fuc_state(fpin)) {  //函数声明，程序变为函数内状态，此时要检查变量空间大小。并记录函数名，为之后检查内部是否有递归做准备。
                            cout << IDENTIFIER_v << " " << arr << " 标识符-函数(安全）-声明状态" << endl; //函数
                           // state=
                            //if (arr.compare("main") != 0) {
                            in_fuc = true;  //进入函数状态 
                            in_fuc_name += arr; //记录此时函数名字，用于检查是否有递归
                            fuc_has_ret = false; //函数开始，先设定无return
                            cout << endl << "进入函数:" << in_fuc_name << endl << endl;
                            fprintf(fout, "%s", arr.c_str());
                            //}
                        }
                        else {  //函数调用，检查是否存在递归
                            cout << IDENTIFIER_v << " " << arr << " 标识符-函数(安全）-调用状态 ;"; //函数
                            if (in_fuc) {
                                if (in_fuc_name.compare(arr) == 0) {
                                    cout << " 您的程序中使用了递归函数，请注意检查是否递归次数过多。";
                                    arr += ch;
                                    while ((ch = fgetc(fpin)) != ';') {
                                        arr += ch;
                                    }
                                    arr += ch;
                                    fprintf(fout, "%s", arr.c_str());
                                    fprintf(fout, "%s", " //您的程序中使用了递归函数，请注意检查是否递归次数过多。");
                                    fseek(fpin, 1L, SEEK_CUR); //退回一格
                                }
                                else {
                                    fprintf(fout, "%s", arr.c_str());
                                }
                            }
                            cout << endl;
                            //有未知bug,需要此行无其他需处理信息。                         
                        }                                         
                    }
                }
                else { //标识符为变量，先检查是否在栈上（标识符下个字符为[），若是，看[]内内容是否大于1024，若是，则分配其到堆上。
                    // 2不知为何的标识符，正常变量等 cout << IDENTIFIER_v << " " << arr << " 标识符-变量" << endl; //变量
                    //此时ch不为（，大概率为‘，’栈上小内存 ‘=’站上小内存，或堆上 ‘[’栈上，可能大内存，处理
                    if (ch == '[' && in_fuc) {  //在栈上 直接处理到]
                        //arr += "";                   

                        string formula = "";
                        int num = 0;
                        while ((ch = fgetc(fpin)) != ']') {
                            if (!IsFilter(ch)) {
                                formula += ch;
                            }
                        }
                        //fseek(fpin, -1L, SEEK_CUR); //退回一格,处理]
                        num = deal_nokh_formula(formula); //传入处理字符串

                        cout << arr << " 算术公式,其值为:" << num <<endl;
                        if (num > 1024) {  //越界，连同]一起处理，分配至堆上,并记录至shift_var_name;
                            shift_var_name.push_back(arr);

                            arr.insert(0, "*"); //加入*符号
                            arr += "= new ";
                            arr += last_state;
                            arr += "[";
                            arr += formula;
                            arr += "]";
                           
                            cout << "变量所占空间过大，已分配到堆上！" << endl;

                        }
                        else {  //不越界,还是原来的变量+[公式]
                            arr += "[";
                            arr += formula;
                            arr += "]";
                        }
                        fprintf(fout, "%s", arr.c_str());
                        continue;
                    }
                    else if (in_state_p) { //该变量为指针，直接处理到，或;
                        pointer.push_back(arr);
                        string pname = "";
                        bool pname_start = false;
                        while (ch != ',' && ch != ';') {
                            arr += ch;          
                            if (pname_start) {
                                if(IsUpLetter(ch) || IsLowLetter(ch) || ch == '_' || IsDigit(ch))
                                pname += ch;
                                if (IsSeparater(ch))
                                    pname_start = false;
                            }
                            if (ch == '=') {
                                pname_start = true;
                            }
                           
                            ch = fgetc(fpin);
                        }
                        pointer_pname.push_back(pname);
                        in_state_p = false;

                        fprintf(fout, "%s", arr.c_str());
                    }
                    else { //正常变量,只输出变量名，[和其中算式 之后正常当字符处理
                       fprintf(fout, "%s", arr.c_str());
                    }
                }
                fprintf(fout, "%s", filter_str.c_str()); //写入中间的过滤符串，正常情况下，之后退回一格，开始处理过滤符后的（等
                fseek(fpin, -1L, SEEK_CUR); //退回一格
            }
        }

        else if (IsDigit(ch)) {      //判断是否为数字
            while (IsDigit(ch) || (ch == '.' && IsDigit(fgetc(fpin)))) {
                arr += ch;
                ch = fgetc(fpin);
            }
            fseek(fpin, -1L, SEEK_CUR); //往前退一格是因为第二种情况ch=.但是下个不为数字
            //3整型数 cout << CONSTANT << " " << arr << " 整形数" << endl;
            fprintf(fout, "%s", arr.c_str());
        }

        else if (IsUpLetter(ch) || IsLowLetter(ch) || ch == '_') {  //判断标志符，开头为大写    /TODO：后面
            while (IsUpLetter(ch) || IsLowLetter(ch) || ch == '_' || IsDigit(ch)) {
                arr += ch;
                ch = fgetc(fpin);
            }
            fseek(fpin, -1L, SEEK_CUR);
            printf("%3d  ", CONSTANT);
            //4 标识符 cout << arr << " 标识符" << endl;
            fprintf(fout, "%s", arr.c_str());
        }      

        else if ((ch_num = IsOperator(ch)) != 0) {
            arr += ch;
            //5 运算符 cout << ch_num << " " << arr << " 运算符" << endl;
            if (ch_num == 63) { //*
                if (in_state) {
                    in_state_p = true;
                 
                }
            }
            fprintf(fout, "%s", arr.c_str());
        }
        else if ((ch_num = IsSeparater(ch)) != 0) {
            arr += ch;
            //6 分隔符 cout << ch_num << " " << arr << " 分隔符" << endl;
            //fprintf(fout, "%s", arr.c_str());
            if (in_fuc) {  //若处于函数内部
                if (ch_num == 53) {
                    left_kh_num++;  //函数内，左大括号+1
                }
                else if (ch_num == 54) {
                    right_kh_num++;   //函数内，右大括号 + 1
                    if (right_kh_num == left_kh_num) {  //左大括号==右大括号数目，则函数结束
                        in_fuc = false;
                        left_kh_num = 0;
                        right_kh_num = 0;
                        cout << endl << "函数结束:" << in_fuc_name << endl << endl;
                        in_fuc_name = "";

                        if (!fuc_has_ret) { //函数无return,若有需要delete的，在}前添加代码。
                            for (auto x: shift_var_name) {
                                fprintf(fout, "%s", "    delete[]");
                                fprintf(fout, "%s", x.c_str());
                                fprintf(fout, "%c\n", ';');
                            }
                            shift_var_name.clear();                    
                        }

                    }
                }            
            }
            if (ch_num == 51) {
                if (in_state) {
                    in_state = false;
                    in_state_p = false;
                }
            }

            fprintf(fout, "%s", arr.c_str());  //写在最后，令需要delete的在函数结束的}之前
        }
        else {
            arr += ch;
            //7 无法识别字符 cout << ch << " 无法识别的字符！" << endl;
            fprintf(fout, "%s", arr.c_str());
        }
    }

}
int main()
{
    char inFile[40] = "code.txt"; //要处理程序文件的路径
    fout = fopen("result.txt", "w"); //处理后的文件，每次执行更新
    while (true) {
        // cin >> inFile;
        if ((fpin = fopen(inFile, "r")) != NULL)
            break;
        else {
            cout << "文件名错误！" << endl;
            cout << "请输入源文件名（包括路径和后缀）:";
        }

    }
    cout << "------词法分析如下------" << endl;
    analyse(fpin);
    return 0;
}


