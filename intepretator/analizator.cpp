#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <tuple>
#include <math.h>

#define MAX_SIZE_TID   100
#define MAX_STACK_INT  100
#define MAX_STACK_LEX  100
#define MAX_STACK_FLT  100
#define MAX_STACK_STR  100
#define MAX_NUM_ITER   100
#define MAX_SIZE_POLIZ 1000

using namespace std;



//------------------------------------------------------------------------------------//
//---------------------------------CLASSES AND ENUMS----------------------------------//
//------------------------------------------------------------------------------------//



enum type_of_lex {
    
    LEX_NULL,            // 0
    LEX_PROGRAM,         // 1
    
    LEX_INT,             // 2
    LEX_STRING,          // 3
    LEX_BOOL,            // 4
    LEX_FLOAT,           // 5

    LEX_IF,              // 6
    LEX_ELSE,            // 7
    LEX_WHILE,           // 8
    LEX_FOR,             // 9
    LEX_BREAK,           // 10
    LEX_GOTO,            // 11
    LEX_READ,            // 12
    LEX_WRITE,           // 13

    LEX_AND,             // 14
    LEX_OR,              // 15
    LEX_NOT,             // 16

    LEX_TRUE,            // 17
    LEX_FALSE,           // 18
    LEX_NUM,             // 19
    LEX_STR,             // 20
    LEX_FLT,             // 21

    LEX_LPAREN,          // 22 (
    LEX_RPAREN,          // 23 )
    LEX_LBRACE,          // 24 {
    LEX_RBRACE,          // 25 }
    LEX_POINT,           // 26 .
    LEX_COMMA,           // 27 ,
    LEX_COLON,           // 28 :
    LEX_SEMICOLON,       // 29 ;
    LEX_MINUS,           // 30 -
    LEX_PLUS,            // 31 +
    LEX_TIMES,           // 32 *
    LEX_SLASH,           // 33 /
    LEX_EQ,              // 34 =
    LEX_LT,              // 35 <
    LEX_GT,              // 36 >
    LEX_EEQ,             // 37 ==
    LEX_LEQ,             // 38 <=
    LEX_NEQ,             // 39 !=
    LEX_GEQ,             // 40 >=
    LEX_UMINUS,          // 41 !
    LEX_FIN,             // 42 EOF

    LEX_STRUCT,          // 43
    LEX_ELEMST,          // 44 
    LEX_ID,              // 45
    
    POLIZ_GO,            // 46
    POLIZ_FGO,           // 47
    POLIZ_BGO,           // 48
    POLIZ_LABEL,         // 49
    POLIZ_ADDRESS        // 50
    
};

template <class T, int max_size> class Stack {

    T s[max_size];
    int top;

public:
    
    Stack() { top = 0; }

    bool is_empty() { return top == 0; }
    bool if_full()  { return top == max_size; }  
    void push(T i)  { 
        if (!if_full()) {
            s[top] = i;
            top++;
        } else throw "Stack is full";
    }
    T pop() {
        if (!is_empty()) {
            --top;
            return s[top];
        } else throw "Stack is empty";
    }
 
};



class Lex {

    type_of_lex        t_lex;
    int                v_lex;
    float              f_lex;
    string             s_lex;
    static const char *table_of_lex[];
    static int         count;

public:

    Lex(type_of_lex t = LEX_NULL, int v = 0, float f = 0.0, string s = "") {
        t_lex = t;
        v_lex = v;
        f_lex = f;
        s_lex = s;
    }
    
    type_of_lex get_type()   { return t_lex; }
    int         get_value()  { return v_lex; }
    string      get_string() { return s_lex; }
    float       get_float()  { return f_lex; }

    static void set_zero() { count = 0; }    

    friend ostream& operator<< (ostream& s, const Lex &l) {
        
        s<<count;
        if (count / 100)      s<<" ";
        else if (count / 10)  s<<"  ";
        else                  s<<"   ";
        
        s<<table_of_lex[l.t_lex]<<" "<<l.v_lex<<" ";
        
        switch (l.t_lex){
            case LEX_STR:
                s<<l.s_lex<<" ";
                break;
            case LEX_FLT:
                s<<l.f_lex<<" ";
                break;
            default: 
                break;
        }

        count++;

        return s<<endl;

    }

};

class Ident {

    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    bool is_lable;
    bool is_struct;
    
    int value_int;
    string str;
    bool boolean;
    float value_float;

public:
    
    Ident(): declare(false), assign(false), is_lable(false), is_struct(false) {}

    void set_name(string name)             { this->name = name; }
    void set_declare(bool declare = true)  { this->declare = declare; }
    void set_type(type_of_lex type)        { this->type = type; }
    void set_assign(bool assign = true)    { this->assign = assign; }
    void set_int(int value)                { this->value_int = value; } 
    void set_float(float value)            { this->value_float = value; }
    void set_str(string str)               { this->str = str; }
    void set_boolean(bool boolean)         { this->boolean = boolean; }
    void set_lable(bool flag = true)       { this->is_lable = flag; }
    void set_struct(bool flag = true)      { this->is_struct = flag; }

    string      get_name()      { return name; }    
    bool        get_declare()   { return declare; }
    type_of_lex get_type()      { return type; }
    bool        get_assign()    { return assign; }
    int         get_int()       { return value_int; }
    float       get_float()     { return value_float; }
    string      get_str()       { return str; }
    bool        get_boolean()   { return boolean; }
    bool        get_is_lable()  { return is_lable; }
    bool        get_is_struct() { return is_struct; }

