#define _CRT_SECURE_NO_WARNINGS
#include "Scanner.hpp"
#include <string.h>
#include "defs.hpp"

/* Конструктор */
TScanner::TScanner(const char* FileName) {
	GetData(FileName);
	GetPtr(0);
	currentLine = 1;
	ptrLine = ptr;
}

/* Ключевые слова */
TypeLex Keyword[MAX_KEYW] = { "int",   "void",   "double",   "main",   "true",   "false",   "bool",   "char",   "do",   "while",   "class", "const"};
int IndexKeyword[MAX_KEYW] = { typeInt, typeVoid, typeDouble, typeMain, typeTrue, typeFalse, typeBool, typeChar, typeDo, typeWhile, typeClass, typeConst };

/* Восстановление и запоминание указателей */
void TScanner::GetPtr(int i) { 
	ptr = i; 
}

int TScanner::SetPtr(void) {
	return ptr; 
}

/* Вывод ошибок */
void TScanner::PrintError(const char* err, const char* lex) {
	if (lex[0] != '\0')
		printf("Строка %d позиция %d: %s - получен: %s\n", currentLine, ptr - ptrLine, err, lex);
	else
		printf("Строка %d позиция %d: %s\n", currentLine, ptr - ptrLine, err);
	exit(0);
}


void TScanner::PrintError(const char* err) {
	printf("строка %d позиция %d: %s\n", currentLine, ptr - ptrLine, err);
}

void TScanner::PrintError() {
	printf("строка %d позиция %d\n", currentLine, ptr - ptrLine);
}

