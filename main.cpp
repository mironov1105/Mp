#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "pars.h"
#include "shifr.h"

using namespace std;

int main(int argc, char* argv[])
{

	setlocale(0, "");

	//Имя файла конфигурации по умолчанию:
	string conf = "D:\\pass.txt";
	string conf1;
	char* text = "";
	int a, b, c, mast;
	bool q = true;
	string word, master;

	//проверка аргументов командной строки на наличие опций:
	for (int i = 0; i < argc; i++) {

		if (strcmp(argv[i], "--help") == 0)
			help();

		if (strcmp(argv[i], "--config") == 0) {
			if (argc > 2){
				string d = string(argv[i + 1]);
				a = d.find('\\');
				if (a > -1)
					conf = d;
			}
		}
	}

	//открывыем файл конфигурации:
	ifstream file(conf);
	file >> conf1;
	file.close();

	//открываем базу паролей и берем хэш мастер-пароля:
	//Хэш мастер-пароля хранится в базе паролей и находится на первой строчке файла.
	ifstream f(conf1);
	f >> mast;
	f.close();

	cout << "Введите мастер пароль: ";
	cin >> master;

	int s = hesh(master);
	if (s == mast)
		for (int i = 0; i < argc; i++) {//Поиск в аргументах командной строки команд программы. Если такие находятся, программы выполняет команду и завершается.

			//strcmp - функция сравнения аргумента командной строки и заданной строки.
			if (strcmp(argv[i], "help") == 0) {
				helpcom();
				return 0;
			}

			if (strcmp(argv[i], "master") == 0) {
				string master_new = string(argv[i + 1]);
				master_pass(master_new, conf1, master);
				return 0;
			}

			if (strcmp(argv[i], "get") == 0) {
				string arg = string(argv[i + 1]);
				for (int i = 0; i < arg.size(); i++)
					text[i] = arg[i];
				get(text, conf1, master);
				return 0;
			}

			if (strcmp(argv[i], "add") == 0) {
				string arg = string(argv[i + 1]);
				add(arg, conf1, master);
				return 0;
			}

			if (strcmp(argv[i], "del") == 0) {
				string arg = string(argv[i + 1]);
				del(arg, conf1, master);
				return 0;
			}

			if (strcmp(argv[i], "chk") == 0) {
				string arg = string(argv[i + 1]);
				chk(arg, conf1, master);
				return 0;
			}

			if ((strcmp(argv[i], "exit") == 0) || (strcmp(argv[i], "quit") == 0) || (strcmp(argv[i], "q") == 0))
				return 0;
		}

	//основной цикл:
	if (s == mast) {

		while (q) {

			getline(cin, word);

			//проверка условий для запуска нужной функции:

			a = word.find("help");
			if (a > -1) {
				helpcom();
				continue;
			}

			a = word.find("master");
			if (a > -1) {
				string master_new = word.erase(0, 7);
				master_pass(master_new, conf1, master);
				master = master_new;
				continue;
			}

			a = word.find("get");
			if (a > -1) {
				word.erase(0, 4);
				char* text1 = new char[word.length() + 1];
				strcpy(text1, word.c_str());
				text = text1;
				get(text, conf1, master);
				continue;
			}

			a = word.find("add");
			if (a > -1) {
				word.erase(0, 4);
				add(word, conf1, master);
				continue;
			}

			a = word.find("del");
			if (a > -1) {
				word.erase(0, 4);
				del(word, conf1, master);
				continue;
			}

			a = word.find("chk");
			if (a > -1) {
				word.erase(0, 4);
				chk(word, conf1, master);
				continue;
			}

			a = word.find("exit");
			b = word.find("quit");
			c = word.find("q");
			if ((a > -1) || (b > -1) || (c > -1))
				q = false;
		}
	}
}