    friend ostream& operator<< (ostream& s, const Ident &T) {
        s<<"Name: "<<T.name<<endl;
        s<<"Declare: "<<T.declare<<endl;
        s<<"Type: ";
       
        switch (T.type) {
            case LEX_STRING:
                s<<"string";
            break;
            case LEX_INT:
                s<<"integer";
            break;
            case LEX_FLOAT:
                s<<"float";
            break;
            case LEX_BOOL:
                s<<"boolean";
            break;
            default:
                if (T.is_lable) s<<"lable";
                else if (T.type == LEX_ELEMST) s<<"struct variable";
                else s<<"struct";
            break;
        }
        s<<endl;

        if (T.is_struct && T.type != LEX_ID && T.type != LEX_ELEMST) { s<<"it's a part of struct"<<endl; }

        s<<"Assign: "<<T.assign<<endl;
        if (T.assign) {
            
            s<<"value: ";
            
            switch (T.type) {
                case LEX_STRING:
                s<<T.str;
                break;
                case LEX_INT:
                    s<<T.value_int;
                break;
                case LEX_FLOAT:
                    s<<T.value_float;
                break;
                case LEX_BOOL:
                    s<<T.boolean;
                break;
            }
            s<<endl;

        }
        return s<<endl;
    }

};

class Table_ident { 

    Ident *p;
    int size;
    int top;

public:

    Table_ident (int max_size) {
        p = new Ident[size=max_size];
        top = 0;
    }
    ~Table_ident() { delete[] p; }
    
    Ident& operator[] (int k) { return p[k]; }

    int set_elem (string name) {
        for (int i = 0; i < top; ++i) 
            if (p[i].get_name() == name)
                return i;
        p[top].set_name(name);
        ++top;
        return top-1;
    }

    int check_elem (string name) {
        for (int i = 0; i < top; ++i) 
            if (p[i].get_name() == name)
                return i;
        return -1;
    }
    
    friend ostream& operator<< (ostream& s, const Table_ident &T) {
        for (int i = 0; i < T.top; i++) s<<T.p[i]<<endl;
        return s<<endl;
    }

};









//------------------------------------------------------------------------------------//
//-----------------------------------MAIN CLASSES-------------------------------------//
//------------------------------------------------------------------------------------//









class Scanner {

    enum               state {H, IDENT, NUMB, FLT, STR, COM, COM1, COM2, ALE, NEQ, NEGN};
    static const char  *TW[];
    static type_of_lex words[];
    static const char  *TD[];
    static type_of_lex pmarks[];
    state              CS;
    ifstream           inf;
    char               sym;
    string             buf;

    void clear() { buf = ""; }
    void add() { buf += sym; }
    int  look(const string buf, const char **list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
            return i;
            ++i;
        }
        return 0;
    }

public:

    Lex get_lex(Table_ident &TID);
    char check_next_sym() {
        char sym;
        if (!inf.eof()) {
            inf.get(sym);
            inf.putback(sym);
            return sym;
        } else return EOF;
    }


    Scanner(string name_prog) {
        inf.open(name_prog);
        if (!inf.is_open()) throw "ERROR: there is no file with name ";
    }
    ~Scanner() { inf.close(); }

};






class Poliz {

    Lex *p;
    int size;
    int free;

public:

    Poliz(int max_size) {
        p = new Lex [size = max_size];
        free = 0;
    }
    ~Poliz() { delete[] p; }

    void put_lex(Lex l) { p[free] = l; ++free; }
    void put_lex(Lex l, int i) { p[i] = l; }
    void blank() { ++free; }
    int get_free() { return free; }
    Lex& operator[] (int index) {
        if (index > size) throw "POLIZ: out of array";
        else if ( index > free ) throw "POLIZ: indefinite element of array";
        else return p[index];
    };

    void print() { 
        Lex::set_zero();
        for(int i; i < free; i++) cout<<p[i]; 
    }

};




class Parser {

    Lex                               curr_lex;
    type_of_lex                       c_type;
    int                               c_value;
    float                             f_value;
    string                            s_value;
    int                               last_fgo; 
    int                               save_free;                        

    Scanner                           scan;
    Table_ident &                     TID;
    Stack<type_of_lex, MAX_STACK_LEX> st_lex;
    Stack<int, MAX_STACK_INT>         st_int;

    void P();
    void S();
    void DS();  
    void D(bool is_struct = false);
    void V(type_of_lex type);
    void OS();
    void O();
    void E();
    void E1();
    void E2();
    void E3();
    void E4();
    void E5();
    void E6();
    void F();

    void set_var_struct(int type, int i);
    void dec(type_of_lex type, bool is_lable = false, bool is_struct = false);
    void check_id();
    void check_op();
    void check_not_uminus();
    void eq_bool();
    void check_id_in_op();

    void gl() {
        curr_lex = scan.get_lex(TID);
        c_type   = curr_lex.get_type();
        c_value  = curr_lex.get_value();
        s_value  = curr_lex.get_string();
        f_value  = curr_lex.get_float();
    }

public:

    Poliz prog;
    Parser(string name_prog, Table_ident &TID): scan(name_prog), TID(TID),  prog(MAX_SIZE_POLIZ) {}
    int analyze();

};


class Executer {
    Lex           pc_el;
    Poliz &       prog;
    Table_ident & TID;
public:
    Executer(Poliz &prog, Table_ident &TID): prog(prog), TID(TID) {}
    Lex ident_to_lex(Lex o);
    void execute();
};









//-----------------------------------------------------------------------------------//
//------------------------------METHODS OF MAIN CLASSES------------------------------//
//-----------------------------------------------------------------------------------//









