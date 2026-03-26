#pragma once
#define MAX_TITLE 100
#define MAX_BOOKS 1000
struct Book {
	char title[MAX_TITLE];
	char author[MAX_TITLE];
	int year;
};
bool isEmpty(const char*);
void Menu_Call(char[8][50]);
void Add_Book(Book[], int&);
void Remove_Book(Book[], int&);
void Search_Book(Book[], int&);
void Print_Books(Book[], int&);
void Sort_Books(Book[], int&);
void Library_Save(Book[], int&);
void Library_Load(Book[], int&);