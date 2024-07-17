#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <tuple>


using namespace std;


//----------------------------------------------------------------------//
//------------------------------STRUCTURES------------------------------//
//----------------------------------------------------------------------//
struct Descriptor{
public:
    int type;
    string name;
    Descriptor(string name, int type): name(name), type(type) {} 
};


//---------------------------------------------------------------------//
//-------------------------------CLASSES-------------------------------//
//---------------------------------------------------------------------//

class Variable {

    string name;

public:

    Variable(): name("") {}
    Variable(string name): name(name) {}

    string get_name() { return name; }

};


template<typename T> 
class TypeVariable: public Variable {

    T volume;
    bool fill;

public:

    TypeVariable(): Variable(), fill(false) {}
    TypeVariable(string name): Variable(name), fill(false) {}
    TypeVariable(string name, T volume): Variable(name), volume(volume), fill(true) {}

    void set_volume(T volume) { 
        this->volume = volume;
        fill = true;    
    }
    
    bool get_fill() { return fill; }
    
    T get_volume() { return volume; }

    T &get_refer() { return &volume; }

    TypeVariable<T> operator+ (const TypeVariable<T> &other) {
        TypeVariable<T> new_elem("", this->get_volume() + other.volume);
        return new_elem;
    }

};


class ReaderVariable {

    list<Descriptor> table;

    int get_type(string type) {

        if (type == "int") return 1;
        else
            if (type == "string") return 2;
            else
                if (type == "boolean") return 3;
                else
                    if (type == "bool") return 3;
                    else
                        if (type == "float") return 4;
                        else 
                            return 0;
    }

    int check_dot_comma(string &str) {
        
        int res = 0;
        
        if (str.back() == ',') { 
            str.pop_back();
            res = 1;
        }
        if (str.back() == ';') {
            str.pop_back();
            res = 2;
        }
        
        return res;

    }

    void read_integers(ifstream &file, list<TypeVariable<int>> &arr_vars) {

        string ind;
        int dot_comma = 0; 
        
        while (dot_comma != 2) {
            
            file>>ind;
            dot_comma = check_dot_comma(ind);
            
            TypeVariable<int> *p = new TypeVariable<int>(ind);
            Descriptor d(ind, 1);
            table.push_front(d);

            if (dot_comma == 0) { // "a = volume," or "a = volume;"
                

                file>>ind;
                file>>ind;
                dot_comma = check_dot_comma(ind);

                int volume = stoi(ind);
                p->set_volume(volume);

            }

            arr_vars.push_front(*p);
        
        }
    }

    void read_strings(ifstream &file, list<TypeVariable<string>> &arr_vars) {

        string ind;
        int dot_comma = 0; 
        
        while (dot_comma != 2) {
            
            file>>ind;
            dot_comma = check_dot_comma(ind);

            TypeVariable<string> *p = new TypeVariable<string>(ind);
            Descriptor d(ind, 2);
            table.push_front(d);

            if (dot_comma == 0) { // "a = volume," or "a = volume;"
                
                file>>ind;
                file>>ind;
                dot_comma = check_dot_comma(ind);

                ind.erase(0, 1);
                ind.pop_back();
                p->set_volume(ind);

            }

            arr_vars.push_front(*p);
        
        }
    }

    void read_booleans(ifstream &file, list<TypeVariable<bool>> &arr_vars) {

        string ind;
        int dot_comma = 0; 
        
        while (dot_comma != 2) {
            
            file>>ind;
            dot_comma = check_dot_comma(ind);

            TypeVariable<bool> *p = new TypeVariable<bool>(ind);
            Descriptor d(ind, 3);
            table.push_front(d);

            if (dot_comma == 0) { // "a = volume," or "a = volume;"
                
                file>>ind;
                file>>ind;
                dot_comma = check_dot_comma(ind);
        
                bool volume;
                if (ind == """true""") volume = true;
                else volume = false;
                p->set_volume(volume);

            }

            arr_vars.push_front(*p);
        
        }
    }

