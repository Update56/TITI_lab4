#include "Diagram.hpp"
#include "iostream"
using namespace std;

void Diagram::SetToken() {
    previousToken = t; // сохраняем предыдущий токен
    t = sc->Scanning(l);
}

void Diagram::GetToken() {
    t = previousToken; // возвращаем предыдущий токен
}

void Diagram::Type() {
    if ((t == typeDouble) || (t == typeChar) || (t == typeId) || (t == typeVoid)) {
        return;
    }
    else {
        sc->PrintError("ожидался тип данных(double, char) или индентификатор", l);
    }
}

void Diagram::Program() {
    SetToken(); sc->PPP();
    Type();
    if (t == typeVoid) {
        SetToken(); sc->PPP();
        if (t == typeMain) {
            Func();
            return;
        }
        else {
            sc->PrintError("Ожидался символ main", l);
        }
    }
    else if ((t == typeDouble) || (t == typeChar)  || (t == typeId)) {
        Data();
    }
    else if (t == typeClass) {
        Class();
    }
    else if (t == typeConst) {
        NamedConst();
    }
    else {
        sc->PrintError("ожидался тип данных (double, char), индификатор, класс, именованная константа", l);
    }
}
void Diagram::Data() {
    do {
        SetToken(); sc->PPP();
        if (t != typeId)
            sc->PrintError("ожидался идентификатор", l);
        SetToken(); sc->PPP();
        if (t == typeEval) {
            Expression();
        }
        if (t == typeId) {
            SetToken(); sc->PPP();
        }
    } while (t == typeComma);

    if (t != typeSemicolon) 
        sc->PrintError("ожидался символ ;", l);
}

void Diagram::Func() {
    SetToken(); sc->PPP();
    if (t != typeLeftBracket)
        sc->PrintError("ожидался символ (", l);
    SetToken(); sc->PPP();
    if (t != typeRightBracket)
        sc->PrintError("ожидался символ )", l);
    SetToken(); sc->PPP();

    if (t == typeLeftBrace)
        CompOper();
}

void Diagram::NamedConst()
{
    SetToken(); sc->PPP();
    if (t != typeId) {
        sc->PrintError("ожидался идентификатор", l);
    }
    SetToken(); sc->PPP();
    Type();
    SetToken(); sc->PPP();
    if (t == typeEval) {
        Expression();
    }
    else {
        sc->PrintError("ожидался знак =", l);
    }
    if (t == constInt || t == constHex) {
        SetToken(); sc->PPP();
        return;
    }
    else
    {
        sc->PrintError("ожидаласт константа", l);
    }
    if (t != typeSemicolon)
        sc->PrintError("ожидался символ ;", l);
}

void Diagram::Class()
{
    SetToken(); sc->PPP();
    if (t != typeId) {
        sc->PrintError("ожидался идентификатор", l);
    }
    SetToken(); sc->PPP();

    if (t != typeLeftBrace)
    {
        sc->PrintError("ожидался символ {", l);
    }
    SetToken(); sc->PPP();
    while (t != typeRightBrace)
    {
        if ((t == typeDouble) || (t == typeChar) || (t == typeId)) {
            Data();
        }
        else if (t == typeVoid){
            Method();
        }
        else
        {
            sc->PrintError("ожидались data или method", l);
        }
        SetToken(); sc->PPP();
    }
    if (t != typeSemicolon)
        sc->PrintError("ожидался символ ;", l);
}

void Diagram::Method()
{
    SetToken(); sc->PPP();
    if (t != typeId) {
        sc->PrintError("ожидался идентификатор", l);
    }
    SetToken(); sc->PPP();

    if (t != typeLeftBracket)
    {
        sc->PrintError("ожидался символ (", l);
    }
    SetToken(); sc->PPP();
    while (t != typeRightBracket)
    {
        Type();
        SetToken(); sc->PPP();
        if (t != typeId) {
            sc->PrintError("ожидался идентификатор", l);
        }
        if (t == typeComma)
        {
            SetToken(); sc->PPP();
        }
    }
    SetToken(); sc->PPP();
    if (t == typeLeftBrace)
        CompOper();
}

