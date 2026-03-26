#include <iostream>
#include <iomanip>
#include <filesystem>
#include <string>
#include "Header.h"
using namespace std;

int main() {

	char menu[8][50]{
		"1. Додати книгу",
		"2. Видалити книгу",
		"3. Знайти книгу",
		"4. Показати всі книги",
		"5. Сортувати книги",
		"6. Зберегти у файл",
		"7. Завантажити з файлу",
		"0. Вихід"
	};
	Book library[MAX_BOOKS];
	int bookCount = 0;
	int choice;
	while (true) {
		Menu_Call(menu);
		cout << "Вибір: " << endl;
		cin >> choice;
		switch (choice) {
		case 1:
			Add_Book(library, bookCount);
			break;
		case 2:
			Remove_Book(library, bookCount);
			break;
		case 3:
			Search_Book(library, bookCount);
			break;
		case 4:
			Print_Books(library, bookCount);
			break;
		case 5:
			Sort_Books(library, bookCount);
			break;
		case 6:
			Library_Save(library, bookCount);
			break;
		case 7:
			Library_Load(library, bookCount);
			break;
		case 0:
			int choice = 0;
			while (choice != 1 && choice != 2) {
				cout << "Не хочете зберегти файл?" << endl;
				cout << "Так - 1" << endl << "Ні - 2" << endl;
				cout << "Вибір: " << endl;
				cin >> choice;
				if (choice != 1 && choice != 2) {
					cout << "Введіть коректну відповідь!" << endl;
				}
			}
			if (choice == 1) {
				Library_Save(library, bookCount);
			}
			return 0;
		}
	}
};