Lex Scanner::get_lex (Table_ident &TID) {
    int d, j;
    float d_f;
    bool neg = true, read_neg = false;
    CS = H;
    do {
        
        inf.get(sym);
        if (inf.eof() && (CS != STR) && (CS != COM1) && (CS != COM2)) return Lex(LEX_FIN, 35);
        else if (inf.eof()) throw "ERROR: the comment or the string no have end";
        
        switch (CS) {
//-------------------------------------------H-------------------------------------------//
            case H:
                if (sym == ' ' || sym == '\n' || sym == '\r' || sym == '\t') { continue;
                } else if (isalpha(sym)) {
                    clear();
                    add();
                    CS = IDENT;
                } else if (isdigit(sym)) {
                    d = sym - '0';
                    CS = NUMB;
                } else if (sym == '\"')  {
                    clear();
                    CS = STR;
                } else if (sym == '/')   {
                    CS = COM;
                } else if (sym == '=' || sym == '<' || sym == '>') {
                    clear();
                    add();
                    CS = ALE;
                } else if (sym == '!')   {
                    clear();
                    add();
                    CS = NEQ;
                } else if (sym == '-' && neg) {
                    d = 0;
                    CS = NEGN;
                } else {
                    clear();
                    add();
                    if (j = look(buf, TD)) {
                        if (pmarks[j] == LEX_MINUS) neg = true;
                        return Lex(pmarks[j], j);
                    } else throw sym;
                }
            break;
//-----------------------------------------IDENT-----------------------------------------//
            case IDENT:
                if (isalpha(sym) || isdigit(sym)) add ();
                else {
                    inf.putback(sym);
                    if (j = look(buf, TW)) {
                        //if (sym == ',') { inf.putback(sym);}
                        return Lex(words[j], j);
                    } else {
                        j = TID.set_elem(buf);
                        if (TID[j].get_type() == LEX_ELEMST) return Lex(LEX_ELEMST, j);
                        else return Lex(LEX_ID, j);
                    }
                }
            break;
//-----------------------------------------NUMBER-----------------------------------------//
            case NUMB:
                if (isdigit(sym)) d = d * 10 + (sym - '0');
                else if (sym == '.') {
                    d_f = d;
                    d = 0;
                    CS = FLT;
                } else {
                    inf.putback(sym);
                    return Lex(LEX_NUM, d);
                }
            break;
//------------------------------------------FLT------------------------------------------//
            case FLT:
                if (isdigit(sym)) d = d * 10 + (sym - '0');
                else {
                    int k = 1;
                    while (d / pow(10, k) > 1) k++;
                    d_f += d / pow(10, k);
                    inf.putback(sym);
                    if (read_neg) {
                        read_neg = false;
                        return Lex(LEX_FLT, 0, -d_f);
                    } else return Lex(LEX_FLT, 0, d_f); 
                }
            break;
//------------------------------------------NEGN-----------------------------------------//
            case NEGN:
                if (isdigit(sym)) {
                    d = d * 10 + (sym - '0');
                    read_neg = true;
                } else if (sym == '.') { 
                    d_f = d;
                    d = 0;
                    CS = FLT;
                } else if (read_neg) {
                    read_neg = false;
                    inf.putback(sym);
                    return Lex(LEX_NUM, -d);
                } else {
                    inf.putback(sym);
                    inf.putback('-');
                    neg = false;
                    CS = H;
                }
            break;
//------------------------------------------STR-------------------------------------------//
            case STR:
                if (sym != '\"') add();
                else {
                    return Lex(LEX_STR, 0, 0.0, buf);
                }
            break;
//------------------------------------------COM-------------------------------------------//
            case COM:
                if (sym == '*') CS = COM1;
                else {
                    inf.putback(sym);
                    return Lex(LEX_SLASH, 10);
                };
            break;
//------------------------------------------COM1------------------------------------------//
            case COM1:
                if (sym == '*') CS = COM2;
            break; 
//------------------------------------------COM2------------------------------------------//
            case COM2:
                if (sym == '/') CS = H;
                else if (sym != '*') CS = COM1;
            break;
//------------------------------------------ALE-------------------------------------------//
            case ALE:
            if (sym == '=') {
                add ();
                j = look(buf, TD);
                return Lex(pmarks[j], j);
            } else {
                j = look(buf, TD);
                return Lex(pmarks[j], j);
            }
            break;
//------------------------------------------NEQ------------------------------------------//
            case NEQ:
                if (sym == '=' ) {
                    add ();
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j); 
                } else {
                    inf.putback(sym);
                    return Lex(LEX_UMINUS, 19);
                    CS = H;
                }
            break;
        }
    } while (true);
}


void Parser::P() {
    if (c_type == LEX_PROGRAM) {
        gl();
        if (c_type == LEX_LBRACE) {
            gl();
            S();
            D();
            OS();
            if (c_type == LEX_RBRACE) {
                gl();
                if (c_type != LEX_FIN) throw "ERROR: a part of program isn't in the program case";
            } else  throw "ERROR: no closing bracket in program part";
        } else throw "ERROR: no begining bracket in program part";
    } else throw "ERROR: program should start with word 'program'";
}

void Parser::S() {

    while (c_type == LEX_STRUCT) {
        gl();
        if (c_type == LEX_ID) {
            st_int.push(c_value);
            dec(c_type, false, true);
            gl();
            if (c_type == LEX_LBRACE) {
                gl();
                DS();
                if (c_type == LEX_RBRACE) {
                    gl();
                    if (c_type == LEX_SEMICOLON) {
                        gl();
                    } else throw "ERROR: no semicolon after struct";
                } else throw "ERROR: no ending bracket in struct";
            } else throw "ERROR: no begining bracket in struct";
        } else throw "ERROR: no identifier for struct";
    } 

}

void Parser::DS() {

    if (c_type == LEX_INT || c_type == LEX_FLOAT || c_type == LEX_BOOL || c_type == LEX_STRING) {
        D(true);
    } else throw "ERROR: no descriptions in struct";

}

void Parser::D(bool is_struct) {

    bool flag = true;
    type_of_lex copy = c_type;
    
    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL || c_type == LEX_FLOAT) {
    
        gl();
        V(copy);
        while (c_type == LEX_COMMA) {
            gl();
            V(copy);
        }
        dec(copy, false, is_struct); 
        if (c_type == LEX_SEMICOLON) {
            gl();
            D(is_struct);
        } else throw "ERROR: a semicolon should follow the declaration of variables";
    
    } else if (c_type == LEX_ID && !is_struct) {
        
        int copy_i = c_value;

        if (TID[c_value].get_is_struct() && TID[c_value].get_type() == LEX_ID) {    
            
            do {
                gl();
                st_int.push(c_value);
                dec(LEX_ELEMST, false, true);

                set_var_struct(copy_i, c_value);
                gl();
            } while (c_type == LEX_COMMA);

            if (c_type == LEX_SEMICOLON) {
                gl();
                D();
            } else throw "ERROR: no semicolon after of appropriation";
       
        }
    
    }
}

