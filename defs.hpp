#ifndef __DEFS
#define __DEFS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT 500 // максимальная длина текста
#define MAX_LEX 50 // максимальная длина лексемы
#define MAX_KEYW 13 // число ключевых слов

typedef char TypeLex[MAX_LEX];
typedef char TypeMod[MAX_TEXT];

// Определение типов лексем
// <----Ключевые слова---->
#define typeId 1
#define typeDouble 10
#define typeChar 11
#define typeMain 12
#define typeDo 13
#define typeWhile 14
#define typeClass 15
#define typeConst 16 //---!
#define typeVoid 17
#define typeInt 18
#define typeBool 19
// <----Константы---->
#define constInt 20
#define constHex 21
#define constDouble 22
// <----T/F---->
#define typeTrue 23
#define typeFalse 24
// <----Спец. знаки---->
#define typeComma 30
#define typeSemicolon 31
#define typeLeftBracket 32	// (
#define typeRightBracket 33 // )
#define typeLeftBrace 34	// {
#define typeRightBrace 35	// }
#define typeAccessOperator 36
// <----Знаки сравнения---->
#define typeNotEq 40
#define typeEq 41
#define typeLessOrEq 42
#define typeLess 43
#define typeMoreOrEq 44
#define typeMore 45
// <----Знаки действий---->
#define typeEval 50
#define typePlus 51
#define typeMinus 52
#define typeMod 53
#define typeDiv 54
#define typeMul 55
#define typeShiftLeft 56
#define typeShiftRight 57
// <----Дополнительные---->
#define typeEnd 100
#define typeError 200
#endif
