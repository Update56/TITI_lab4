#ifndef __SEMAN
#define __SEMAN
#include "defs.hpp"
#include <vector>
#include <string>
enum TypeObject {
	ObjEmpty = 0,
	ObjVar,		//переменная
	ObjFunct,	//функция
	ObjClass,	//класс
	ObjMethod,	//метод класса
	ObjConst,	//именованая константа
	ObjArg,		//аргумент метода
};
enum DataType {
	DataTypeEmpty,  
	DataTypeDouble, 
	DataTypeBool,
	DataTypeInt,
	DataTypeChar,
	DataTypeVoid,
	DataTypeCustom,
};
struct Node // информация в вершине дерева
{
	TypeLex id; // идентификатор объекта
	TypeObject TypeObj; // тип объекта
	DataType DataType; // тип значения
	char* Data; // ссылка на значение или NULL
};
class Tree // элемент семантической таблицы
{
private:
	Node* info; // информация об объекте таблицы
	Tree* Up, * Left, * Right;
	// родитель, левый и правый потомок
public:
	static Tree* Cur; // текущий элемент дерева
	Tree(Tree* l, Tree* r, Tree* u, Node* Data);	// конструктор создает узел с заданными связями и данными
	Tree();	// конструктор создает новый узел с пустыми связями и данными
	void SetLeft(Node* Data);		// создать левого потомка от текущей вершины
	void SetRight(Node* Data);		// создать правого потомка от текущей вершины
	void SetRightTree(Tree* vert);	// установить правого потомка
	Tree* GetRight(void);			// получить правого потомка
	Tree* GetLeft(void);			// получить левого потомка
	Tree* FindUp(Tree* From, TypeLex id);			// поиск данных в дереве от заданной вершины
	Tree* FindUpOneLevel(Tree* From, TypeLex id);	// поиск данных в дереве от заданной вершины по области видимости
	Tree* FindUp(TypeLex id);		// поиск данных в дереве, начиная от текущей вершины
	Tree* FindDownOneLevel(Tree* From, TypeLex id); // поиск данных в дереве от заданной вершины по области видимости сверху вниз
	int Print(int maxI, const char*);			// отладочная программа печати дерева
	void SetCur(Tree* a);			// установить текущий узел дерева
	Tree* GetCur(void);				// получить значение текущего узла дерева
	Tree* SemInclude(TypeLex a, DataType t, TypeObject type); // занесение идентификатора a в таблицу с типом t
	Tree* SemGetVar(TypeLex a, bool err = true);		// найти в таблице переменную
	int DupControl(Tree* Addr, TypeLex a); // проверка идентификатора на повторное описание внутри блока
	Tree* SemInCompOperator();	//создание нового уровня видимости в составном операторе
	DataType SemGetTypeExpr(DataType t1, DataType t2, int oper);	//получение типа результата операции над выражениями
	void SemControlTypeAssign(Tree* t, DataType t2);	//проверка соответствия типов при присваивании
	void SemReturn(Tree* tmp);	//выход на предыдущий уровень видимости из составного оператора
	void PrintError(const char*, TypeLex a);	//вывод ошибок с идентификатором
	void PrintError(const char*);	//вывод ошибок без идентификатора
	DataType GetType();	//получение типа данных идентификатора
	TypeObject GetObjType(); //получение типа объекта
	Node* GetObjInfo(); //получение типа объекта
	std::vector<DataType> GetListArgs(Tree* met); //получить лист аргемнтов
};

#endif

