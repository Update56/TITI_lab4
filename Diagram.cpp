#include "Diagram.hpp"
void TDiagram::Program() {
	TypeLex l; int t = 0;
	int nextType = lookForward(1);
	while (nextType != typeEnd) {	//пока следующая лексема - не конец данных
		if (nextType == typeInt || nextType == typeDouble || nextType == typeBool || nextType == typeChar) {	//если это тип данных
			int k = 2;
			while (lookForward(k) == typeInt || lookForward(k) == typeDouble || lookForward(k) == typeBool || lookForward(k) == typeChar) k++;	//пропускаем тип
			nextType = lookForward(k + 1);
			if (nextType == typeLeftBracket) {	//если круглая скобка - это функция
				Func();
			}
			else {	//иначе - это данные
				Data();
			}
		}
		else if (nextType == typeClass)
		{
			Class();
		}
		else if (nextType == typeConst)
		{
			t = sc->Scanner(l);
			NamedConst();
		}
		else if (nextType == typeEnd) {
			t = sc->Scanner(l);
			sc->PrintError("отсутствует main", l);
		}
		else {
			t = sc->Scanner(l);
			sc->PrintError("ожидался тип данных, class или const", l);
		}
		nextType = lookForward(1);
	}
	root->SetCur(start);
	root->Print(0, "начало программы");
	t = sc->Scanner(l);
}
void TDiagram::Class() {
	TypeLex l; int t;
	t = sc->Scanner(l);
	if (t != typeClass) sc->PrintError("ожидался Class", l);
	t = sc->Scanner(l);
	if (t != typeId) sc->PrintError("ожидался идентификатор класса", "");
	Tree* v = root->SemInclude(l, DataTypeEmpty, ObjClass); //добавляем класс в таблицу

	t = sc->Scanner(l);
	if (t != typeLeftBrace)
		sc->PrintError("ожидалась { в class", l);
	int sP = sc->SetPtr();
	t = sc->Scanner(l);
	while (t != typeRightBrace) {

		if (t == typeInt || t == typeDouble || t == typeBool || t == typeChar) {
			sc->GetPtr(sP);
			Data();
		}
		else if (t == typeVoid) {
			Method();
		}
		else if (t == typeConst) {
			NamedConst();
		}
		else if (t == typeEnd) {
			sc->PrintError("не завёршенный класс", "");
		}
		else if (t == typeClass)
		{
			sc->PrintError("Не поддерживаются вложенные классы", "");
		}
		else {
			sc->PrintError("ожидались data или method в class", l);
		}
		sP = sc->SetPtr();
		t = sc->Scanner(l);
	}
	t = sc->Scanner(l);
	if (t != typeSemicolon)
		sc->PrintError("ожидался символ ; (class)", l);
	root->SemReturn(v);
}

void TDiagram::Method() {
	TypeLex l; int t;
	DataType dataType;
	t = sc->Scanner(l);
	if (t != typeId) {
		sc->PrintError("ожидался идентификатор #1(met)", l);
	}
	Tree* v = root->SemInclude(l, DataTypeVoid, ObjMethod); // метод в таблицу

	t = sc->Scanner(l);
	if (t != typeLeftBracket) {
		sc->PrintError("ожидался символ ( (met)", l);
	}
	int type = lookForward(1);
	while (type != typeRightBracket)
	{
		dataType = Type();
		t = sc->Scanner(l);
		if (t != typeId) {
			sc->PrintError("ожидался идентификатор аргумента (met)", l);
		}
		Tree* v = root->SemInclude(l, dataType, ObjArg);
		t = sc->Scanner(l);
		if (t == typeComma)
		{
			continue;
		}
		else if (t == typeRightBracket)
			break;
		else
		{
			sc->PrintError("ожидалась , или ( (met)", l);
		}
	}
	if (type == typeRightBracket)
		t = sc->Scanner(l);
	CompOper();
	t = sc->Scanner(l);
	if (t != typeSemicolon)
		sc->PrintError("ожидался символ ; (met)", l);
	root->SemReturn(v);
}

