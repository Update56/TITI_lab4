
#include "Semant.hpp"
#define max(a,b) a<b? b : a
Tree* Tree::Cur = nullptr;
Tree::Tree(Tree* l, Tree* r, Tree* u, Node* Data)
{
	n = new Node();
	Up = u; Left = l; Right = r; // установили ссылки
	memcpy(n, Data, sizeof(Node)); // установили данные
}

Tree::Tree()
{
	n = new Node();
	memcpy(n->id, &"", 2); n->TypeObject = ObjEmpty; n->DataType = TypeEmpty; n->Data = NULL;
	Up = Cur; Left = NULL; Right = NULL;
}

Tree::~Tree()
{
	if (Left != NULL) delete Left;
	if (Right != NULL) delete Right;
	if (n != NULL)   delete n;

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

Tree* Tree::FindUp(Tree* From, TypeLex id)
{
	Tree* i = From; // текущая вершина поиска
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0)) {
		i = i->Up; // поднимаемся наверх по связям
	}
	return i;

}

Tree* Tree::FindUpOneLevel(Tree* From, TypeLex id)
{
	Tree* i = From; // текущая вершина поиска
	while ((i != NULL) && (i->Up != NULL) &&
		(i->Up->Right != i)
		)
	{
		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i; // нaшли совпадающий идентификатор
		i = i->Up; // поднимаемся наверх по связям
	}
	return NULL;
}

Tree* Tree::FindUp(TypeLex id)
{
	return FindUp(this, id);
}

int Tree::Print(int maxI)	//вывод дерева с нумерацией пустых вершин
{
	if (n->TypeObject != ObjEmpty) printf("Вершина с данными %s -->", n->id);
	else printf("Вершина с данными пустая вершина  -->");

	if (Left != NULL) {
		if (Left->n->TypeObject != ObjEmpty) printf(" слева данные %s", Left->n->id);
		else {
			printf(" слева пустая вершина ");

			maxI++;
		}
	}
	if (Right != NULL) {
		if (Right->n->TypeObject != ObjEmpty) printf(" справа данные %s", Right->n->id);
		else {
			printf(" справа пустая вершина ");

			maxI++;
		}
	}
	printf("\n");
	if (Left != NULL) maxI = Left->Print(maxI);
	if (Right != NULL) maxI = Right->Print(maxI);
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

Tree* Tree::SemInclude(TypeLex a, DataType t, TypeObject TypeObject)
{
	if (DupControl(Cur, a))
		PrintError("Повторное описание идентификатора ", a);
	Tree* v; Node* b = new Node();
	if (TypeObject != ObjFunc)
	{
		memcpy(b->id, a, strlen(a) + 1); b->TypeObject = TypeObject;  b->DataType = t; b->Data = NULL;

		Cur->SetLeft(b); // сделали вершину - переменную
		Cur = Cur->Left;
		return Tree::Cur;
	}
	else
	{
		memcpy(b->id, a, strlen(a) + 1); b->TypeObject = TypeObject; b->DataType = t; b->Data = NULL;

		Cur->SetLeft(b); // сделали вершину - функцию
		Cur = Cur->Left;
		v = Cur; // это точка возврата после выхода из функции
		if (t != TypeEmpty) {	//новая область видимости
			memcpy(b->id, &"", 2); b->TypeObject = ObjEmpty; b->DataType = TypeEmpty; b->Data = NULL;

			Cur->SetRight(b); // сделали пустую вершину
			Cur = Cur->Right;
		}
		return v;
	}
}


Tree* Tree::SemGetVar(TypeLex a)
{
	Tree* v = FindUp(Cur, a);
	if (v == NULL) {
		printf("ОШИБКА: Отсутствует описание идентификатора %s ", a);
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

void Tree::SemOutCompOperator(Tree* tmp)
{
	Cur = tmp;
}

void Tree::PrintError(const char* s, TypeLex a)
{
	printf("ОШИБКА: %s - %s \n", s, a);

}
void Tree::SemControlTypeAssign(Tree* t, DataType t2) {
	if (t->n->DataType == TypeChar && t2 == TypeDouble) {
		printf("Возможна потеря данных - присваивание char = double\n");
		t->n->DataType = TypeEmpty;
	}
}

void Tree::PrintError(const char* s)
{
	printf("ОШИБКА: %s ", s);
	throw("ОШИБКА");
}

DataType Tree::GetType()
{
	return this->n->DataType;
}

DataType Tree::SemGetTypeExpr(DataType t1, DataType t2, int oper) {
	if (t1 == TypeEmpty || t2 == TypeEmpty) 
		return TypeEmpty;
	if (oper >= typeNotEq && oper <= typeMore) {
		return TypeDouble; // Операции сравнения возвращают double
	}
	else if (oper >= typePlus && oper <= typeMul) {

		if (t1 == TypeChar && t2 == TypeChar)
			return TypeChar;

		if ((t1 == TypeChar && t2 == TypeDouble) || 
			(t1 == TypeDouble && t2 == TypeChar) ||
			(t1 == TypeDouble && t2 == TypeDouble))
			return TypeDouble;

		printf("Несовместимые типы данных для операции\n");
		return TypeEmpty;
	}
	else if (oper == typeEval)
	{
		if (t1 == TypeChar && t2 == TypeChar)
			return TypeChar;

		if ((t1 == TypeChar && t2 == TypeDouble) ||
			(t1 == TypeDouble && t2 == TypeChar) ||
			(t1 == TypeDouble && t2 == TypeDouble))
			return TypeDouble;
	}
	printf("Неизвестный оператор\n");
	return TypeEmpty;
};