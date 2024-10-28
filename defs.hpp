#pragma once
#ifndef DEFS_HPP         
#define DEFS_HPP          

#include <cstdio>         
#include <cstdlib>       
#include <cstring>       

#define MAX_TEXT 10000    // максимальная длина текста
#define MAX_LEX 100    // максимальная длина лексемы

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
// <----Константы---->
#define constInt 21
#define constHex 22
// <----Спец. знаки---->
#define typeComma 30
#define typeSemicolon 31
#define typeLeftBracket 32
#define typeRightBracket 33
#define typeLeftBrace 34
#define typeRightBrace 35
#define typeLeftSqBracket 36
#define typeRightSqBracket 36
// <----Знаки операций---->
#define typeEval 40
#define typeUnEq 41
#define typeEq 42
#define typeLessOrEq 43
#define typeLess 44
#define typeMoreOrEq 45
#define typeMore 46
#define typeShiftRight 47
#define typeShiftLeft 48
#define typePlus 49
#define typeMinus 50
#define typeMod 51
#define typeDiv 52
#define typeMul 53
// <----Дополнительные---->
#define typeEnd 100
#define typeError 200
#endif 