void TDiagram::Data()	// данные
{
	TypeLex l; int t;
	DataType dataType;
	dataType = Type();	//тип данных
	do {	//пока не дойдем до точки с запятой - считываем список переменных через запятую
		Variable(dataType);
		t = sc->Scanner(l);
		if (t != typeSemicolon && t != typeComma && t != typeAccessOperator)
			sc->PrintError("В данных ожидался символ , ; . ", l);
	} while (t != typeSemicolon);
}

void TDiagram::Func()	// функция
{
	TypeLex l; int t; DataType dataType;
	int type = lookForward(1);
	TypeLex typeData;
	if (type != typeInt) {
		dataType = Type();
		t = sc->Scanner(l);
		if (t == typeMain) sc->PrintError("неподходящее возвращаемое значение main", "");
		else if (t != typeId) sc->PrintError("ожидался идентификатор функции", l);
	}
	else {
		dataType = Type();
		t = sc->Scanner(l);
		if (t != typeMain && t != typeId) sc->PrintError("ожидался идентификатор функции или main", l);
	}
	Tree* v = root->SemInclude(l, dataType, ObjFunct);
	t = sc->Scanner(l);
	if (t != typeLeftBracket) sc->PrintError("ожидался символ (", l);

	t = sc->Scanner(l);
	if (t != typeRightBracket) sc->PrintError("ожидался символ )", l);
	CompOper();
	root->SetCur(v);
}

DataType TDiagram::NamedConst()
{
	TypeLex l; int t;
	DataType type = Type();
	Tree* constant;
	t = sc->Scanner(l);
	if (t != typeId)
		sc->PrintError("ожидался идентификатор константы", l);

	if (type != DataTypeEmpty) {
		root->SemInclude(l, type, ObjConst);
	}
	constant = root->SemGetVar(l);
	if (constant == NULL) {
		sc->PrintError();
		constant = root->SemInclude(l, DataTypeEmpty, ObjConst);
	}
	int nextType = lookForward(1);
	if (nextType != typeEval) {
		t = sc->Scanner(l);
		sc->PrintError("Для константы требуется инициализатор", l);
	}
	else if (nextType == typeEval) {
		t = sc->Scanner(l);
		DataType type2 = Expression();
		root->SemControlTypeAssign(constant, type2);
		if (constant->GetType() == DataTypeEmpty)
			sc->PrintError();
	}
	t = sc->Scanner(l);
	return constant->GetType();
}

DataType TDiagram::Type()
{
	TypeLex l; int t;
	t = sc->Scanner(l);
	if (t != typeBool && t != typeDouble && t != typeInt && t != typeChar)
		sc->PrintError("ожидался тип данных (int, double, char)", l);
	else if (t == typeInt) {
		int nextType = lookForward(1);
		if (nextType == typeMain) {
			return DataTypeInt;
		}
		return DataTypeInt;
	}
	else if (t == typeDouble) {
		return DataTypeDouble;
	}
	else if (t == typeBool) {
		return DataTypeBool;
	}
	else if (t == typeChar) {
		return DataTypeChar;
	}
}

DataType TDiagram::Variable(DataType type, TypeLex lex)
{
	TypeLex l; int t;
	t = sc->Scanner(l);
	Tree* variable;
	if (t != typeId)
		sc->PrintError("ожидался идентификатор переменной", l);

	if (type != DataTypeEmpty) {
		root->SemInclude(l, type, ObjVar);
	}
	variable = root->SemGetVar(l);

	//для экземпляров класса
	if (variable->GetType() == DataTypeCustom && lex != NULL)
	{
		variable->SetRightTree((root->SemGetVar(lex))->GetRight());
	}

	if (variable == NULL) {
		sc->PrintError();
		variable = root->SemInclude(l, DataTypeEmpty, ObjVar);
	}
	int nextType = lookForward(1);
	if (nextType != typeEval)
		return variable->GetType();
	else if (nextType == typeEval) {
		t = sc->Scanner(l);
		DataType type2 = Expression();
		root->SemControlTypeAssign(variable, type2);
		if (variable->GetType() == DataTypeEmpty)
			sc->PrintError();
	}
	return variable->GetType();
}

