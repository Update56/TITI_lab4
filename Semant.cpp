#include "Semant.hpp"
#define max(a,b) a<b? b : a
Tree* Tree::Cur = nullptr;
Tree::Tree(Tree* l, Tree* r, Tree* u, Node* Data)
{
	info = new Node();
	Up = u; Left = l; Right = r; // установили ссылки
	memcpy(info, Data, sizeof(Node)); // установили данные
}

Tree::Tree()
{
	info = new Node();
	memcpy(info->id, &"", 2); info->TypeObj = ObjEmpty; info->DataType = DataTypeEmpty; info->Data = NULL;
	Up = Cur; Left = NULL; Right = NULL;
}

void Tree::SetLeft(Node* Data)
{
	Tree* a = new Tree(NULL, NULL, this, Data); // новая вершина
	Left = a;
}

void Tree::SetRight(Node* Data)
{
	Tree* a = new Tree(NULL, NULL, this, Data); // новая вершина
	Right = a;
}

void Tree::SetRightTree(Tree* vert)
{
	this->Right = vert;
}

Tree* Tree::GetRight(void)
{
	return this->Right;
}

Tree* Tree::GetLeft(void)
{
	return this->Left;
}

Tree* Tree::FindUp(Tree* From, TypeLex id)
{
	Tree* i = From; // текущая вершина поиска
	while ((i != NULL) &&
		(memcmp(id, i->info->id, max(strlen(i->info->id), strlen(id))) != 0)) {
		i = i->Up; // поднимаемся наверх по связям
	}
	return i;
}

Tree* Tree::FindUpOneLevel(Tree* From, TypeLex id)
{
	Tree* i = From; // текущая вершина поиска
	while ((i != NULL) && (i->Up != NULL) && (i->Up->Right != i)) {
		if (memcmp(id, i->info->id, max(strlen(i->info->id), strlen(id))) == 0)
			return i; // нaшли совпадающий идентификатор
		i = i->Up; // поднимаемся наверх по связям
	}
	return NULL;
}

Tree* Tree::FindUp(TypeLex id)
{
	return FindUp(this, id);
}

Tree* Tree::FindDownOneLevel(Tree* From, TypeLex id)
{
	Tree* i = From; // текущая вершина поиска
	while (i != NULL) {
		if (memcmp(id, i->info->id, max(strlen(i->info->id), strlen(id))) == 0)
			return i; // нaшли совпадающий идентификатор
		i = i->Left; // опускаемся вниз левым по связям
	}
	return NULL;
}

int Tree::Print(int maxI, const char* id)	//вывод дерева с нумерацией пустых вершин
{
	if (info->TypeObj != ObjEmpty)
		printf("Вершина с данными \"%s\" -->", info->id);
	else
		printf("Вершина с данными пустая вершина (%s) -->", id);

	if (Left != NULL) {
		if (Left->info->TypeObj != ObjEmpty)
			printf(" слева данные \"%s\"", Left->info->id);
		else {
			printf(" слева пустая вершина ");
			maxI++;
		}
	}
	if (Right != NULL && info->TypeObj != ObjClass) {
		if (Right->info->TypeObj != ObjEmpty)
			printf(" справа данные \"%s\"", Right->info->id);
		else {
			printf(" справа пустая вершина ");
			maxI++;
		}
	}
	printf("\n");
	if (Left != NULL)
		maxI = Left->Print(maxI, info->id);
	if (Right != NULL && info->TypeObj != ObjClass)
		maxI = Right->Print(maxI, info->id);
	return maxI;
}

void Tree::SetCur(Tree* a)
{
	Cur = a;
}

Tree* Tree::GetCur(void)
{
	return Cur;
}

