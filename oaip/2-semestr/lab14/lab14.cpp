#include <iostream>

using namespace std;

struct list {
	int data;
	list* next;
};

void menu(list* head);
void create_list(list*& head);
void create_node(list*& head);
void add(list*& head);
void del(list*& head);
void output(list* head);
void reverce(list*& head);
bool empty(list* head);

int main() {
	list* head;
	head = NULL;
	menu(head);
}

void menu(list *head) {
	int choise;
	while (true) {
		cout << "выберите действие:\n";
		cout << "1.создание списка\n";
		cout << "2.добавление элемента\n";
		cout << "3.удаление элемента\n";
		cout << "4.просмотр списка\n";
		cout << "5.инверсия списка\n";
		cout << "6.выход\n";
		cin >> choise;
		switch (choise) {
		case 1:
			system("cls");
			create_list(head);
			break;
		case 2:
			system("cls");
			add(head);
			break;
		case 3:
			system("cls");
			del(head);
			break;
		case 4:
			system("cls");
			output(head);
			break;
		case 5:
			system("cls");
			reverce(head);
			break;
		case 6:
			return;
			break;
		}
	}
}

void create_list(list*&head) {
	head = NULL;
	cout << "введите колво элементов списка\n";
	int n;
	cin >> n;
	cout << "введите "<<n<<" элементов списка\n";
	for (int i = 0; i < n; i++) {
		create_node(head);
	}
	if (n > 0) {
		output(head);
	}
}

void create_node(list*& head) {
	list *new_node = new list;
	cin >> new_node->data;
	new_node->next = NULL;

	list* q = head;
	if (q == NULL) {
		new_node->next = head;
		head = new_node;
	}
	else {
		while (q->next) {
			q = q->next;
		}
		new_node->next = q->next;
		q->next = new_node;
	}
}

void del(list*& head) {
	if (empty(head) == 1) {
		output(head);
		cout << "введите элемент для удаления";
		int d;
		cin >> d;
		list* q = head;
		bool find = false;
		while (q) {
			if (d == q->data) {
				find = true;
				if (q != NULL) {
					if (q == head) {
						head = head->next;
						delete q;
						q = head;
					}
					else {
						list* temp = head;
						while (temp->next != q) {
							temp = temp->next;
						}
						temp->next = q->next;
						delete q;
						q = temp;
					}
				}
			}
			else q = q->next;
		}
		if (find == false) {
			cout << "Данный элемент не найден.\n";
		}
		else {
			output(head);
			cin.ignore();
		}

	}
}

void output(list* head) {
	if (empty(head) == 1) {
		list* q = head;
		cout << "список\n";
		while (q) {
			cout << q->data<<" ";
			q = q->next;
		}
		cout << endl;
	}
}

void reverce(list*& head) {
	if (empty(head) == 1) {
		list* reversed_list=NULL;
		for (list* node = head, *next_node; node != NULL; node = next_node) {
			next_node = node->next;
			node->next = reversed_list;
			reversed_list = node;
		}
		head = reversed_list;
		output(head);
	}
}

bool empty(list* head) {
	if (head != NULL) return 1; else {
		cout << "список пуст\n";
		return 0;
	}
}

void add(list*& head) {
	int choise;
	do {
		cout << "введите новый элемент списка: ";
		create_node(head);
		cout << "хотите добавить еще элемент? (1 - да, 2 - нет)\n";
		cin >> choise;
		if (choise != 1 && choise != 2) {
			cout << "Вы ввели неправильный номер.\nВвод будет прекращен.\n";
			cin.ignore();
			cin.ignore();
		}
	} while (choise != 2);
	output(head);
}
