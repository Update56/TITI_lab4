#include <cstdio>
#include "defs.hpp"
#include "Scanner.hpp"

int main(void) {
    int type;
    TypeLex l; // локальные переменные типа и изображения лексемы

    TScanner scanner;  // Создаем экземпляр класса TScanner
    scanner.GetData(); // ввести данные

    do {
        type = scanner.Scanner(l); // Используем метод Scanner
        printf("Type: %d, lexeme: %s\n", type, l);
    } while (type != typeEnd);

    return 0;
}