Tree* Tree::SemInclude(TypeLex a, DataType t, TypeObject type)
{
	if (DupControl(Cur, a))
		PrintError("Повторное описание идентификатора ", a);
	Tree* v; Node* b = new Node();
	if (type != ObjFunct && type != ObjClass && type != ObjMethod)
	{
		memcpy(b->id, a, strlen(a) + 1); b->TypeObj = type; b->DataType = t; b->Data = NULL;

		Cur->SetLeft(b); // сделали вершину - переменную
		Cur = Cur->Left;
		return Tree::Cur;
	}

	else if (type == ObjClass)
	{
		memcpy(b->id, a, strlen(a) + 1); b->TypeObj = type; b->DataType = t; b->Data = NULL;
		Cur->SetLeft(b); // сделали вершину - класс
		Cur = Cur->Left;
		v = Cur;
		memcpy(b->id, &"", 2); b->TypeObj = ObjEmpty; b->DataType = DataTypeEmpty; b->Data = NULL;

		Cur->SetRight(b); // сделали пустую вершину
		Cur = Cur->Right;

		return v;
	}
	else if (type == ObjMethod)
	{
		memcpy(b->id, a, strlen(a) + 1); b->TypeObj = type; b->DataType = t; b->Data = NULL;

		Cur->SetLeft(b); // сделали вершину - метод
		Cur = Cur->Left;
		v = Cur; // это точка возврата после выхода 
		if (t != DataTypeEmpty) {	//новая область видимости
			memcpy(b->id, &"", 2); b->TypeObj = ObjEmpty; b->DataType = DataTypeEmpty; b->Data = NULL;

			Cur->SetRight(b); // сделали пустую вершину
			Cur = Cur->Right;
		}
		return v;
	}
	else
	{
		memcpy(b->id, a, strlen(a) + 1); b->TypeObj = type; b->DataType = t; b->Data = NULL;

		Cur->SetLeft(b); // сделали вершину - функцию
		Cur = Cur->Left;
		v = Cur; // это точка возврата после выхода из функции
		if (t != DataTypeEmpty) {	//новая область видимости
			memcpy(b->id, &"", 2); b->TypeObj = ObjEmpty; b->DataType = DataTypeEmpty; b->Data = NULL;

			Cur->SetRight(b); // сделали пустую вершину
			Cur = Cur->Right;
		}
		return v;
	}
}

Tree* Tree::SemGetVar(TypeLex a, bool err)
{
	Tree* v = FindUp(Cur, a);
	if (v == NULL) {
		if (err)
		{
			printf("ОШИБКА: Отсутствует описание идентификатора %s \n", a);
			exit(0);
		}
		return NULL;
	}
	return v;
}

int Tree::DupControl(Tree* Addr, TypeLex a)
{
	if (FindUpOneLevel(Addr, a) == NULL) return 0;
	return 1;
}

Tree* Tree::SemInCompOperator()
{
	Cur->Left = new Tree();
	Cur = Cur->Left;
	Tree* tmp = Cur;
	Cur->Right = new Tree();
	Cur = Cur->Right;
	return tmp;
}

void Tree::SemReturn(Tree* tmp)
{
	Cur = tmp;
}

void Tree::PrintError(const char* s, TypeLex a)
{
	printf("ОШИБКА: %s - %s \n", s, a);
}
void Tree::SemControlTypeAssign(Tree* t, DataType t2) {

	if (t->info->DataType == DataTypeDouble && t2 == DataTypeChar) {
		printf("Возможна потеря данных - присваивание double = char: ");
		t->info->DataType = DataTypeEmpty;
	}
}

void Tree::PrintError(const char* s)
{
	printf("ОШИБКА: %s ", s);
	throw("ОШИБКА");
}

DataType Tree::GetType()
{
	return this->info->DataType;
}

TypeObject Tree::GetObjType()
{
	return this->info->TypeObj;
}

Node* Tree::GetObjInfo()
{
	return this->info;
}

std::vector<DataType> Tree::GetListArgs(Tree* met)
{
	std::vector<DataType> TypeList;
	Tree* check = met->Left;
	while (check->GetObjType() == ObjArg)
	{
		TypeList.push_back(check->GetType());
		if (check->Left != NULL)
			check = check->Left;
		else
			return TypeList;
	}
	return TypeList;
}

DataType Tree::SemGetTypeExpr(DataType t1, DataType t2, int oper) {
	if (t1 == DataTypeEmpty || t2 == DataTypeEmpty)
		return DataTypeEmpty;

	if (oper >= typeNotEq && oper <= typeMore)
		return DataTypeDouble; // Операции сравнения возвращают булевый результат

	else if (oper == typePlus || oper == typeMinus || oper == typeMul || oper == typeDiv || oper == typeMod) {
		if (t1 == DataTypeChar && t2 == DataTypeChar)
			return DataTypeChar;

		if ((t1 == DataTypeChar && t2 == DataTypeDouble) ||
			(t1 == DataTypeDouble && t2 == DataTypeChar) ||
			(t1 == DataTypeDouble && t2 == DataTypeDouble))
			return DataTypeDouble;

		if ((t1 == DataTypeInt && t2 == DataTypeDouble) ||
			(t1 == DataTypeDouble && t2 == DataTypeInt))
			return DataTypeDouble;

		if ((t1 == DataTypeInt && t2 == DataTypeChar) ||
			(t1 == DataTypeChar && t2 == DataTypeInt))
			return DataTypeChar;

		printf("Несовместимые типы данных для операции\n");
		return DataTypeEmpty;
	}
	printf("Неизвестный оператор\n");
	return DataTypeEmpty;
};

