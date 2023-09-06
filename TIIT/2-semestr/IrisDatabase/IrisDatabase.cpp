#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Iris {//наши ириски
    float sepal_length;
    float sepal_width;
    float petal_length;
    float petal_width;
    string class_type;
};

float dist(float* iris_check, float* test_data) {//функция рассчета расстояния
    int distance = 0;
    for (int i = 0; i < 4; i++)
        distance += powf(iris_check[i] - test_data[i], 2);
    return sqrt(distance);
}

int main() {

	ifstream db;
	db.open("iris.data");
    vector<Iris> iris_inf;

    string line,buf;
    while (getline(db, line)) {//берем базу из файла и загружаем в прогу
        Iris iris;
        istringstream str(line);
        getline(str, buf, ',');
        iris.sepal_length = stof(buf);
        getline(str, buf, ',');
        iris.sepal_width = stof(buf);
        getline(str, buf, ',');
        iris.petal_length = stof(buf);
        getline(str, buf, ',');
        iris.petal_width = stof(buf);
        getline(str, buf);
        iris.class_type = buf;
        iris_inf.push_back(iris);
    };

    float** test_data = new float* [iris_inf.size()];//создаем массив для рассчетов (не умею через векторы)
    for (int i = 0; i < iris_inf.size(); i++) {
        test_data[i] = new float[4];
    }

    for (int i = 0; i < iris_inf.size(); i++) {//удобнее для кода рассчетов
        test_data[i][0] = iris_inf[i].sepal_length;
        test_data[i][1] = iris_inf[i].sepal_width;
        test_data[i][2] = iris_inf[i].petal_length;
        test_data[i][3] = iris_inf[i].petal_width;
    }

    // w0+w1*sl+w2*sw+w3*pl+w4*pw = [0, 1]
    // w0... [0 , 1] versicolor
    float iris_check[4];//какой ирис мы определяем
    cout << "input sepal length\n";
    cin >> iris_check[0];
    cout << "input sepal width\n";
    cin >> iris_check[1];
    cout << "input petal length\n";
    cin >> iris_check[2];
    cout << "input petal width\n";
    cin >> iris_check[3];
    system("cls");

    float** distances = new float* [iris_inf.size()];//считаем расстояния
    for (int i = 0; i < iris_inf.size(); i++) {
        distances[i] = new float[2];
    }

    for (int i = 0; i < iris_inf.size(); i++) {//рассчет дистанций и сразу запоминаем какой вид чтобы было легче
        distances[i][0] = dist(iris_check, test_data[i]);
        if (iris_inf[i].class_type == "setosa") {
            distances[i][1] = 1;
        }
        else if (iris_inf[i].class_type == "versicolor") {
            distances[i][1] = 2;
        }
        else distances[i][1] = 3;
    }
    for (int i = 0; i < iris_inf.size()-1; i++) {
        for (int j = i+1; j < iris_inf.size(); j++) {
            if (distances[i][0] > distances[j][0])
                swap(distances[i], distances[j]);
        }
    }

    double k = 10;//можно дабавить выбираемое колво соседей*
    int setosa = 0, versicolor = 0, virginica = 0;
    for (int i = 0; i < k; i++) {//отбираем н соседей и считаем сколько каких
        if (distances[i][1] == 1) setosa++;
        if (distances[i][1] == 2) versicolor++;
        if (distances[i][1] == 3) virginica++;
    }
    
    cout << "ваш цветок:\n";
    cout << "setosa " << (setosa / k)*100 << "%\n";
    cout << "versicolor " << versicolor / k * 100<< "%\n";
    cout << "virginica " << virginica / k * 100<< "%\n";
}