void Parser::V(type_of_lex type) {
    if (c_type == LEX_ID) {
        
        int copy = c_value;
        st_int.push(c_value);
        gl();
        
        if (c_type == LEX_EQ) {
            gl();
            switch (type) {
                case LEX_INT:
                    if (c_type == LEX_NUM) {
                        TID[copy].set_assign();
                        TID[copy].set_int(c_value);
                    } else throw "ERROR: different type of variable in assignment integer";
                break;
                case LEX_FLOAT:
                    if (c_type == LEX_FLT) {
                        TID[copy].set_assign();
                        TID[copy].set_float(f_value);
                    } else throw "ERROR: different type of variable in assignment float";
                break;
                case LEX_BOOL:
                    if (c_type == LEX_TRUE || c_type == LEX_FALSE) {
                        TID[copy].set_assign();
                        TID[copy].set_boolean(c_value);
                    } else throw "ERROR: different type of variable in assignment boolean";
                break;
                case LEX_STRING:
                    if (c_type == LEX_STR) {
                        TID[copy].set_assign();
                        TID[copy].set_str(s_value);
                    } else throw "ERROR: different type of variable in assignment string";
                break;
                default:
                    throw c_type;
                break;
            }
            gl();
        }
    } else throw "ERROR: there are no identifiers in the variable declaration part";
}

void Parser::OS() {
    int count_i = 0;
    O();
    while (c_type != LEX_RBRACE) {
        if (count_i < MAX_NUM_ITER) {
            O();
            count_i++;
        } else throw "ERROR: no closing bracket in program part";
    } 
}

void Parser::O() {
    
    int pl0, pl1, pl2, pl3;

    switch(c_type) {
//---------------------------------------IF---------------------------------------//
        case LEX_IF:
            gl();
            if (c_type == LEX_LPAREN) {
                gl();
                E();
                eq_bool();
                pl2 = prog.get_free();
                last_fgo = prog.get_free();
                prog.blank();
                prog.put_lex(Lex(POLIZ_FGO));
                if(c_type == LEX_RPAREN) {
                    gl();
                    O();
                    pl3 = prog.get_free();
                    prog.blank();
                    prog.put_lex(POLIZ_GO);
                    prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
                    if (c_type == LEX_ELSE) {
                        gl();
                        O();
                        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
                    } else throw "ERROR: no 'else' in operator 'if'";
                } else throw "ERROR: no closing bracket in operator 'if'";
            } else throw "ERROR: no beginin bracket in operator 'if'";
        break;
//---------------------------------------WHILE---------------------------------------//
        case LEX_WHILE:
            gl();
            if (c_type == LEX_LPAREN) {
                gl();
                pl0 = prog.get_free();
                E();
                eq_bool();

                pl1 = prog.get_free();
                last_fgo = prog.get_free();
                prog.blank();
                prog.put_lex(Lex(POLIZ_FGO));
                
                if (c_type == LEX_RPAREN) {
                    gl();
                    O();
                    prog.put_lex(Lex(POLIZ_LABEL, pl0));
                    prog.put_lex(Lex(POLIZ_GO));
                    prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
                } else throw "ERROR: no closing bracket in operator 'while'";
            } else throw "ERROR: no begining bracket in operator 'while'";
        break;
//----------------------------------------FOR----------------------------------------//
        case LEX_FOR:
            gl();
            if (c_type == LEX_LPAREN) {
                gl();
                E();
                if (c_type == LEX_SEMICOLON) {
                    
                    gl();
                    pl0 = prog.get_free();
                    
                    if (c_type != LEX_SEMICOLON) {
                        E();
                        eq_bool();   
                    } else {
                        prog.put_lex(Lex(LEX_TRUE));
                    }
                    
                    pl1 = prog.get_free();
                    prog.blank();
                    
                    last_fgo = prog.get_free();
                    prog.put_lex(Lex(POLIZ_FGO));
                    
                    pl2 = prog.get_free();
                    prog.blank();
                    prog.put_lex(Lex(POLIZ_GO));
                    
                    if (c_type == LEX_SEMICOLON) {
                        
                        gl();
                        pl3 = prog.get_free();
                        E();
                        
                        prog.put_lex(Lex(POLIZ_LABEL, pl0));
                        prog.put_lex(POLIZ_GO);
                        
                        if(c_type == LEX_RPAREN) {
                            
                            gl();
                            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
                            O();
                            
                            prog.put_lex(Lex(POLIZ_LABEL, pl3));
                            prog.put_lex(Lex(POLIZ_GO));
                            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
                        
                        } else throw "ERROR: no closing bracket in operator 'for'";
                    } else throw "ERROR: no the second semicolon on operator 'for'";
                } else throw "ERROR: no the first semicolon on operator 'for'";
            } else throw "ERROR: no begining bracket in oprator 'for'";
        break;
//---------------------------------------READ---------------------------------------//
        case LEX_READ:
            gl();
            if (c_type == LEX_LPAREN) {
                gl();
                if (c_type == LEX_ID) {
                    check_id_in_op();
                    prog.put_lex(Lex(POLIZ_ADDRESS, c_value));
                    gl();
                    if (c_type == LEX_RPAREN) {
                        gl();
                        if (c_type == LEX_SEMICOLON) {
                            gl();
                            prog.put_lex(Lex(LEX_READ)); 
                        } else throw "ERRPOR: no semicolon after operator 'read'";
                    } else throw "ERROR: no closing bracket in operator 'read'";
                } else throw "ERROR: no identifier in operator 'read'";
            } else throw "ERROR: no begining bracket in operator 'read'";
        break;
//---------------------------------------WRITE---------------------------------------//
        case LEX_WRITE:
            gl();
            if (c_type == LEX_LPAREN) {
                gl();
                E();
                while (c_type == LEX_COMMA) {
                    gl();
                    E();
                }
                if (c_type == LEX_RPAREN) {
                    gl();
                    if (c_type == LEX_SEMICOLON) { 
                        gl();
                        prog.put_lex(Lex(LEX_WRITE));
                    } else throw "ERRPOR: no semicolon after operator 'write'";
                } else throw "ERROR: no closing bracket in operator 'write'";
            } else throw "ERROR: no begining bracket in operator 'write'";
        break;
//---------------------------------------BREAK---------------------------------------//
        case LEX_BREAK:
            gl();
            if (c_type == LEX_SEMICOLON) {
                gl();
                prog.put_lex(Lex(POLIZ_LABEL, last_fgo));
                prog.put_lex(Lex(POLIZ_BGO));
            } else throw "ERROR: no semicolon after break";
        break;
//---------------------------------------GOTO----------------------------------------//
        case LEX_GOTO:
            gl();
            if (c_type == LEX_ID) {
                if (TID[c_value].get_assign()) {
                    prog.put_lex(Lex(POLIZ_LABEL, TID[c_value].get_int()));
                } else {
                    save_free = prog.get_free();
                    prog.blank();
                }    
                gl();
                if (c_type == LEX_SEMICOLON) {
                    gl();
                    prog.put_lex(Lex(POLIZ_GO));
                } else throw "ERROR: no semicolon after goto";
            } else throw "ERROR: no identifiers after goto";
        break;
//-----------------------------------COMPOUND OP------------------------------------//
        case LEX_LBRACE:
            gl();
            OS();
            if (c_type == LEX_RBRACE) gl();
            else throw "ERROR: no closing bracket after composite operator";
        break;
//----------------------------------EXPRESSION OP-----------------------------------//
        case LEX_ID:

            if (scan.check_next_sym() == ':') {
                st_int.push(c_value);
                dec(c_type, true);
                prog.put_lex(Lex(POLIZ_ADDRESS, c_value));
                if (save_free != -1) { 
                    prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), save_free);
                    save_free = -1;
                };
                gl();
                gl();
                O();
            } else {    
                E();
                if (c_type == LEX_SEMICOLON) gl();
                else throw "ERROR: no semicolon after operator of appropriation";
            }
        
        break;
