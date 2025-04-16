#ifndef __DIAGRAM
#define __DIAGRAM
#include "Scanner.hpp"
#include "Semant.hpp"
#include <vector>
class   TDiagram {
private:    // сканер и все функции СД, кроме аксиомы
    TScanner* sc;
    Tree* root; //текущий узел
    Tree* start; //стартовый узел
    void  Data();           //данные
    DataType Type();        //тип
    void  Func();           //функция
    DataType NamedConst();  //именованая константа
    void Class();           //класс
    void Method();          //метод класса
    void Operator();        //оператор
    void Assigment(Tree* var = NULL);       //присваивание
    DataType Variable(DataType type, TypeLex lex = NULL);   //переменная с названием пользовательского типа
    DataType Expression();  //выражение
    DataType Comparison();  //сравнение
    DataType Shift();       //сдвиг
    DataType Term();        //слагаемое
    DataType Factor();      //множитель
    DataType Elementary();  //эл.выражения
    void CompOper();        //составной оператор
    DataType FuncCall();    //вызов функции
    void DoWhile();         //цикл
    Tree* ClassAccsess(TypeLex lex); //проверка доступа к членам класса
    int lookForward(int k); //определение типа следущей лексемы на k позиции от текущего указателя
public:
    TDiagram(TScanner* s) {
        sc = s;
        this->root = new Tree();
        this->root->SetCur(this->root);
        start = this->root->GetCur();
    }
    ~TDiagram() {
        delete start;
    }
    void Program(); 
};
#endif
