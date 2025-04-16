#ifndef __SCANNER
#define __SCANNER
#include "defs.hpp"
class TScanner {
private:
	TypeMod t; // исходный текст
	int ptr; // указатель текущей позиции в исходном тексте
	int currentLine;	//текущая строка
	int ptrLine;	//значение указателя в начале текущей строки
public:
	void GetPtr(int i);
	int SetPtr(void);
	void PrintError(const char*, const char*);
	void PrintError();
	int Scanner(TypeLex l);
	void GetData(const char*);
	TScanner(const char*);
	~TScanner() {}
};
#endif