//------------------------------------STRUCT OP------------------------------------//
        case LEX_ELEMST:
            E();
            if (c_type == LEX_SEMICOLON) gl();
            else throw "ERROR: no semicolon after operator of appropriation";
        break;
//-------------------------------------ERROR---------------------------------------//
        default:
            throw "ERROR: invalid operator";
        break;
    
    }

}

void Parser::E() {
    E1();
    if (c_type == LEX_EQ) {
        st_lex.push(LEX_EQ);
        gl();
        E1();
        check_op();
    }
}

void Parser::E1() {
    E2();
    while (c_type == LEX_OR) {
        st_lex.push(c_type);
        gl();
        E1();
        check_op();
    } 
}

void Parser::E2() {
    E3();
    while (c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        E3();
        check_op();
    }
}
 
void Parser::E3() {
    E4();
    while (c_type == LEX_LT  || c_type == LEX_GT  ||
           c_type == LEX_LEQ || c_type == LEX_GEQ ||
           c_type == LEX_EEQ || c_type == LEX_NEQ   ) {
        st_lex.push(c_type);
        gl();
        E4();
        check_op();
    }
}

void Parser::E4() {
    E5();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        st_lex.push(c_type);
        gl();
        E5();
        check_op();
    }
}

void Parser::E5() {
    E6();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH) {
        st_lex.push(c_type);
        gl();
        E6();
        check_op();
    }
}

void Parser::E6() {
    switch (c_type) {
        case LEX_NOT:
            gl();
            F();
            check_not_uminus();
        break;
        case LEX_UMINUS:
            gl();
            F();
            check_not_uminus();
        break;
        default:
            F();
        break;
    }
}

void Parser::F() {

    switch (c_type) {
        
        case LEX_ID:
            check_id();
            prog.put_lex(Lex(POLIZ_ADDRESS, c_value));
            gl();
        break;
        
        case LEX_NUM:
            st_lex.push(LEX_NUM);
            prog.put_lex(curr_lex);
            gl();
        break;
        
        case LEX_FLT:
            st_lex.push(LEX_FLT);
            prog.put_lex(curr_lex);
            gl();
        break;
        
        case LEX_STR:
            st_lex.push(LEX_STR);
            prog.put_lex(curr_lex);
            gl();
        break;
        
        case LEX_TRUE:
            st_lex.push(LEX_BOOL);
            prog.put_lex(Lex(LEX_TRUE, 1));
            gl();
        break;
        
        case LEX_FALSE:
            st_lex.push(LEX_BOOL);
            prog.put_lex(Lex(LEX_FALSE, 0));
            gl();
        break;
        
        case LEX_ELEMST: {
            
            string str = TID[c_value].get_name();
            
            if (scan.check_next_sym() == '.') {
           
                gl();
                gl();
                str += ".";
                str += TID[c_value].get_name();
                
                int i;
                if ((i = TID.check_elem(str))!= -1) {
                    gl();
                    st_lex.push(TID[i].get_type()); 
                    prog.put_lex(Lex(POLIZ_ADDRESS, i)); 
                } else throw "ERROR: there is no such part of strcut";
           
            } else {
                prog.put_lex(Lex(POLIZ_ADDRESS, c_value));
                gl();
                st_lex.push(LEX_ELEMST);
            }

        } break;

        case LEX_LPAREN:
            gl();
            E();
            if (c_type == LEX_RPAREN) gl();
            else throw "ERROR: no closing bracket in expression";
        break;

        default:
            //throw "ERROR: invalid expression";
        break;
    }

}
 
