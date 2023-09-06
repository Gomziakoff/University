#include <iostream>
#include <cmath>
using namespace std;

// Структура для хранения слагаемого многочлена
struct Term {
    double coeff; // коэффициент
    int exp; // степень
    Term* next; // указатель на следующий элемент списка
};

// Функция для добавления нового слагаемого в список
void addTerm(Term*& head, double coeff, int exp) {
    Term* newTerm = new Term;
    newTerm->coeff = coeff;
    newTerm->exp = exp;
    newTerm->next = NULL;
    if (head == NULL) {
        head = newTerm;
    }
    else {
        Term* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newTerm;
    }
}

// Функция для вывода многочлена на экран
void displayPolynomial(Term* head) {
    if (head == NULL) {
        cout << "0" << endl;
        return;
    }
    int k = 0;
    while (head != NULL) {
        if (k > 0) {
            cout << "+";
        }
        k++;
        if (head->exp == 0) {
            cout << head->coeff;
        }
        else if (head->exp == 1) {
            cout << head->coeff << "x";
        }
        else {
            cout << head->coeff << "x^" << head->exp;
        }
        head = head->next;
    }
    cout << endl;
}

// Функция для взятия производной многочлена
void differentiate(Term*& head) {
    if (head == NULL) {
        cout << "Многочлен пуст." << endl;
        return;
    }
    Term* temp = head;
    while (temp != NULL) {
        temp->coeff = temp->coeff * temp->exp;
        temp->exp = temp->exp - 1;
        if (temp->exp < 0) {
            // удаление слагаемого с отрицательной степенью
            if (temp == head) {
                head = temp->next;
                delete temp;
                temp = head;
            }
            else {
                Term* prev = head;
                while (prev->next != temp) {
                    prev = prev->next;
                }
                prev->next = temp->next;
                delete temp;
                temp = prev->next;
            }
        }
        else {
            temp = temp->next;
        }
    }
}

// Главная функция
int main() {
    Term* head = NULL;
    int option = 0;
    while (option != 3) {
        cout << "Выберите действие:" << endl;
        cout << "1. Добавить слагаемое в многочлен" << endl;
        cout << "2. Взять производную многочлена" << endl;
        cout << "3. Выйти из программы" << endl;
        cout << "Ваш выбор: ";
        cin >> option;
        if (option == 1) {
            double coeff;
            int exp;
            cout << "Введите коэффициент и степень слагаемого: ";
            cin >> coeff >> exp;
            addTerm(head, coeff, exp);
            cout << "Слагаемое добавлено." << endl;
            cout << "Текущий многочлен: ";
            displayPolynomial(head);
        }
        else if (option == 2) {
            differentiate(head);
            cout << "Производная многочлена: ";
            displayPolynomial(head);
        }
        else if (option == 3) {
            // Освобождение памяти
            Term* temp;
            while (head != NULL) {
                temp = head->next;
                delete head;
                head = temp;
            }
            cout << "Программа завершена." << endl;
        }
        else {
            cout << "Некорректный выбор. Попробуйте еще раз." << endl;
        }
    }
    return 0;
}