/* Основная функция сканирования */
int TScanner::Scanner(TypeLex l) {
	int i = 0; // текущая длина лексемы
	for (i = 0; i < MAX_LEX; i++) l[i] = 0; // очистка лексемы
	i = 0;

start:
	// Пропустим пробелы, табуляции и переходы
	while ((t[ptr] == ' ') || (t[ptr] == '\n') || (t[ptr] == '\t')) {
		if (t[ptr] == '\n' && ptrLine != ptr + 1) {
			ptrLine = ptr + 1;
			currentLine++;
		}
		ptr++;
	}

	// Однострочный комментарий
	if ((t[ptr] == '/') && (t[ptr + 1] == '/')) {
		ptr = ptr + 2;
		while ((t[ptr] != '\n') && (t[ptr] != '\0')) ptr++;
		goto start;
	}
	// Многострочный комментарий
	else if ((t[ptr] == '/') && (t[ptr + 1] == '*')) {
		ptr = ptr + 2;
		while (!((t[ptr] == '*') && (t[ptr + 1] == '/')) && (t[ptr] != '\0')) {
			if (t[ptr] == '\n' && ptrLine != ptr) {
				ptrLine = ptr;
				currentLine++;
			}
			ptr++;
		}
		if ((t[ptr] != '*') && (t[ptr + 1] != '/')) {
			PrintError("Незакрытый многострочный комментарий", "");
			return typeError;
		}
		ptr = ptr + 2;
		goto start;
	}
	else if (t[ptr] == '\0') {
		l[0] = '#';
		return typeEnd;
	}

	// Числа: поддержка 0x
	else if ((t[ptr] >= '0' && t[ptr] <= '9')) {
		bool hexDetect = false;
		while ((t[ptr] >= '0' && t[ptr] <= '9') || (t[ptr] >= 'a' && t[ptr] <= 'f') || t[ptr] == 'x' || t[ptr] == '.') {
			if (t[ptr] == '0') {
				if (t[ptr + 1] == 'x'){
					hexDetect = true;
					l[i++] = t[ptr++];
			}
			}
			else if (t[ptr] == 'x' && hexDetect == true)
				PrintError("Неверный формат hex", "");
			l[i++] = t[ptr++];
		}
		return constInt;
	}

	// Идентификаторы или ключевые слова
	else if ((t[ptr] >= 'a' && t[ptr] <= 'z') || (t[ptr] >= 'A' && t[ptr] <= 'Z') || t[ptr] == '_') {
		l[i++] = t[ptr++];
		while ((t[ptr] >= 'a' && t[ptr] <= 'z') || (t[ptr] >= 'A' && t[ptr] <= 'Z') || (t[ptr] >= '0' && t[ptr] <= '9') || t[ptr] == '_') {
			if (i < MAX_LEX - 1)
				l[i++] = t[ptr++];
			else
				PrintError("Привышена длинна ликсемы", "");
		}
		for (int j = 0; j < MAX_KEYW; j++) {
			if (strcmp(l, Keyword[j]) == 0) {
				return IndexKeyword[j];
			}
		}
		return typeId;
	}

	//специальные знаки
	else if (t[ptr] == '.')
	{
		l[i++] = t[ptr++]; return typeAccessOperator;
	}
	else if (t[ptr] == ',')
	{
		l[i++] = t[ptr++]; return typeComma;
	}
	else if (t[ptr] == ';')
	{
		l[i++] = t[ptr++]; return typeSemicolon;
	}
	else if (t[ptr] == '(')
	{
		l[i++] = t[ptr++]; return typeLeftBracket;
	}
	else if (t[ptr] == ')')
	{
		l[i++] = t[ptr++]; return typeRightBracket;
	}
	else if (t[ptr] == '{')
	{
		l[i++] = t[ptr++]; return typeLeftBrace;
	}
	else if (t[ptr] == '}')
	{
		l[i++] = t[ptr++]; return typeRightBrace;
	}
	//знаки операций
	else if (t[ptr] == '*')
	{
		l[i++] = t[ptr++]; return typeMul;
	}
	else if (t[ptr] == '/')
	{
		l[i++] = t[ptr++]; return typeDiv;
	}
	else if (t[ptr] == '%')
	{
		l[i++] = t[ptr++]; return typeMod;
	}
	else if (t[ptr] == '+')
	{
		l[i++] = t[ptr++]; return typePlus;
	}
	else if (t[ptr] == '-')
	{
		l[i++] = t[ptr++]; return typeMinus;
	}
	else if (t[ptr] == '<')
	{
		l[i++] = t[ptr++];
		if (t[ptr] == '=')
		{
			l[i++] = t[ptr++];
			return typeLessOrEq;
		}
		if (t[ptr] == '<')
		{
			l[i++] = t[ptr++];
			return typeShiftLeft;
		}
		return typeLess;
	}
	else if (t[ptr] == '>')
	{
		l[i++] = t[ptr++];
		if (t[ptr] == '=') {
			l[i++] = t[ptr++];
			return typeMoreOrEq;
		}
		if (t[ptr] == '>')
		{
			l[i++] = t[ptr++];
			return typeShiftRight;
		}
		return typeMore;
	}
	else if (t[ptr] == '=')
	{
		l[i++] = t[ptr++];
		if (t[ptr] == '=') {
			l[i++] = t[ptr++];
			return typeEq;
		}
		else {
			return typeEval;
		}
	}
	else if (t[ptr] == '!')
	{
		l[i++] = t[ptr++];
		if (t[ptr] == '=') {
			l[i++] = t[ptr++];
			return typeNotEq;
		}
		else {
			PrintError("Неверный символ", l);
			return typeError;
		}
	}
	else {
		l[i] = t[ptr];
		PrintError("Неверный символ", l); // ошибка
		return typeError;
	}
	//десятичная константа
	while ((t[ptr] <= '9') && (t[ptr] >= '0'))
		if (i < MAX_LEX - 1) l[i++] = t[ptr++];
		else ptr++;
	return constInt;
}
/* Чтение данных */
void TScanner::GetData(const char* FileName) {
	char aa;
	FILE* in = fopen(FileName, "r");
	if (in == NULL) {
		PrintError("Отсутствует входной файл", "");
		exit(1);
	}
	int i = 0;
	while (!feof(in)) {
		fscanf(in, "%c", &aa);
		if (!feof(in)) t[i++] = aa;
		if (i >= MAX_TEXT - 1) {
			PrintError("Слишком большой размер исходного модуля", "");
			break;
		}
	}
	t[i] = '\0'; // знак конца текста
	fclose(in);
}

