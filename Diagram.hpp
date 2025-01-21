#ifndef __DIAGRAM
#define __DIAGRAM
#include "Scanner.hpp"
#include "defs.hpp"
#include "vector"
#include "array"


class Diagram
{
private:
	Scanner* poss;
	Scanner* sc;
	TypeLex l; // “окен
	int t;     //  од текущего токена
	int bracketCount = 0;
	int previousToken;
	int previousPtr = 0;
	vector<array<int, 2> > lexcemes; //массив лексем [0] - код лексемы, [1] - позици€ курсора начала этой лексемы
	
public:
	Diagram(Scanner* s) { sc = s; }
	~Diagram() {}
	void SetToken();	//получить токен
	void GetToken();	//возвращаем предыдущий токен
	void Program();		//1.программа
	void Data();		//2.данные
	void Type();		//3.тип
	void Func();		//4.функци€
	void NamedConst();	//5.именованные константы	
	void Class();		//6.класс
	void Method();		//7.метод
	void Operator();	//8.оператор
	void Assigment();	//9.присваивание
	void Variable();	//10.переменные
	void Expression();	//11.выражение
	void Comparison();	//12.сравнение
	void Shift();		//13.сдвиг
	void Term();		//14.слагаемое
	void Factor();		//15.множитель
	void Elementary();	//16.элементарные выражени
	void Unary();		//17.унарные операции
	void CompOper();	//20.cоставной оператор+
};
#endif