    void read_floats(ifstream &file, list<TypeVariable<float>> &arr_vars) {

        string ind;
        int dot_comma = 0; 
        
        while (dot_comma != 2) {
            
            cout<<"!"<<endl;

            file>>ind;
            dot_comma = check_dot_comma(ind);

            TypeVariable<float> *p = new TypeVariable<float>(ind);
            Descriptor d(ind, 4);
            table.push_front(d);

            if (dot_comma == 0) { // "a = volume," or "a = volume;"
                
                file>>ind;
                file>>ind;
                dot_comma = check_dot_comma(ind);
        
                float volume = stof(ind);
                p->set_volume(volume);

            }

            arr_vars.push_front(*p);
        
        }
    }

public:

    ReaderVariable() {}

    tuple<list<TypeVariable<int>>,
          list<TypeVariable<string>>,
          list<TypeVariable<bool>>,
          list<TypeVariable<float>>,
          list<Descriptor>> set_variables(string name_file) {
        
        list<TypeVariable<int>> integers;
        list<TypeVariable<string>> strings;
        list<TypeVariable<bool>>  booleans;
        list<TypeVariable<float>> floats;
          
        ifstream file(name_file);
        
        table.clear();

        if (file.is_open()) {
            string ind;
            
            while (ind != "{") { file>>ind; }
            int type = 1;
            while (type != 0) {
                
                file>>ind;
                type = get_type(ind);
                
                switch (type) {
                
                case 1: //int
                    read_integers(file, integers);
                    break;
                
                case 2: //string
                    read_strings(file, strings);
                    break;
                
                case 3: //bool
                    read_booleans(file, booleans);
                    break;
                
                case 4: //float
                    read_floats(file, floats);
                    break;

                default:
                    break;
                
                }
            }
        } else { cout<<"Error: there is no file with name: "<<name_file<<endl; }

        return make_tuple(integers, strings, booleans, floats, table);
    
    }

};


//-----------------------------------------------------------------------//
//-------------------------------FUNCTIONS-------------------------------//
//-----------------------------------------------------------------------//

template<typename T>
ostream& operator<< (ostream &os, TypeVariable<T> &elem) {
    if (elem.get_fill()) return os<<elem.get_name()<<" "<<elem.get_volume()<<endl;
    else return os<<elem.get_name()<<" empty"<<endl;
}


int get_type_var(const string &name, const list<Descriptor> &table) {
    
    auto iter = table.begin();
    while((iter != table.end()) && (iter->name != name)) { iter++; }
    
    if (iter->name == name) return iter->type;
    else {
        cout<<"Error: there is no variable with name: "<<name<<endl;
        return -1;
    }
    
}


void print_data(list<TypeVariable<int>> integers,
                list<TypeVariable<string>> strings,
                list<TypeVariable<bool>> booleans,
                list<TypeVariable<float>> floats) {
    
    cout<<"integers:"<<endl;
    if (!integers.empty()) {
        auto i = integers.begin();
        while (i != integers.end()) { 
            cout<<*i; 
            i++;
        }
        cout<<endl;
    } else { cout<<"empty"<<endl<<endl; }
    
    cout<<"strings:"<<endl;
    if (!strings.empty()) {
        auto i = strings.begin();
        while (i != strings.end()) { 
            cout<<*i;
            i++;
        }
        cout<<endl;
    } else { cout<<"empty"<<endl<<endl; }
    
    cout<<"booleans:"<<endl;
    if (!booleans.empty()) {
        auto i = booleans.begin();
        while (i != booleans.end()) { 
            cout<<*i;
            i++;    
        }
        cout<<endl;
    } else { cout<<"empty"<<endl<<endl; }
    
    cout<<"float:"<<endl;
    if (!floats.empty()) {
        auto i = floats.begin();
        while (i != floats.end()) { 
            cout<<*i;
            i++;
        }
        cout<<endl;
    } else { cout<<"empty"<<endl<<endl; }

}