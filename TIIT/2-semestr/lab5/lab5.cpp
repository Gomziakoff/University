#include <iostream>
#include <cmath>
#include <iomanip>
#include "f.h"

using namespace std;


int main() {
	srand(time(NULL));
	vector<double> data = {0};
	int ill, day = 1, contacts, dead = 0, cured = 0;
	ill = frand(5, 10);
	agent* people = new agent[ill];
	cout << setw(8) << "day" << setw(8) << "ill" << setw(8) << "dead" << setw(8) << "cured" << setw(8) << "ill+" << setw(8) << "dead+" << setw(8) << "cured+"<<endl;
	for (day; day < 51; day++) {
		int ill_today = 0, dead_today = 0,cured_today = 0;
		contacts = 0;
		for (int i = 0; i < ill; i++) {//вычисляем количество контактов
			if (people[i].status == 2) {
				people[i].SC = people[i].getSC();
				contacts += people[i].SC;
			}
			else if (people[i].status == 3) {
				people[i].SC = people[i].getSC() / people[i].R;
				contacts += people[i].SC;
			}
		}

		
		for (int i = 0; i < ill; i++) {
			double rand_test = (int)frand(0, 100);//проверяем узнал ли человек о болезни
			if (people[i].status == 2) {
				if (reg.T > rand_test) {
					people[i].status = 3;
				}
			}
		}

		
		for (int i = 0; i < ill; i++) {
			double rand_HA = (int)frand(0, 100);//проверяем попал ли человек на лечение
			if (people[i].status == 3) {
				if (people[i].HA < 2 && rand_HA < 10) people[i].status = 4;
				if (people[i].HA > 2 && rand_HA < 1) people[i].status = 4;
			}
		}

		for (int i = 0; i < ill; i++) {//перерасчет времени выздоровления и шанса смерти для попавших на лечение
			if (people[i].status == 4 && people[i].started == 0) {
				people[i].RT = people[i].getRT();
				people[i].DR = people[i].getDR();
				people[i].started = 1;
			}
		}

		
		for (int i = 0; i < ill; i++) {
			double death_rand = (int)frand(0, 100);//проверка на смерть
			if (people[i].status != 1) {
				if (death_rand < people[i].DR) {
					people[i].status = 1;
					dead_today++;
					dead++;
				}
			}
		}

		for (int i = 0; i < ill; i++) { //проверяем сколько человек вылечилось
			if (people[i].status != 1) {
				people[i].RT--;
				if (people[i].RT < 1) {
					people[i].status = 1;
					cured++;
					cured_today++;
				}
			}
		}

		ill_today = ill / (ebola.I * 100);//считаем кол-во новых больных
		data.push_back(ill_today/100);
		agent* buf = new agent[ill];
		
		ill += ill_today;
		people = new agent[ill];
		for (int i = 0; i < ill-ill_today; i++) {
					people[i]=buf[i];
				}
		
		cout << setw(8) << day << setw(8) << ill << setw(8) << dead << setw(8) << cured << setw(8) << ill_today << setw(8) << dead_today << setw(8) << cured_today << endl;
	}

	//делаем графичек
	int max_value = *max_element(data.begin(), data.end());
	for (int i = max_value; i >= 0; --i) {
		cout.width(4);
		cout << i*100 << " | ";
		for (int j = 0; j < data.size(); ++j) {
			if (data[j] >= i) {
				cout << "x ";
			}
			else {
				cout << "  ";
			}
		}
		cout << endl;
	}
	cout << "---------------------------------------"
		<< endl;

	
	//дополнительные вычисления
	cout << "\nexpectrd value 20.5%\n";


}