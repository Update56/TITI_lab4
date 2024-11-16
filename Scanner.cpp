#define _CRT_SECURE_NO_WARNINGS
#include "defs.hpp"
#include "Scanner.hpp"

Scanner::Scanner(FILE* in)
{
    GetData(in);
    putPointer(0);
    pos = 0;
    line = 1;
}

int Scanner::PPP()
{
    pos = pos + 1;
    return pos;
}

void Scanner::PrintError(string errorMessage, string lexeme)
{
    if (lexeme[0] == 0)
        cout << "Ошибка: " << errorMessage << endl;
    else
        cout << "Строка " << line << ", Позиция " << pos << " Ошибка: " << errorMessage << ". Неверный символ: " << lexeme << endl;
    exit(0);
}

void Scanner::GetData(FILE* in)
{
    if (in == NULL)
    {
        PrintError("Отсутствует входной файл", "");
        exit(1);
    }
    int i = 0;
    char tmp;
    while (!feof(in))
    {
        fscanf(in, "%c", &tmp);
        if (!feof(in))
            t[i++] = tmp;
        if (i > MAX_TEXT)
        {
            PrintError("Слишком большой размер исходного модуля", "");
            break;
        }
    }
    t[i] = '\0';
    fclose(in);
}
void Scanner::GetPtr(int i) {
    ptr = i;//восстановить указатель
}

int Scanner::SetPtr(void) {
    return ptr;// запомнить указатель
}