void TDiagram::Assigment(Tree* var)	// присваивание
{
	Tree* variable;
	if (var == NULL)
	{
		TypeLex l; int t;
		t = sc->Scanner(l);
		if (t != typeId)
			sc->PrintError("ожидался идентификатор переменной", l);

		Tree* v = root->SemGetVar(l, false);
		if (v != NULL)
			if (v->GetObjType() == ObjConst)
				sc->PrintError("Ошибка нельзя изменить константу", l);

		variable = root->SemGetVar(l);	//проверка существования
		if (variable == NULL) {
			sc->PrintError();
			variable = root->SemInclude(l, DataTypeEmpty, ObjVar);
		}
		t = sc->Scanner(l);
		if (t != typeEval)
			sc->PrintError("ожидался символ =", l);
	}
	else {
		variable = var;
	}

	DataType type = Expression();
	root->SemControlTypeAssign(variable, type);
	if (variable->GetType() == DataTypeEmpty)
		sc->PrintError();
}


DataType TDiagram::Expression()	// выражение
{
	TypeLex l; int t;
	DataType type1 = Comparison();
	int nextType = lookForward(1);
	while ((nextType == typeNotEq) || (nextType == typeEq))	//пока следующая лексема != или ==
	{
		t = sc->Scanner(l);
		int zn = t;
		DataType type2 = Comparison();
		DataType type3 = root->SemGetTypeExpr(type1, type2, zn);
		if (type1 != DataTypeEmpty && type2 != DataTypeEmpty && type3 == DataTypeEmpty) sc->PrintError();
		type1 = type3;
		nextType = lookForward(1);
	}
	return type1;
}

DataType TDiagram::Comparison()	// сравнение
{
	TypeLex l; int t;
	DataType type1 = Shift();
	int nextType = lookForward(1);
	while ((nextType <= typeMore) && (nextType >= typeLessOrEq))	//пока следующая лексема <, >, <= или >=
	{
		t = sc->Scanner(l);
		int zn = t;
		DataType type2 = Shift();
		DataType type3 = root->SemGetTypeExpr(type1, type2, zn);
		if (type1 != DataTypeEmpty && type2 != DataTypeEmpty && type3 == DataTypeEmpty) sc->PrintError();
		type1 = type3;
		nextType = lookForward(1);
	}
	return type1;
}

DataType TDiagram::Shift()	// сдвиг
{
	TypeLex l; int t;
	DataType type1 = Term();
	int nextType = lookForward(1);
	while ((nextType == typeShiftLeft) || (nextType == typeShiftRight))	//пока следующая лексема << или >>
	{
		t = sc->Scanner(l);
		int zn = t;
		DataType type2 = Term();
		nextType = lookForward(1);
	}
	return type1;
}

DataType TDiagram::Term()	// слагаемое
{
	TypeLex l; int t;
	DataType type1 = Factor();
	int nextType = lookForward(1);
	while ((nextType == typePlus) || (nextType == typeMinus))	//пока следующая лексема + или -
	{
		t = sc->Scanner(l);
		int zn = t;
		DataType type2 = Factor();
		DataType type3 = root->SemGetTypeExpr(type1, type2, zn);
		if (type1 != DataTypeEmpty && type2 != DataTypeEmpty && type3 == DataTypeEmpty) sc->PrintError();
		type1 = type3;
		nextType = lookForward(1);
	}
	return type1;
}

