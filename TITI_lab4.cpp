#include <iostream>
#include <stdio.h>
#include <string.h>
#include "defs.hpp"
#include "Diagram.hpp"
int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");
	TScanner* sc;
	if (argc <= 1) sc = new TScanner("input.txt");
	else sc = new TScanner(argv[1]); 
	TDiagram* dg = new TDiagram(sc);
	try {
		dg->Program();
		int type; TypeLex l;
		type = sc->Scanner(l);
		if (type == typeEnd) printf("Синтаксических ошибок не обнаружено. \n");
		else sc->PrintError("Лишний текст в конце программы.", "");
		delete sc;
		delete dg;
	}
	catch (const char* err) {
		sc->PrintError();
		printf("%s", err);
		delete sc;
		delete dg;
	}
	return 0;
}