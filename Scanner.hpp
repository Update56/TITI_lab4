#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP
#include "defs.hpp"
#include <iostream>
#include <string.h>
using namespace std;

class Scanner {
private:
    char t[MAX_TEXT]; //исходный текст
    int ptr = 0; //указатель текущей позиции в исходном тексте
    FILE* in;
    TypeMod code;
    int _pos;
    void GetData(FILE* in);
public:
    int line;
    Scanner(FILE* in);
    int PPP();
    void GetPtr(int i); //восстановить указатель
    int SetPtr(void); //запомнить указатель
    void PrintError(string errorMessage, string lexeme); //выдать сообщение об ошибке
    int Scanning(TypeLex l); //программа сканера
};
#endif 