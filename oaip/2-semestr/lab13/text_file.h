#pragma once
#include <fstream>
#include "console.h"
void readtxt(book*& array, int& N);
void writetxt(book* array, int N);
void index_file(int N, int* index);
void writebin(book* array, int N);
void readbin(book*& array, int& N);