#include <string>
#include <iostream>

using namespace std;

//шифрование шифром Вижинера:
string shifr(string text, string master) {

    string D = "";//перемемнная, которая заполняется ключом несколько раз, чтобы длина слова и длина даной переменной были равны.
    string A = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-=`~!@#$%^&*()_+|[]{};':,<.>/?";//Алфавит символов.
    int b = int(text.size());
    int c = int(master.size());
    int* F = new int[b];
    int* G = new int[b];
    //если ключ меньше либо равен слову:
    if (b >= c) {
        for (int i = 0; i < (b / c); i++)
            D += master;
        for (int i = 0; i < (b % c); i++)
            D += master[i];
    }
    else {//если ключ больше слова
        for (int i = 0; i < b; i++)
            D += master[i];
    }
    //заполняем массивы индексами букв или символов в алфавите:
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < 92; j++) {
            if (text[i] == A[j])
                F[i] = j;
            if (D[i] == A[j])
                G[i] = j;
        }
    }
    
    //шифруем слово, прибавляя к индексу i-ой буквы слова индекс i-ой буквы ключа и берем остаток от деления на 92(для случаев, когда сумма больше длины алфавита):
    int e;
    for (int i = 0; i < b; i++) {
        e = (F[i] + G[i]) % 92;
        text[i] = A[e];
    }
    return text;
}

//функция дешифрования:
string reshifr(string text, string master) {//за исключением финала, ничем не отличается.
    string D = "";
    string A = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-=`~!@#$%^&*()_+|[]{};':,<.>/?";
    int b = int(text.size());
    int c = int(master.size());
    int* F = new int[b];
    int* G = new int[b];
    if (b >= c) {
        for (int i = 0; i < (b / c); i++)
            D += master;
        for (int i = 0; i < (b % c); i++)
            D += master[i];
    }
    else {
        for (int i = 0; i < b; i++)
            D += master[i];
    }
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < 92; j++) {
            if (text[i] == A[j])
                F[i] = j;
            if (D[i] == A[j])
                G[i] = j;
        }
    }
    //отнимаем от i-ой буквы слова i-ый символ ключа, чтобы вернуть первоначальное слово(прибавляем 92 для избежания отрицательных значений).
    int e;
    for (int i = 0; i < b; i++) {
        e = (92 + F[i] - G[i]) % 92;
        text[i] = A[e];
    }
    return text;
}
