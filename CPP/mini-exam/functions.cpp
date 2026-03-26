#include <iostream>
#include "Header.h"
#include <climits>
#include <string>
#include <fstream>
#include <cstdio>
using namespace std;
bool isEmpty(const char* str) /*для безпеки, що не передане значення не буде змінено*/ {
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] != ' ') return false;
	}
	return true;
}
void Menu_Call(char menu[8][50]) {
	cout << "Бібліотека: " << endl;
	for (int i = 0;i < 8;i++) {
		cout << menu[i] << endl;
	}
	cout << endl;
}
void Add_Book(Book library[], int& bookCount) {
	if (bookCount == MAX_BOOKS) {
		cout << "Бібліотека повна!" << endl;
		cout << endl;
		return;
	}
	cout << "Додавання книги." << endl;

	char title[SHRT_MAX];
	cin.ignore();
	do {
		cout << "Назва книги: " << endl;
		cin.getline(title, SHRT_MAX);
		if (strlen(title) >= MAX_TITLE) {
			cout << "Назва повинна бути менше 100 символів!" << endl;
		}
	} while (strlen(title) >= MAX_TITLE || isEmpty(title));

	char author[SHRT_MAX];
	do {
		cout << "Автор книги: " << endl;
		cin.getline(author, SHRT_MAX);
		if (strlen(author) >= MAX_TITLE) {
			cout << "Ім'я автора повинно бути менше 100 символів!" << endl;
		}
	} while (strlen(author) >= MAX_TITLE || isEmpty(author));
	int year;
	do {
		cout << "Рік видання: " << endl;
		cin >> year;
		if (year > 2026 || year < 0) {
			cout << "Введіть правильну дату!" << endl;
		}
	} while (year > 2026 || year < 0);
	Book added_book;
	strcpy(added_book.title, title);
	strcpy(added_book.author, author);
	added_book.year = year;
	library[bookCount] = added_book;
	bookCount++;
	cout << "Книгу додано." << endl;
	cout << endl;
}
void Remove_Book(Book library[], int& bookCount) {
	cout << "Видалення книги." << endl;
	if (bookCount == 0) {
		cout << "Нема книг!" << endl;
		return;
	}
	int choice = 0;
	while (choice != 1 && choice != 2) {
		cout << "За індексом - 1" << endl << "За назвою - 2" << endl;
		cout << "Вибір: ";
		cin >> choice;
		if (choice != 1 && choice != 2) {
			cout << "Введіть коректну відповідь!" << endl;
		}
	}
	int index;
	if (choice == 1) {
		do {
			cout << "Введіть індекс: ";
			cin >> index;
			if (index < 0 || index >= bookCount) {
				cout << "Введіть коректний індекс!" << endl;
			}
		} while (index < 0 || index >= bookCount);

		for (int i = index; i < bookCount - 1; i++) {
			library[i] = library[i + 1];
		}
		bookCount--;
		cout << "Книгу видалено." << endl;
	}
	else if (choice == 2) {
		char title[MAX_TITLE];
		cout << "Введіть назву: ";
		cin.ignore();
		cin.getline(title, MAX_TITLE);

		int foundIndices[MAX_BOOKS];
		int foundCount = 0;
		for (int i = 0; i < bookCount; i++) {
			if (strcmp(library[i].title, title) == 0) {
				foundIndices[foundCount++] = i;
			}
		}
		if (foundCount == 0) {
			cout << "Книгу не знайдено." << endl;
			return;
		}
		cout << "Знайдено співпадіння:" << endl;
		for (int i = 0; i < foundCount; i++) {
			int index = foundIndices[i];
			cout << i << ". " << library[index].title << " (" << library[index].year << ") " << library[index].author << " (Індекс у бібліотеці: " << index << ")" << endl;
		}

		int removeChoice;
		do {
			cout << "Введіть номер книги для видалення: ";
			cin >> removeChoice;
			if (removeChoice < 0 || removeChoice >= foundCount) {
				cout << "Введіть коректний номер!" << endl;
			}
		} while (removeChoice < 0 || removeChoice >= foundCount);

		index = foundIndices[removeChoice];
		for (int i = index; i < bookCount - 1; i++) {
			library[i] = library[i + 1];
		}
		bookCount--;
		cout << "Книгу видалено." << endl;
	}
}
void Search_Book(Book library[], int& bookCount) {
	cout << "Пошук книги." << endl;
	if (bookCount == 0) {
		cout << "Нема книг!" << endl;
		return;
	}
	int choice =0;
	while (choice != 1 && choice != 2) {
		cout << "За роком - 1" << endl << "За назвою - 2" << endl;
		cout << "Вибір: " << endl;
		cin >> choice;
		if (choice != 1 && choice != 2) {
			cout << "Введіть коректну відповідь!" << endl;
		}
	}
	if (choice == 1) {
		int year;
		do {
			cout << "Введіть рік: " << endl;
			cin >> year;
			if (year > 2026 || year < 0) {
				cout << "Введіть правильну дату!" << endl;
			}
		} while (year > 2026 || year < 0);
		for (int i = 0;i < bookCount;i++) {
			if (library[i].year == year) {
				cout <<library[i].title << " (" << library[i].year << ") " << library[i].author << " (Індекс у бібліотеці: " << i << ")" << endl;
			}
		}
	}
	else if (choice == 2) {
		char title[MAX_TITLE];
		do {
			cout << "Введіть назву: " << endl;
			cin.ignore();
			cin.getline(title, MAX_TITLE);
		} while (isEmpty(title));
		cout << "Знайдено: " << endl;
		for (int i = 0;i < bookCount;i++) {
			if (strcmp(library[i].title, title) == 0) {
				cout << library[i].title << "("<<library[i].year<<")"<<endl;
			}
		}
	}
	cout << "Пошук завершено." << endl;
}
void Print_Books(Book library[], int& bookCount) {
	cout << "Всі книги: " << endl;
	if (bookCount == 0) {
		cout << "Нема книг!" << endl;
		return;
	}
	for (int i = 0;i < bookCount;i++) {
		cout << i <<". "<< library[i].title << "(" << library[i].year << ")" << endl;
	}
	cout << endl;
}
void Sort_Books(Book library[], int& bookCount) {
	int choice = 0;
	cout << "Сортування." << endl;
	if (bookCount == 0) {
		cout << "Нема книг!" << endl;
		return;
	}
	while (choice != 1 && choice != 2) {
		cout << "За роком - 1" << endl << "За назвою - 2" << endl;
		cout << "Вибір: " << endl;
		cin >> choice;
		if (choice != 1 && choice != 2) {
			cout << "Введіть коректну відповідь!" << endl;
		}
	}
	if (choice == 1) {
		bool swapped = true;
		int start = 0;
		int end = bookCount - 1;

		while (swapped) {
			swapped = false;
			for (int i = start; i < end; ++i) {
				if (library[i].year > library[i + 1].year) {
					swap(library[i], library[i + 1]);
					swapped = true;
				}
			}
			if (!swapped)
				break;
			swapped = false;
			--end;
			for (int i = end - 1; i >= start; --i) {
				if (library[i].year > library[i + 1].year) {
					swap(library[i], library[i + 1]);
					swapped = true;
				}
			}
			++start;
		}
	}
	else if (choice == 2) {
		bool swapped = true;
		int start = 0;
		int end = bookCount - 1;

		while (swapped) {
			swapped = false;
			for (int i = start; i < end; ++i) {
				if (strcmp(library[i].title, library[i + 1].title) > 0) {
					swap(library[i], library[i + 1]);
					swapped = true;
				}
			}
			if (!swapped)
				break;
			swapped = false;
			--end;
			for (int i = end - 1; i >= start; --i) {
				if (strcmp(library[i].title, library[i + 1].title) > 0) {
					swap(library[i], library[i + 1]);
					swapped = true;
				}
			}
			++start;
		}
	}
	cout << "Сортування завершено." << endl;
	cout << endl;
}
void Library_Save(Book library[], int& bookCount) {
	char filename[MAX_TITLE+4];
	cout << "Назва файлу для збереження: " << endl;
	cin >> filename;
	strcat(filename, ".dat");
	FILE* f = fopen(filename, "wb");
	if (!f) {
		cout << "Не вдалося відкрити файл!" << endl;
		return;
	}
	fwrite(&bookCount, sizeof(int), 1, f);
	fwrite(library, sizeof(Book), bookCount, f);
	fclose(f);
}
void Library_Load(Book library[], int& bookCount) {
	char filename[MAX_TITLE+4];
	cout << "Назва файлу для відкриття: " << endl;
	cin >> filename;
	strcat(filename, ".dat");
	FILE* f = fopen(filename, "rb");
	if (!f) {
		cout << "Не вдалося відкрити файл!" << endl;
		return;
	}
	fread(&bookCount, sizeof(int), 1, f);
	fread(library, sizeof(Book), bookCount, f);
	fclose(f);
}