#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP
#include "defs.hpp"

class TScanner {
private:
    char t[MAX_TEXT]; // исходный текст
    int ptr = 0; // указатель текущей позиции в исходном тексте
    FILE* in;
public:
    TScanner() : ptr(0) {}
    void PutPtr(int i); //восстановить указатель
    int GetPtr(void); //запомнить указатель
    void PrintError(int i); //выдать сообщение об ошибке
    int Scanner(TypeLex l); //программа сканера
    void GetData(void); //ввод файла с исходным модулем
};

#endif 