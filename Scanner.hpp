#ifndef __SCANNER
#define __SCANNER
#include "defs.hpp"
class TScanner {
private:
	TypeMod t; // исходный текст
	int ptr; // указатель текущей позиции в исходном тексте
	int ptrLine;	//значение указателя в начале текущей строки
public:
	int currentLine;	//текущая строка
	void GetPtr(int i);
	int SetPtr(void);
	void PrintError(const char*, const char*);
	void PrintError(const char*);
	void PrintError();
	int Scanner(TypeLex l);
	void GetData(const char*);
	TScanner(const char*);
	~TScanner() {}
};
#endif