void Diagram::CompOper() {
    int braceCount = 0; // Для отслеживания уровня вложенности фигурных скобок
    do {
        if (t == typeLeftBrace) {
            braceCount++; // Увеличиваем уровень вложенности
        }
        else if (t == typeRightBrace) {
            braceCount--; // Уменьшаем уровень вложенности
            if (braceCount < 0) {
                sc->PrintError("Лишняя закрывающая фигурная скобка", l);
                return;
            }
        }
        if (t == typeLeftBrace) {
            SetToken(); sc->PPP();   // Считываем следующий токен
        }
        else if ((t == typeDouble) || (t == typeChar)) {
            Data();
            SetToken(); sc->PPP();
        }
        if ((t == typeSemicolon) || (t == typeId) || (t == typeDo)){
            Operator();
        }
        if (t == typeConst) {
            NamedConst();
        }
    } while ((braceCount > 0) && (t == typeLeftBrace || t == typeRightBrace || t == typeDouble || t == typeChar ||t == typeSemicolon || t == typeId || t == typeDo || t == typeWhile));

    if (braceCount > 0) {
        sc->PrintError("не закрыты фигурные скобки", l);
    }
}

void Diagram::Operator() {
    if (t == typeSemicolon) {
        SetToken(); sc->PPP();
        return;
    }
    if (t == typeDo) {
        Operator();
        SetToken(); sc->PPP();
        if (t == typeWhile) {
            SetToken(); sc->PPP();
            if (t == typeLeftBracket)
            {
                Expression();
                if (t == typeRightBracket) {
                    SetToken(); sc->PPP();
                    if (t == typeSemicolon) {
                        return;
                    }
                    else
                        sc->PrintError("ожидался символ ;", l);
                }
                else
                    sc->PrintError("ожидался символ )", l);
            }
            else
                sc->PrintError("ожидался символ (", l);
        }
        else
            sc->PrintError("ожидался while", l);
    }
    
    if (t == typeRightBrace) {
        return;
    }
    if (t == typeLeftBrace) {
        CompOper();
        return;
    }

    Assigment();
    if (t != typeSemicolon)
        sc->PrintError("ожидался символ ;", l);
}


void Diagram::Assigment() {
    
    Variable();
    if (t == typeEval) {
        Expression();
    }
    else {
        sc->PrintError("ожидался знак =", l);
    }
}

void Diagram::Variable() {

    if (t != typeId) {
        sc->PrintError("ожидался идентификатор", l);
    }
    if (t == typeAccessOperator)
    {
        Variable();
    }
}

void Diagram::Expression() {
    SetToken(); sc->PPP();
    Comparison();
    while (t == typePlus || t == typeMinus) {
        SetToken(); sc->PPP();
        Comparison();
    }
}

void Diagram::Comparison() {
    Shift();
    while ((t == typeLessOrEq) || (t == typeLess) || (t == typeMoreOrEq) || (t == typeMore) || (t == typeUnEq)) {
        SetToken(); sc->PPP();
        Shift();
    }
}

void Diagram::Shift() {
    Term();
    while ((t == typeShiftLeft) || (t == typeShiftRight)) {
        SetToken(); sc->PPP();
        Term();
    }
}

void Diagram::Term() {
    Factor();
    while (t == typePlus || t == typeMinus) {
        SetToken(); sc->PPP();
        Factor();
    }
}

void Diagram::Factor() {
    Elementary();

    while (t == typeMul || t == typeDiv || t == typeMod) {
        SetToken(); sc->PPP();
        Elementary();
    }
}

void Diagram::Elementary() {
    if (t == constInt || t == constHex) {
        SetToken(); sc->PPP();
        return;
    }
    if (t == typeId) {
        Variable();
        return;
    }
    if (t == typeLeftBracket) {
        Expression();
    }
    if (t == typeRightBracket) {
        SetToken(); sc->PPP();
    }
    Unary();
}

void Diagram::Unary() {
    if (t == typePlus || t == typeMinus) {
        SetToken(); sc->PPP();
    }
    Elementary();
}