DataType TDiagram::Factor()	// множитель
{
	TypeLex l; int t;
	int nextType = lookForward(1);
	if ((nextType == typePlus) || (nextType == typeMinus)) {	//считываем + или -, если они есть
		t = sc->Scanner(l);
	}
	DataType type1 = Elementary();
	nextType = lookForward(1);
	while ((nextType <= typeMul) && (nextType >= typeMod))	//пока следующая лексема *, / или %
	{
		t = sc->Scanner(l);
		int zn = t;
		DataType type2 = Elementary();
		DataType type3 = root->SemGetTypeExpr(type1, type2, zn);
		if (type1 != DataTypeEmpty && type2 != DataTypeEmpty && type3 == DataTypeEmpty) sc->PrintError();
		type1 = type3;
		nextType = lookForward(1);
	}
	return type1;
}

DataType TDiagram::Elementary()	// эл.выражения
{
	TypeLex l; int t;
	int nextType = lookForward(1);
	if ((nextType == constInt)) {
		t = sc->Scanner(l); //если это константа - просто считываем
		return DataTypeInt;
	}
	else if ((nextType == constHex)) {
		t = sc->Scanner(l); //если это константа - просто считываем
		return DataTypeInt;
	}
	else if (nextType == typeLeftBracket)	//если есть "(" - это выражение в скобках
	{
		t = sc->Scanner(l);
		DataType type = Expression();
		t = sc->Scanner(l);
		if (t != typeRightBracket) sc->PrintError("ожидался символ )", l);
		return type;
	}
	else {	//иначе - это вызов функции, если есть "(", иначе - переменная
		DataType type;
		nextType = lookForward(2);
		if (nextType == typeLeftBracket) {
			type = FuncCall();
		}
		else if (nextType == typeAccessOperator)
		{
			t = sc->Scanner(l);
			Tree* var = ClassAccsess(l);
			return var->GetType();
		}
		else {
			type = Variable(DataTypeEmpty);
		}
		return type;
	}
}

DataType TDiagram::FuncCall()	// вызов функции
{
	TypeLex l; int t;
	t = sc->Scanner(l);
	if (t != typeId)
		sc->PrintError("ожидался идентификатор функции", l);

	DataType type = root->GetType();
	std::vector<DataType> typeParam;
	int num = 0;
	t = sc->Scanner(l);
	if (t != typeLeftBracket) sc->PrintError("ожидался символ (", l);
	int nextType = lookForward(1);
	while (nextType != typeRightBracket) {	//считывание параметров функции как список выражений через запятую
		typeParam.push_back(Expression());
		num++;
		nextType = lookForward(1);
		if (nextType != typeRightBracket && nextType != typeComma) {
			t = sc->Scanner(l);
			sc->PrintError("В вызове функции ожидался символ , или )", l);
		}
		if (nextType == typeComma) t = sc->Scanner(l);
	}
	t = sc->Scanner(l);
	return type;
}

void TDiagram::CompOper()	// составной оператор
{
	TypeLex l; int t;
	t = sc->Scanner(l);
	if (t != typeLeftBrace) sc->PrintError("в составном операторе ожидался символ {", l);
	int nextType = lookForward(1);
	while (nextType != typeRightBrace) { //пока следующей лексемой не будет закрывающаяся фигурная скобка - считывание данных и операторов
		if (nextType == typeEnd) {
			t = sc->Scanner(l);
			sc->PrintError("в составном операторе ожидался символ }", "");
		}
		if (nextType == typeBool || nextType == typeDouble || nextType == typeChar)
			Data();
		else if (nextType == typeConst)
		{
			t = sc->Scanner(l);
			NamedConst();
		}
		else if (nextType == typeClass)
			Class();
		else {
			Operator();
		}

		nextType = lookForward(1);
	}
	t = sc->Scanner(l);
}

