#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
enum Proizv { Belarus = 1, Russia, Lituania, Ukraine };
union Nalichie {
	int check;
	char access;
};
struct book {
	int reg;
	char author[80],
		name[80],
		izdat[80];
	int year;
	unsigned pages;
	Nalichie nal;
	Proizv proizv;
};

extern int key;

void input(book*& array, int& N);
void output(book* array, int N);