int Scanner::Scanning(TypeLex l) {
    int curr_len = 0; // текущая длина лексемы
    char curr_char; // текущий символ

    while ((curr_char = t[ptr]) == ' ' || curr_char == '\n' || curr_char == '\t') {
        if (curr_char == '\n')
            line++;
        ptr++; // пропуск незначащих элементов
    }
    if (curr_char == '\0') {
        l[0] = '\0';
        return typeEnd;
    }

    // Обработка комментариев
    if (curr_char == '/' && t[ptr + 1] == '/') {
        // Пропускаем всю строку
        while (curr_char != '\n' && curr_char != '\0') {
            curr_char = t[++ptr];
        }
        return Scanning(l);
    }

    // Идентификаторы и ключевые слова
    if ((curr_char >= 'a' && curr_char <= 'z') || (curr_char >= 'A' && curr_char <= 'Z') || curr_char == '_') {
        do {
            if (curr_len < MAX_LEX - 1) {  // Измерение для предотвращения переполнения
                l[curr_len++] = curr_char;
            }
            else
            {
                printf("Error: line exceeds maximum allowed length of characters.\n");
                exit(EXIT_FAILURE);
            }
            curr_char = t[++ptr];

        } while ((curr_char >= 'a' && curr_char <= 'z') || (curr_char >= 'A' && curr_char <= 'Z')
            || (curr_char >= '0' && curr_char <= '9') || curr_char == '_');
        l[curr_len] = '\0';

        // Проверка на ключевые слова
        if (strcmp(l, "double") == 0) return typeDouble;
        else if (strcmp(l, "char") == 0) return typeChar;
        else if (strcmp(l, "main") == 0) return typeMain;
        else if (strcmp(l, "do") == 0) return typeDo;
        else if (strcmp(l, "while") == 0) return typeWhile;
        else if (strcmp(l, "class") == 0) return typeClass;
        else if (strcmp(l, "const") == 0) return typeConst;
        else if (strcmp(l, "void") == 0) return typeVoid;
        else return typeId;
    }
    //обратбока целых
    if (curr_char >= '1' && curr_char <= '9') {
        do {
            if (curr_len < MAX_LEX - 1) {  // Измерение для предотвращения переполнения
                l[curr_len++] = curr_char;
            }
            else {
                printf("Error: number exceeds maximum allowed length of characters.\n");
                exit(EXIT_FAILURE);
            }
            curr_char = t[++ptr];
        } while ((curr_char >= '0') && (curr_char <= '9'));
        l[curr_len] = '\0';
        return constInt;
    }
    // Обработка целых и 16 с/с чисел(с нуля)
    if (curr_char == '0') {
        if (curr_len < MAX_LEX - 1) {  // Измерение для предотвращения переполнения
            l[curr_len++] = curr_char;
        }
        else {
            printf("Error: number exceeds maximum allowed length of characters.\n");
            exit(EXIT_FAILURE);
        }
        curr_char = t[++ptr];
        //обработка 16 с/с (с 'x')
        if (curr_char == 'x'){
            if (curr_len < MAX_LEX - 1) {  // Измерение для предотвращения переполнения
                l[curr_len++] = curr_char;
            }
            else {
                printf("Error: number exceeds maximum allowed length of characters.\n");
                exit(EXIT_FAILURE);
            }
            curr_char = t[++ptr];
            if((curr_char >= '0' && curr_char <= '9') || (curr_char >= 'a' && curr_char <= 'f')){
                do {
                    if (curr_len < MAX_LEX - 1) {  // Измерение для предотвращения переполнения
                        l[curr_len++] = curr_char;
                    }
                    else {
                        printf("Error: number exceeds maximum allowed length of characters.\n");
                        exit(EXIT_FAILURE);
                    }
                    curr_char = t[++ptr];
                } while ((curr_char >= '0' && curr_char <= '9') || (curr_char >= 'a' && curr_char <= 'f'));
                l[curr_len] = '\0';
                return constHex;
            }
            else
            {
                PrintError("AAA", "curr_char");
                return typeError;
                exit(0);
            }
        }
        //обработка 
        else if (curr_char >= '0' && curr_char <= '9')
        {
            do{
                if (curr_len < MAX_LEX - 1) {  // Измерение для предотвращения переполнения
                    l[curr_len++] = curr_char;
                }
                else {
                    printf("Error: number exceeds maximum allowed length of characters.\n");
                    exit(EXIT_FAILURE);
                }
                curr_char = t[++ptr];
            } while (curr_char >= '0' && curr_char <= '9');
            l[curr_len] = '\0';
            return constInt;
        }
        else
        {
            curr_char = t[++ptr];
            l[curr_len] = '\0';
            return constInt;
        }
    }
    // Операторы и специальные символы
    switch (curr_char) {
    case ',':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeComma;
    case ';':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeSemicolon;
    case '(':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeLeftBracket;
    case ')':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeRightBracket;
    case '{':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeLeftBrace;
    case '}':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeRightBrace;
    case '[':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeLeftSqBracket;
    case ']':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeRightSqBracket;
    case '.':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeAccessOperator;
    case '=':
        if (t[ptr + 1] == '=') {
            l[curr_len++] = curr_char; curr_char = t[++ptr];
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeEq;
        }
        else {
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeEval;
        }
    case '>':
        if (t[ptr + 1] == '=') {
            l[curr_len++] = curr_char; curr_char = t[++ptr];
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeMoreOrEq;
        }
        else if (t[ptr + 1] == '>') {
            l[curr_len++] = curr_char; curr_char = t[++ptr];
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeShiftRight;
        }
        else {
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeMore;
        }
    case '<':
        if (t[ptr + 1] == '=') {
            l[curr_len++] = curr_char; curr_char = t[++ptr];
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeLessOrEq;
        }
        else if (t[ptr + 1] == '<') {
            l[curr_len++] = curr_char; curr_char = t[++ptr];
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeShiftLeft;
        }
        else {
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeLess;
        }
    case '!':
        if (t[ptr + 1] == '=') {
            l[curr_len++] = curr_char; curr_char = t[++ptr];
            l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
            return typeUnEq;
        }
        else {
            PrintError("AAA", "curr_char");
            return typeError;
        }
    case '+':
        l[curr_len++] = curr_char;
        l[curr_len] = '\0';
        ptr++;
        return typePlus;
    case '-':
        l[curr_len++] = curr_char;
        l[curr_len] = '\0';
        ptr++;
        return typeMinus;
    case '*':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeMul;
    case '/':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeDiv;
    case '%':
        l[curr_len++] = curr_char; l[curr_len] = '\0'; ptr++;
        return typeMod;
    default:
        PrintError("AAA", "curr_char");
        return typeError;
        exit(0);
    }
}

