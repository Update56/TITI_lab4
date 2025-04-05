#pragma once
#ifndef __SEMAN
#define __SEMAN
#include "defs.hpp"
#include <vector>

enum TypeObject {
	ObjEmpty,
	ObjVar,
	ObjFunc	
};

enum DataType {
	TypeEmpty,
	TypeDouble, 
	TypeChar
};

struct Node // информация в вершине дерева
{
	TypeLex id;				// идентификатор объекта
	TypeObject TypeObject;	// тип объекта
	DataType DataType;		// тип значения
	char* Data;				// ссылка на значение или NULL
};

class Tree // элемент семантической таблицы
{
private:
	Node* n; // информация об объекте таблицы
	Tree* Up, * Left, * Right;
	// родитель, левый и правый потомок

public:
	static Tree* Cur; // текущий элемент дерева
	Tree(Tree* l, Tree* r, Tree* u, Node* Data);	// конструктор создает узел с заданными связями и данными
	Tree();											// конструктор создает новый узел с пустыми связями и данными
	~Tree();										// деструтор дерева
	void SetLeft(Node* Data);						// создать левого потомка от текущей вершины
	void SetRight(Node* Data);						// создать правого потомка от текущей вершины
	Tree* FindUp(TypeLex id);						// поиск данных в дереве, начиная от текущей вершины
	Tree* FindUp(Tree* From, TypeLex id);			// поиск данных в дереве от заданной вершины
	Tree* FindUpOneLevel(Tree* From, TypeLex id);	// поиск данных в дереве от заданной вершины по области видимости
	int Print(int maxI);							// отладочная программа печати дерева
	void SetCur(Tree* a);							// установить текущий узел дерева
	Tree* GetCur(void);								// получить значение текущего узла дерева
	Tree* SemGetVar(TypeLex a);						// найти в таблице переменную
	int DupControl(Tree* Addr, TypeLex a);			// проверка идентификатора на повторное описание внутри блока
	void PrintError(const char*, TypeLex a);		// вывод ошибок с идентификатором
	void PrintError(const char*);					// вывод ошибок без идентификатора
	DataType GetType();								// получение типа данных идентификатора

	DataType SemGetTypeExpr(DataType t1, DataType t2, int oper);	// получение типа результата операции над выражениями (Sem2)
	void SemControlTypeAssign(Tree* t, DataType t2);				// проверка соответствия типов при присваивании (Sem3)

	Tree* SemInclude(TypeLex a, DataType t, TypeObject type);		// занесение идентификатора a в таблицу с типом t (Sem7)
	Tree* SemInCompOperator();						// создание нового уровня видимости в составном операторе (Sem8)
	void SemOutCompOperator(Tree* tmp);				// выход на предыдущий уровень видимости из составного оператора (Sem9)
};

#endif