#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include "defs.hpp"
#include "Scanner.hpp"
#include "Diagram.hpp"
#include "iostream"

int main(int argc, char* argv[]) {

    class game {
        void help();
        double wheel = 10;
    };

    setlocale(LC_ALL, "Rus");
    int type;
    TypeLex l;

    FILE* file = fopen("input.txt", "r");
    if (file == nullptr) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return 1;
    }

    Scanner* scanner = new Scanner(file);
    Diagram* diagram = new Diagram(scanner);

    diagram->Program();

    type = scanner->Scanning(l);

    if (type == typeEnd)
        printf("Синтаксических ошибок не обнаружено.\n");
    else
        scanner->PrintError("Лишний текст в конце программы.", "");

    delete scanner;
    delete diagram;
    return 0;
}