void Parser::set_var_struct(int type, int i) {

    type++;
    while (TID[type].get_type() != LEX_ID && TID[type].get_type() != LEX_ELEMST && TID[type].get_is_struct()) {
        int j = TID.set_elem(TID[i].get_name() + "." + TID[type].get_name());
        st_int.push(j);
        dec(TID[type].get_type(), false, true);
        type++;
    }

}

void Parser::dec(type_of_lex type, bool is_lable, bool is_struct) {
    int i;
    while(!st_int.is_empty()) {
        i = st_int.pop();

        if (TID[i].get_declare()) throw "ERROR: variable already declared";
        else {
            TID[i].set_declare();
            TID[i].set_type(type);
            if (is_lable) {
                TID[i].set_lable(); 
                TID[i].set_int(prog.get_free());
                TID[i].set_assign();
            }
            if (is_struct) TID[i].set_struct();
        } 
    }
}

void Parser::check_id() {
    if (TID[c_value].get_declare()) st_lex.push(TID[c_value].get_type());
    else throw "ERROR: variable not yet declared";
}

int check_type_for_op(type_of_lex type) {
    
    if      (type == LEX_INT)    return 1;
    else if (type == LEX_FLOAT)  return 2; 
    else if (type == LEX_STRING) return 3;
    else if (type == LEX_BOOL)   return 4;
    else if (type == LEX_NUM)    return 5;
    else if (type == LEX_FLT)    return 6;
    else if (type == LEX_STR)    return 7;
    else if (type == LEX_TRUE || type == LEX_FALSE) return 8;
    else return -1;

}


void Parser::check_op() {
    
    bool flag = true;
    type_of_lex t1, t2, op, res;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    
    int it1 = check_type_for_op(t1), it2 = check_type_for_op(t2);
    
    if (t1 == t2 && t1 == LEX_ELEMST) {    
        if (op == LEX_EQ) {
            st_lex.push(LEX_ELEMST);
            prog.put_lex(Lex(op));
        } else throw "ERROR: no such operation for struct";
    } else {
        if (it1 % 4 == it2 % 4) {
        
            if (op == LEX_MINUS || op == LEX_PLUS) {
                
                if (it1 != -1)
                    if (it1 < 5) res = t1;
                    else res = t2;
                else throw "ERROR: invalid operand type in '-' or '+'";

            } else if (op == LEX_LT || op == LEX_GEQ || op == LEX_EEQ ||
                    op == LEX_GT || op == LEX_LEQ || op == LEX_NEQ) {

                if (it1 != -1)
                    res = LEX_BOOL;
                else throw "ERROR: invalid operand type in logical operation";

            } else if (op == LEX_TIMES || op == LEX_SLASH) {

                if (it1 % 4 == 1 || it1 % 4 == 2)
                    if (it1 < 5) res = t1;
                    else res = t2;
                else throw "ERROR: invalid operand type '*' or '/'";

            } else if (op == LEX_AND || op == LEX_OR) {

                if (it1 % 4 == 0) 
                    if (it1 == 0) res = t1;
                    else res = t2;
                else throw "ERROR: invalid operand type in 'and' or 'or'";
        
            } else if (op == LEX_EQ) {

                if (it1 != -1)
                    if (it1 < 5) res = t1;
                    else throw "ERROR: assigning a value to a constant";
                else throw "ERROR: invalid operand type in appropriation";

            } else throw "ERROR: no such operation";
        
        } else throw "ERROR: invalid operand types";        
        st_lex.push(res);
        prog.put_lex(op);
    }
}

void Parser::check_not_uminus() {
    if (st_lex.pop() != LEX_BOOL) throw "ERROR: wrong type is in operation '!' or 'not'";
    else { 
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_NOT));
    }
}

void Parser::eq_bool() {
    if (st_lex.pop() != LEX_BOOL) throw "ERROR: wrong type of expression 'it should be equal bool'";
}

void Parser::check_id_in_op() {
    if(!TID[c_value].get_declare()) throw "ERROR: no declare";   
}

int Parser::analyze() {
    try {
        save_free = -1;
        gl();
        P();
        cout<<"ALL SYSTEMS GOOD"<<endl;
        cout<<TID;
        return 0;
    } catch(const char *err) {
        cout<<err<<endl;
        return -1;
    } catch(type_of_lex lex) {
        cout<<"ERROR: not expected token"<<lex<<endl;
        return -1;
    }
}


Lex Executer::ident_to_lex(Lex o) {

    Lex i = o;

    if (o.get_type() == POLIZ_ADDRESS) {
        Ident elem = TID[o.get_value()];
        switch (elem.get_type()) {
            case LEX_INT:
                i = Lex(LEX_NUM, elem.get_int());
            break;
            case LEX_FLOAT:
                i = Lex(LEX_FLT, 0, elem.get_float());
            break;
            case LEX_BOOL:
                i = Lex(LEX_NUM, elem.get_boolean());
            break;
            case LEX_STRING:
                i = Lex(LEX_STR, 0, 0.0, elem.get_str());
            break;
        }
    } 
    
    return i;

}

