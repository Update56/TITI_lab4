#include "Diagram.hpp"
#include "iostream"
using namespace std;

void Diagram::SetToken() {
	previousToken = t; //сохраняем в предыдущий токен
	previousPtr = sc->SetPtr(); //сохраняем в предыдущий курсор 
	t = sc->Scanning(l); //сканирование токена
	lexcemes.push_back({ t, previousPtr });
}

void Diagram::GetToken() {
	t = previousToken; //возвращаем предыдущий токен
	sc->GetPtr(previousPtr); //возвращаем предыдущий указатель
}

void Diagram::Type() {
	if ((t == typeDouble) || (t == typeChar) || (t == typeId) || (t == typeVoid)) {
		return;
	}
	else {
		sc->PrintError("ожидался тип данных(double, char) или индентификатор (type)", l);
	}
}

void Diagram::Program() {
	SetToken(); sc->PPP();
	Type();
	if (t == typeVoid) {
		SetToken(); sc->PPP();
		if (t == typeMain) {
			Func();
			return;
		}
		else {
			sc->PrintError("Ожидался символ main (prog)", l);
		}
	}
	else if ((t == typeDouble) || (t == typeChar) || (t == typeId)) {
		Data();
	}
	else if (t == typeClass) {
		Class();
	}
	else if (t == typeConst) {
		NamedConst();
	}
	else {
		sc->PrintError("ожидался тип данных (double, char), индификатор, класс, именованная константа (prog)", l);
	}
}
void Diagram::Data() {
	do {
		SetToken(); sc->PPP();
		if (t != typeId)
			sc->PrintError("ожидался идентификатор (data)", l);
		SetToken(); sc->PPP();
		if (t == typeEval) {
			Expression();
		}
		if (t == typeId) {
			SetToken(); sc->PPP();
		}
	} while (t == typeComma);
	if (t != typeSemicolon)
		sc->PrintError("ожидался символ ; (data)", l);
}

void Diagram::Func() {
	SetToken(); sc->PPP();
	if (t != typeLeftBracket)
		sc->PrintError("ожидался символ ( (func)", l);
	SetToken(); sc->PPP();
	if (t != typeRightBracket)
		sc->PrintError("ожидался символ ) (func)", l);
	SetToken(); sc->PPP();

	if (t == typeLeftBrace)
		CompOper();
}

void Diagram::NamedConst()
{
	SetToken(); sc->PPP();
	Type();
	SetToken(); sc->PPP();
	if (t != typeId) {
		sc->PrintError("ожидался идентификатор (NC)", l);
	}
	SetToken(); sc->PPP();
	if (t == typeEval) {
		Expression();
	}
	else {
		sc->PrintError("ожидался знак = (NC)", l);
	}
	if (t == constInt || t == constHex) {
		SetToken(); sc->PPP();
		return;
	}
	if (t != typeSemicolon)
		sc->PrintError("ожидался символ ; (NC)", l);
}

void Diagram::Class()
{
	SetToken(); sc->PPP();
	if (t != typeId) {
		sc->PrintError("ожидался идентификатор (class)", l);
	}
	SetToken(); sc->PPP();

	if (t != typeLeftBrace)
	{
		sc->PrintError("ожидался символ { (class)", l);
	}
	SetToken(); sc->PPP();
	while (t != typeRightBrace)
	{
		if ((t == typeDouble) || (t == typeChar) || (t == typeId)) {
			Data();
		}
		else if (t == typeVoid) {
			Method();
		}
		else if (t == typeConst) {
			NamedConst();
		}
		else
		{
			sc->PrintError("ожидались data или method", l);
			break;
		}
		SetToken(); sc->PPP();
	}
	SetToken(); sc->PPP();
	if (t != typeSemicolon)
		sc->PrintError("ожидался символ ; (class)", l);
}

void Diagram::Method()
{
	SetToken(); sc->PPP();
	if (t != typeId) {
		sc->PrintError("ожидался идентификатор #1(met)", l);
	}
	SetToken(); sc->PPP();

	if (t != typeLeftBracket)
	{
		sc->PrintError("ожидался символ ( (met)", l);
	}
	SetToken(); sc->PPP();
	while (t != typeRightBracket)
	{
		Type();
		SetToken(); sc->PPP();
		if (t == typeComma)
		{
			SetToken(); sc->PPP();
		}
	}
	SetToken(); sc->PPP();
	if (t == typeLeftBrace)
		CompOper();
	SetToken(); sc->PPP();
	if (t != typeSemicolon)
		sc->PrintError("ожидался символ ; (met)", l);
}