void TDiagram::Operator()	// оператор
{
	TypeLex l; int t;
	int nextType = lookForward(1);
	if (nextType == typeDo)
	{
		DoWhile();  // если следующая лексема do while
		return;
	}
	else if (nextType == typeLeftBrace) {
		Tree* tmp = root->SemInCompOperator();
		CompOper();	//если фигурная скобка - составной оператор
		root->SemReturn(tmp);
	}
	else if (nextType == typeId) {
		nextType = lookForward(2);
		if (nextType == typeLeftBracket) {	//если круглая скобка - вызов функции
			FuncCall();
			t = sc->Scanner(l);
		}
		else if (nextType == typeEval) { //если равно - присваивание
			Assigment();
			t = sc->Scanner(l);
		}
		else if (nextType == typeAccessOperator) { //если точка оператор доступа
			t = sc->Scanner(l);
			if (root->SemGetVar(l, false)->GetObjType() == ObjClass) {
				sc->PrintError("Является идентификатором класса", l);
			}
			Tree* var = ClassAccsess(l);
			t = sc->Scanner(l);
			if (var->GetObjType() == ObjMethod) {
				int countArg = 0;
				auto ArgList = var->GetListArgs(var->GetRight());
				do
				{
					t = sc->Scanner(l);
					if (t == typeComma) {
						countArg++;
					}
				} while (t != typeRightBracket);
				if (countArg != ArgList.size() - 1) {
					std::string name = var->GetObjInfo()->id;
					name = name + ": неверное кол-вл аргументов метода";
					sc->PrintError(name.c_str());
				}
				t = sc->Scanner(l);
			}
			else if (t == typeEval)
			{
				if (var->GetObjType() == ObjConst)
				{
					sc->PrintError("Ошибка нельзя изменить константу", l);
				}
				Assigment(var);
				t = sc->Scanner(l);
			}
		}
		else if (nextType == typeId)
		{
			t = sc->Scanner(l);
			Tree* f = root->Cur->FindUp(l);
			if (f == NULL)
			{
				sc->PrintError("Неверный идентификатор", l);
			}
			Variable(DataTypeCustom, l);
			t = sc->Scanner(l);
		}
		else {
			t = sc->Scanner(l);
			sc->PrintError("ожидался идентификатор", l);
		}

		if (t != typeSemicolon)
			sc->PrintError("после оператора ожидался символ ;", l);
	}
	else if (nextType == typeSemicolon) {
		t = sc->Scanner(l);
		return;
	}
	else if (nextType == typeWhile) {
		t = sc->Scanner(l);
		return;
	}
	else {
		t = sc->Scanner(l);
		sc->PrintError("Ошибка в операторе", l);
	}
}

Tree* TDiagram::ClassAccsess(TypeLex lex) {
	TypeLex l; int t;
	Tree* f = root->SemGetVar(lex); //находим экзепляр класса
	t = sc->Scanner(l);
	if (t != typeAccessOperator) {
		sc->PrintError("Ожидался оператор доступа", l);
	}
	t = sc->Scanner(l);
	f = f->GetRight(); //переходим ветку с классом
	f = f->FindDownOneLevel(f, l); //ищем член класса
	if (f == NULL) {
		printf("ОШИБКА: обращение к отсутствующему члену класса %s \n", l);
		exit(0);
	}
	return f; //возвращаем элемент
}

int TDiagram::lookForward(int k) {	//определение типа следущей лексемы на k позиции от текущего указателя
	TypeLex l;
	int savedPointer = sc->SetPtr();
	int nextType;
	for (int i = 0; i < k; i++)
		nextType = sc->Scanner(l);
	sc->GetPtr(savedPointer);
	return nextType;
}

void TDiagram::DoWhile()
{
	TypeLex l; int t;
	t = sc->Scanner(l);
	if (t != typeDo)
		sc->PrintError("ожидался do", l);
	CompOper();
	t = sc->Scanner(l);
	if (t != typeWhile)
		sc->PrintError("ожидался while", l);
	t = sc->Scanner(l);
	if (t != typeLeftBracket)
		sc->PrintError("ожидалась (", l);
	if (lookForward(1) == typeRightBracket)
		sc->PrintError("Ожидалось сравнение в while(<>)", "");
	Expression();
	t = sc->Scanner(l);
	if (t != typeRightBracket)
		sc->PrintError("ожидалась )", l);
	t = sc->Scanner(l);
	if (t != typeSemicolon) sc->PrintError("ожидалась ;", l);
}