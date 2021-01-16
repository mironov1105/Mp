#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <Windows.h>
#include "shifr.h"

using namespace std;

//Функция хеширования. Суммирует индексы символов:
int hesh(string word) {
	int a = 0;
	for (int i = 0; i < word.length(); i++) {
		a += word[i];
	}
	return a;
}

//Функция опции "--help":
void help() {
	cout << "1) --help\nВывод доступных опций\n"
		<< "2) --config PATH\nЗапускает программу с конфогом PATH\n"
		<< "Если опция не указана, берет конфиг по стандартному фиксированному пути\n\n";
}

//Функция команды "help":
void helpcom() {
	cout << "1) master\nУстановка нового мастер-пароля\n"
		"2) get SITE:LOGIN\nКопирует в буфер обмена пароль для сайта SITE, логина LOGIN\n"
		"3) add SITE:LOGIN\n"
		"Добавляет в базу пароль для сайта SITE, логина LOGIN\n"
		"4) del SITE:LOGIN\n"
		"удаляет из базы пароль для сайта SITE, логина LOGIN\n"
		"5) chk SITE:LOGIN\n"
		"Проверяет наличие пароля для сайта SITE, логина LOGIN\n"
		"6) help\n"
		"Выводит доступные комманды и их описание\n"
		"7) q / quit / exit\n"
		"Завершает работу программы\n\n";
}

//Функция команды "master":
void master_pass(string master, string conf, string mast) {//master - новый мастер-пароль; conf - путь к базе паролей; mast - старый мастер пароль.
	int i = 0;
	string buff;
	ifstream f(conf);
	//Создаю массивы под логины и пароли
	string* login = new string[10000];
	string* pass = new string[10000];
	int massa;
	f >> massa;//исключаю хэш мастер-пароля из работы функции.
	//Пока переменные считываются, они заполняют собой соответствующие массивы:
	while (!f.eof()) {
		f >> buff;
		login[i] = reshifr(buff, mast);
		f >> buff;
		pass[i] = reshifr(buff, mast);
		i++;
	}
	f.close();
	ofstream ff(conf);
	//Меняем хэш старого мастер-пароля на новый:
	int m;
	m = hesh(master);
	//проверка, нужно ли в файле переходить на новую строку:
	if (i != 0)
		ff << m << '\n';
	else
		ff << m;
	//Запись перешифрованных логинов и паролей обратно в файл:
	for (int j = 0; j < i - 1; j++)
		ff << shifr(login[j], master) << ' ' << shifr(pass[j], master) << '\n';
	if (i >= 0)
		ff << shifr(login[i - 1], master) << ' ' << shifr(pass[i - 1], master);//Делается, чтобы избежать перехода на следующую строку при записи последних логина и пароля в файл.
	ff.close();
}
void get(char* text, string conf, string mast) {//text - введенные site:login; conf - путь до базы паролей; mast - мастер-пароль.
	string buff;
	ifstream f(conf);
	string login;
	string pass;
	//Исключаю хэ мастер-пароля из работы функции:
	int master;
	f >> master;

	while (!f.eof()) {
		f >> login;
		f >> pass;
		login = reshifr(login, mast);
		pass = reshifr(pass, mast);
		if (text == login) {//если такой сайт:логин найден, переносим пароль для них в массив типа char(нужно для записи в буфер обмена).
			text = "";
			char* text1 = new char[pass.length() + 1];
			strcpy(text1, pass.c_str());
			text = text1;
			break;
		}
	}
	
	f.close();

	//Добавление пароля в буфер обмена:
	if (OpenClipboard(0)) {
		EmptyClipboard();
		HGLOBAL hbuff = GlobalAlloc(GMEM_FIXED, strlen(text) + 1);
		char* hbuf = (char*)GlobalLock(hbuff);
		strcpy(hbuf, text);
		GlobalUnlock(hbuff);
		SetClipboardData(CF_TEXT, hbuf);
		CloseClipboard();
	}
}

void add(string text, string conf, string mast) {//text - введенные site:login; conf - путь до базы паролей; mast - мастер-пароль.
	int i = 0;
	string buff;
	string passw;
	ifstream f(conf);
	//Создаю массивы для логинов и паролей:
	string* login = new string[10001];
	string* pass = new string[10001];
	int master;
	f >> master;//исключаю хэш мастер-пароля из работы функции.
	while (!f.eof())
	{
		f >> buff;
		login[i] = reshifr(buff, mast);
		f >> buff;
		pass[i] = reshifr(buff, mast);
		i++;
	}
	f.close();
	cout << "Введите пароль для заполнения: ";
	cin >> passw;
	//Добавляю в массивы элементы:
	login[i] = text;
	pass[i] = passw;
	ofstream ff(conf);
	ff << master << '\n';
	//Вывод массивов с обавлеенными элементами:
	for (int j = 0; j < i; j++) {
		ff << shifr(login[j], mast) << ' ' << shifr(pass[j], mast) << '\n';
	}
	if (i >= 0)
		ff << shifr(login[i], mast) << ' ' << shifr(pass[i], mast);
	ff.close();
}

void del(string text, string conf, string mast) {//text - введенные site:login; conf - путь до базы паролей; mast - мастер-пароль.
	int i = 0;
	string buff;
	ifstream f(conf, ios::in);
	//Массивы для записи данных6
	string* login = new string[10000];
	string* pass = new string[10000];
	int master;
	f >> master;//исключаю хш мастер-пароля из работы фуункции
	while (!f.eof()) {
		f >> buff;
		login[i] = reshifr(buff, mast);
		f >> buff;
		pass[i] = reshifr(buff, mast);
		i++;
	}
	f.close();
	for (int j = 0; j < i; j++) {
		if (text == login[j]) {//Проверяю наличие нужного site:login в массиве.
			for (j; j < i; j++) {//если нахожу, удаляю и его и пароль смещением элементов массива на 1 к началу.
				login[j] = login[j + 1];
				pass[j] = pass[j + 1];
			}
			break;
		}
	}
	ofstream ff(conf);
	if (i != 0)
		ff << master << '\n';
	else
		ff << master;
	//Вывод полученных массивов:
	for (int j = 0; j < i - 2; j++)
		ff << shifr(login[j], mast) << ' ' << shifr(pass[j], mast) << '\n';
	if (i >= 2)
		ff << shifr(login[i - 2], mast) << ' ' << shifr(pass[i - 2], mast);
	ff.close();
}

void chk(string text, string conf, string mast) {//text - введенные site:login; conf - путь до базы паролей; mast - мастер-пароль.
	int i = 0;
	string buff;
	ifstream f(conf);
	string login;
	string pass;
	int master;
	f >> master;//исключаю хэш мастер-пароля из работы функции.
	while (!f.eof()) {
		f >> buff;
		login = reshifr(buff, mast);
		f >> buff;
		pass = reshifr(buff, mast);
		if (login == text) {//сравниваю слово с полученным site:login. Если совпадают, вывод "Password is found" и прекращение работы функции.
			cout << "Password is found"<< '\n';
			break;
		}
	}
	if (login != text)//если site:login не найден, выводится "Password is not found".
		cerr << "Password is not found"<<'\n';
	f.close();
}