void Diagram::CompOper() {
	int braceCount = 0; // Для отслеживания уровня вложенности фигурных скобок
	do {
		if (t == typeLeftBrace) {
			braceCount++; // Увеличиваем уровень вложенности
		}
		else if (t == typeRightBrace) {
			braceCount--; // Уменьшаем уровень вложенности
			if (braceCount < 0) {
				sc->PrintError("Лишняя закрывающая фигурная скобка (CO)", l);
				return;
			}
		}
		if (t == typeLeftBrace) {
			SetToken(); sc->PPP();   // Считываем следующий токен
		}
		else if ((t == typeDouble) || (t == typeChar)) {
			Data();
			SetToken(); sc->PPP();
		}
		if ((t == typeSemicolon) || (t == typeId) || (t == typeDo)) {
			Operator();
		}
		if (t == typeConst) {
			NamedConst();
		}
		if (t == typeClass) {
			Class();
		}
	} while ((braceCount > 0) && (t == typeLeftBrace || t == typeRightBrace || t == typeDouble || t == typeChar || t == typeSemicolon || t == typeId || t == typeDo || t == typeWhile));

	if (braceCount > 0) {
		sc->PrintError("не закрыты фигурные скобки (CO)", l);
	}
}

void Diagram::Operator() {
	if (t == typeSemicolon) {
		SetToken(); sc->PPP();
		return;
	}
	if (t == typeDo) {
		SetToken(); sc->PPP();
		Operator();
		SetToken(); sc->PPP();
		if (t == typeWhile) {
			SetToken(); sc->PPP();
			if (t == typeLeftBracket)
			{
				Expression();
				if (t == typeRightBracket) {
					SetToken(); sc->PPP();
					if (t == typeSemicolon) {
						return;
					}
					else
						sc->PrintError("ожидался символ ; (oper)", l);
				}
				else
					sc->PrintError("ожидался символ ) (oper)", l);
			}
			else
				sc->PrintError("ожидался символ ( (oper)", l);
		}
		else
			sc->PrintError("ожидался while (oper)", l);
	}

	if (t == typeRightBrace) {
		return;
	}
	if (t == typeLeftBrace) {
		CompOper();
		return;
	}

	Assigment();
	if (t != typeSemicolon)
		sc->PrintError("ожидался символ ; (oper)", l);
}


void Diagram::Assigment() {

	Variable();
	if (t == typeEval) {
		Expression();
	}
	else {
		sc->PrintError("ожидался знак = (assig)", l);
	}
}

void Diagram::Variable() {

	if (t == typeAccessOperator)
	{
		SetToken(); sc->PPP();
		Variable();
	}
	if (t == typeId) {
		SetToken(); sc->PPP();
		Variable();
	}
	if (t == typeEval)
	{
		return;
	}
	if (t != typeId) {
		sc->PrintError("ожидался идентификатор (var)", l);
	}
}

void Diagram::Expression() {
	SetToken(); sc->PPP();
	Comparison();
	while (t == typePlus || t == typeMinus) {
		SetToken(); sc->PPP();
		Comparison();
	}
}

void Diagram::Comparison() {
	Shift();
	while ((t == typeLessOrEq) || (t == typeLess) || (t == typeMoreOrEq) || (t == typeMore) || (t == typeUnEq)) {
		SetToken(); sc->PPP();
		Shift();
	}
}

void Diagram::Shift() {
	Term();
	while ((t == typeShiftLeft) || (t == typeShiftRight)) {
		SetToken(); sc->PPP();
		Term();
	}
}

void Diagram::Term() {
	Factor();
	while (t == typePlus || t == typeMinus) {
		SetToken(); sc->PPP();
		Factor();
	}
}

void Diagram::Factor() {
	Elementary();

	while (t == typeMul || t == typeDiv || t == typeMod) {
		SetToken(); sc->PPP();
		Elementary();
	}
}

void Diagram::Elementary() {
	if (t == constInt || t == constHex) {
		SetToken(); sc->PPP();
		return;
	}
	if (t == typeId) {
		Variable();
		return;
	}
	if (t == typeLeftBracket) {
		Expression();
	}
	if (t == typeRightBracket) {
		SetToken(); sc->PPP();
	}
	Unary();
}

void Diagram::Unary() {
	if (t == typePlus || t == typeMinus) {
		SetToken(); sc->PPP();
	}
	Elementary();
}