void Executer::execute() {

    Stack<Lex, MAX_STACK_LEX> args;
    Lex i, j;
    int index = 0, size = prog.get_free();

    while (index < size) {

        pc_el = prog[index];

        switch (pc_el.get_type()) {
        
        case LEX_TRUE:
            args.push(Lex(LEX_NUM, 1));
        break;    
        case LEX_FALSE:
            args.push(Lex(LEX_NUM, 0));
        break;
        case LEX_NUM:
        case LEX_STR:
        case LEX_FLT:
        case LEX_ELEMST:
        case POLIZ_ADDRESS:
        case POLIZ_LABEL:
            args.push(pc_el);
        break;
        
        case LEX_NOT:
            i = ident_to_lex(args.pop());
            args.push(Lex(LEX_NUM, !i.get_value()));
        break;
        
        case LEX_AND:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());
            args.push(Lex(LEX_NUM, i.get_value() && j.get_value()));
        break;
        
        case LEX_OR:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());
            args.push(Lex(LEX_NUM, i.get_value() || j.get_value()));
        break;

        case POLIZ_GO:
            i = ident_to_lex(args.pop());
            index = i.get_value() - 1;
        break;

        case POLIZ_FGO:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());
            if (!j.get_value()) index = i.get_value() - 1;
        break;

        case POLIZ_BGO:
            i = ident_to_lex(args.pop());
            j = prog[i.get_value()];
            index = j.get_value();
        break;

        case LEX_WRITE:
            //pass ERROR write(E1, E2);
            i = args.pop();
            if (i.get_type() == POLIZ_ADDRESS)
                 if (!TID[i.get_value()].get_assign()) throw "ERROR: variable nothing assigned";

            i = ident_to_lex(i);
    
            switch (i.get_type()) {
            case LEX_NUM:
                cout<<i.get_value();
            break;
            case LEX_STR:
                cout<<i.get_string();
            break;
            case LEX_FLT:
                cout<<i.get_float();
            break;                }

            cout<<endl;
        break;

        case LEX_READ: {
            i = args.pop();
            
            int elem_int;
            bool elem_bool;
            string elem_string;
            float elem_float;

            switch (TID[i.get_value()].get_type()) {
            case LEX_BOOL:
                cout<<"Input boolean: ";
                cin>>elem_bool;
                TID[i.get_value()].set_boolean(elem_bool); 
            break;
            case LEX_INT:
                cout<<"Input integet: ";
                cin>>elem_int;
                TID[i.get_value()].set_int(elem_int);
            break;
            case LEX_STRING:
                cout<<"Input string: ";
                cin>>elem_string;
                TID[i.get_value()].set_str(elem_string);
            break;
            case LEX_FLOAT:
                cout<<"Input float: ";
                cin>>elem_float;
                TID[i.get_value()].set_float(elem_float);
            break;                }

            TID[i.get_value()].set_assign();
        } break;

        case LEX_PLUS:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, i.get_value() + j.get_value()) );
            break;
            case LEX_STR:
                args.push( Lex(LEX_STR, 0, 0.0, i.get_string() + j.get_string()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_FLT, 0, i.get_float() + j.get_float()) );
            break;                 }

        break;

        case LEX_MINUS:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, j.get_value() - i.get_value()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_FLT, 0, j.get_float() - i.get_float()) );
            break;                 }

        break;

        case LEX_TIMES:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, i.get_value() * j.get_value()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_FLT, 0, i.get_float() * j.get_float()));
            break;                 }

        break;

        case LEX_SLASH:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, j.get_value() / i.get_value()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_FLT, 0, j.get_float() / i.get_float()) );
            break;                 }

        break;

        case LEX_LT:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, j.get_value() < i.get_value()) );
            break;
            case LEX_STR:
                args.push( Lex(LEX_NUM, j.get_string() < i.get_string()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_NUM, j.get_float() < i.get_float()) );
            break;                 } 

        break;

        case LEX_GT:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, j.get_value() > i.get_value()) );
            break;
            case LEX_STR:
                args.push( Lex(LEX_NUM, j.get_string() > i.get_string()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_NUM, j.get_float() > i.get_float()) );
            break;                 } 

        break;

        case LEX_LEQ:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, j.get_value() <= i.get_value()) );
            break;
            case LEX_STR:
                args.push( Lex(LEX_NUM, j.get_string() <= i.get_string()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_NUM, j.get_float() <= i.get_float()) );
            break;                 } 

        break;

        case LEX_GEQ:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, j.get_value() >= i.get_value()) );
            break;
            case LEX_STR:
                args.push( Lex(LEX_NUM, j.get_string() >= i.get_string()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_NUM, j.get_float() >= i.get_float()) );
            break;                 } 

        break;

        case LEX_EEQ:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, j.get_value() == i.get_value()) );
            break;
            case LEX_STR:
                args.push( Lex(LEX_NUM, j.get_string() == i.get_string()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_NUM, j.get_float() == i.get_float()) );
            break;                 } 

        break;

        case LEX_NEQ:
            i = ident_to_lex(args.pop()); j = ident_to_lex(args.pop());

            switch (i.get_type()) {
            case LEX_NUM:
                args.push( Lex(LEX_NUM, j.get_value() != i.get_value()) );
            break;
            case LEX_STR:
                args.push( Lex(LEX_NUM, j.get_string() != i.get_string()) );
            break;
            case LEX_FLT:
                args.push( Lex(LEX_NUM, j.get_float() != i.get_float()) );
            break;                 } 

        break;

        case LEX_EQ: {
            j = ident_to_lex(args.pop()); i = args.pop();
            Ident &elem = TID[i.get_value()];

            switch (elem.get_type()) {
            case LEX_INT:
                elem.set_int(j.get_value());
            break;
            case LEX_STRING:
                elem.set_str(j.get_string());
            break;
            case LEX_BOOL:
                elem.set_boolean(j.get_value());
            break;
            case LEX_FLOAT:
                elem.set_float(j.get_float());
            break;
            case LEX_ELEMST: {
                int k = i.get_value() + 1;
                while (((TID[k].get_type() == LEX_INT) || (TID[k].get_type() == LEX_STRING) ||
                       (TID[k].get_type() == LEX_BOOL) || (TID[k].get_type() == LEX_FLOAT))
                        && TID[k].get_is_struct()) {
                    
                    switch (TID[k].get_type()) {
                    case LEX_INT:
                        TID[k].set_int(j.get_value());
                    break;
                    case LEX_STRING:
                        TID[k].set_str(j.get_string());
                    break;
                    case LEX_BOOL:
                        TID[k].set_boolean(j.get_value());
                    break;
                    case LEX_FLOAT:
                        TID[k].set_float(j.get_float());
                    break;                     }
                    
                    TID[k].set_assign();

                    k++;
                }    
            } break; 
            }

            elem.set_assign();
            args.push(i);

        break;       }

        default:
            throw "ERROR: invalid type in poliz";
        break;
        }

        index++;

    }

}


const char *Lex::table_of_lex[] {

    
    "LEX_NULL     ",          // 0
    "LEX_PROGRAM  ",         // 1
    
    "LEX_INT      ",         // 2
    "LEX_STRING   ",         // 3
    "LEX_BOOL     ",         // 4
    "LEX_FLOAT    ",         // 5

    "LEX_IF       ",         // 6
    "LEX_ELSE     ",         // 7
    "LEX_WHILE    ",         // 8
    "LEX_FOR      ",         // 9
    "LEX_BREAK    ",         // 10
    "LEX_GOTO     ",         // 11
    "LEX_READ     ",         // 12
    "LEX_WRITE    ",         // 13

    "LEX_AND      ",         // 14
    "LEX_OR       ",         // 15
    "LEX_NOT      ",         // 16

    "LEX_TRUE     ",         // 17
    "LEX_FALSE    ",         // 18
    "LEX_NUM      ",         // 19
    "LEX_STR      ",         // 20
    "LEX_FLT      ",         // 21

    "LEX_LPAREN   ",         // 22 (
    "LEX_RPAREN   ",         // 23 )
    "LEX_LBRACE   ",         // 24 {
    "LEX_RBRACE   ",         // 25 }
    "LEX_POINT    ",         // 26 .
    "LEX_COMMA    ",         // 27 ,
    "LEX_COLON    ",         // 28 :
    "LEX_SEMICOLON",         // 29 ;
    "LEX_MINUS    ",         // 30 -
    "LEX_PLUS     ",         // 31 +
    "LEX_TIMES    ",         // 32 *
    "LEX_SLASH    ",         // 33 /
    "LEX_EQ       ",         // 34 =
    "LEX_LT       ",         // 35 <
    "LEX_GT       ",         // 36 >
    "LEX_EEQ      ",         // 37 ==
    "LEX_LEQ      ",         // 38 <=
    "LEX_NEQ      ",         // 39 !=
    "LEX_GEQ      ",         // 40 >=
    "LEX_UMINUS   ",         // 41 !
    "LEX_FIN      ",         // 42 EOF

    "LEX_STRUCT   ",         // 43
    "LEX_ELEMST   ",         // 44 
    "LEX_ID       ",         // 45
    "POLIZ_GO     ",         // 46
    "POLIZ_FGO    ",         // 47
    "POLIZ_BGO    ",         // 48
    "POLIZ_LABEL  ",         // 49
    "POLIZ_ADDRESS"          // 50

};

const char *Scanner::TW[]  = {
    
    "",                  // 0
    "program",           // 1
    
    "int",               // 2
    "string",            // 3
    "bool",              // 4
    "float",             // 5

    "if",                // 6
    "else",              // 7
    "while",             // 8
    "for",               // 9
    "break",             // 10
    "goto",              // 11
    "read",              // 12
    "write",             // 13

    "and",               // 14
    "or",                // 15
    "not",               // 16
    "true",              // 17
    "false",             // 18
    "struct",            // 19
    nullptr

};

const char *Scanner::TD[] = {

    "",                  // 0
    "(",                 // 1 (
    ")",                 // 2 )
    "{",                 // 3 {
    "}",                 // 4 }
    ".",                 // 5 .
    ",",                 // 6 ,
    ":",                 // 7 :
    ";",                 // 8 ;
    "-",                 // 9 -
    "+",                 // 10 +
    "*",                 // 11 *
    "/",                 // 12 /
    "=",                 // 13 =
    "<",                 // 14 <
    ">",                 // 15 >
    "==",                // 16 ==
    "<=",                // 17 <=
    "!=",                // 18 !=
    ">=",                // 19 >=
    "!",                 // 20 !
    nullptr
};

type_of_lex Scanner::words[] = {
    
    LEX_NULL,            // 0
    LEX_PROGRAM,         // 1
    
    LEX_INT,             // 2
    LEX_STRING,          // 3
    LEX_BOOL,            // 4
    LEX_FLOAT,           // 5

    LEX_IF,              // 6
    LEX_ELSE,            // 7
    LEX_WHILE,           // 8
    LEX_FOR,             // 9
    LEX_BREAK,           // 10
    LEX_GOTO,            // 11
    LEX_READ,            // 12
    LEX_WRITE,           // 13

    LEX_AND,             // 14
    LEX_OR,              // 15
    LEX_NOT,             // 16
    LEX_TRUE,            // 17
    LEX_FALSE,           // 18
    
    LEX_STRUCT          // 19

};

type_of_lex Scanner::pmarks[] = {

    
    LEX_NULL,            // 0
    
    LEX_LPAREN,          // 1 (
    LEX_RPAREN,          // 2 )
    LEX_LBRACE,          // 3 {
    LEX_RBRACE,          // 4 }
    LEX_POINT,           // 5 .
    LEX_COMMA,           // 6 ,
    LEX_COLON,           // 7 :
    LEX_SEMICOLON,       // 8 ;
    LEX_MINUS,           // 9 -
    LEX_PLUS,            // 10 +
    LEX_TIMES,           // 11 *
    LEX_SLASH,           // 12 /
    LEX_EQ,              // 13 =
    LEX_LT,              // 14 < 
    LEX_GT,              // 15 >
    LEX_EEQ,             // 16 ==
    LEX_LEQ,             // 17 <=
    LEX_NEQ,             // 18 !=
    LEX_GEQ,             // 19 >=
    LEX_UMINUS           // 20 !

};

int Lex::count = 0;

int main(int argc, char *argv[]) {
    Table_ident TID(MAX_SIZE_TID);

    if (argc > 1) {
        Parser par(argv[1], TID);

        Executer exec(par.prog, TID);

        try {
            if (!par.analyze()) {
                par.prog.print(); 
                cout<<endl<<endl<<"PROGRAM: "<<endl;
                exec.execute();
            }
        } catch(char c) { 
            cout<<"ERROR: undefine symbol "<<c<<endl;
        } catch(const char *err) {
            cout<<err<<endl;
        }

        return 0;
    };

}
