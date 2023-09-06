#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>

#define e  2.71828182845904523536

using namespace std;

struct Iris {//наши ириски
    float sepal_length;
    float sepal_width;
    float petal_length;
    float petal_width;
    string class_type;
};

vector<Iris> readData(string filename) {//считывание файла v2.0 (замена запятых на пробелы тема)
    vector<Iris> irisData;
    ifstream db(filename);
    float sepal_length, sepal_width, petal_length, petal_width;
    string class_type;
    while (db >> sepal_length >> sepal_width >> petal_length >> petal_width >> class_type) {
        irisData.push_back({ sepal_length, sepal_width, petal_length, petal_width, class_type});
    }
    return irisData;
}

void norm(vector<Iris>& irisData) {
    float min1 = 100, min2 = 100, min3 = 100, min4 = 100, max1 = 0, max2 = 0, max3 = 0, max4 = 0;
    for (int i = 0; i < irisData.size(); i++) {
        if (irisData[i].sepal_length < min1) min1 = irisData[i].sepal_length;
        if (irisData[i].sepal_length > max1) max1 = irisData[i].sepal_length;
        if (irisData[i].sepal_width < min2) min2 = irisData[i].sepal_width;
        if (irisData[i].sepal_width > max2) max2 = irisData[i].sepal_width;
        if (irisData[i].petal_length < min3) min3 = irisData[i].petal_length;
        if (irisData[i].petal_length > max3) max3 = irisData[i].petal_length;
        if (irisData[i].petal_width < min4) min4 = irisData[i].petal_width;
        if (irisData[i].petal_width > max4) max4 = irisData[i].petal_width;
    }
    for (int i = 0; i < irisData.size(); i++) {
        irisData[i].sepal_length = (irisData[i].sepal_length - min1) / (max1 - min1);
        irisData[i].sepal_width = (irisData[i].sepal_width - min2) / (max2 - min2);
        irisData[i].petal_length = (irisData[i].petal_length - min1) / (max3 - min3);
        irisData[i].petal_width = (irisData[i].petal_width -min1) / (max4 - min4);
    }
}

void split(const vector<Iris>& irisData, vector<Iris>& trainData, vector<Iris>& testData, vector<Iris>& validData, float tests, float valid) {//разделение на обучение и тест
    int numTests = irisData.size() * tests;
    int numValid = irisData.size() * valid;
    vector<int> index(numTests+numValid);
    for (int i = 0; i < numTests+numValid; i++) {
        index[i] = rand() % irisData.size();
        for (int j = 0; j < i - 1; j++) {
            if (index[i] == index[j]) {
                index[i] = rand() % irisData.size();
                j = 0;
            }
        }
    }
    sort(index.begin(), index.end());
    int DataIndex = 0;
    for (int i = 0; i < irisData.size(); i++) {
        if (DataIndex%2 == 0 && DataIndex < index.size() && i == index[DataIndex]) {
            testData.push_back(irisData[i]);
            DataIndex++;
        }
        else if (DataIndex % 2 == 1 && DataIndex < index.size() && i == index[DataIndex]) {
            validData.push_back(irisData[i]);
            DataIndex++;
        }
        else {
            trainData.push_back(irisData[i]);
        }
    }
}

double frand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double N(double m, double sigm) {//Преобразование Бокса — Мюллера, нормальная генерация
    double x, y, z;
    x = frand(0, 1);
    y = frand(0, 1);
    z = cos(2 * 3.14 * x) * sqrt(-2 * log(y));
    return m + sigm * z;
}

float MSE(float* weights, vector<Iris> trainData, vector<int> y) {
    float y_exp;
    float sum = 0;
    for (int i = 0; i < trainData.size(); i++) {
        y_exp = weights[0] + weights[1] * trainData[i].sepal_length + weights[2] * trainData[i].sepal_width + weights[3] * trainData[i].petal_length + weights[4] * trainData[i].petal_width;
        sum += sqrt(pow(y[i] - y_exp, 2));
    }
    return 1.0 / y.size() * sum;
}

void train(float*& weights, vector<Iris> trainData, string type) {
    double alf = 0.3, c = 0.99;
    vector<int> y;
    float weights1[5];
    for (int i = 0; i < trainData.size(); i++) {
        if (trainData[i].class_type == type) {
            y.push_back(1);
        }
        else {
            y.push_back(-1);
        }
    }
    double MSE1 = MSE(weights, trainData, y), MSE2;
    while (alf > 0.000000000000001) {
        for (int i = 0; i < 5; i++) {
            weights1[i] = weights[i] + alf * N(0, 1);
        }
        MSE2 = MSE(weights1, trainData, y);
        if (MSE2 < MSE1) {
            for (int i = 0; i < 5; i++) {
                weights[i] = weights1[i];
            }
            MSE1 = MSE2;
        }
        alf *= c;
    }
}

double sigm(float x){
    return powl(e, x) / (1 + pow(e, x));
}
// Нормализация - уmеньшить alpha check
// валидация check
// rand - N(0, 1) check
// model ?
// sigmoid check
float test(vector<Iris> testData,float* ws,float* wvs,float* wvi) {
    double setosa, versicolor, virginica;
    double num = 0;
    string otv;
    for (int i = 0; i < testData.size(); i++) {
       setosa = ws[0] + ws[1] * testData[i].sepal_length + ws[2] * testData[i].sepal_width + ws[3] * testData[i].petal_length + ws[4] * testData[i].petal_width;
       versicolor = wvs[0] + wvs[1] * testData[i].sepal_length + wvs[2] * testData[i].sepal_width + wvs[3] * testData[i].petal_length + wvs[4] * testData[i].petal_width;
       virginica = wvi[0] + wvi[1] * testData[i].sepal_length + wvi[2] * testData[i].sepal_width + wvi[3] * testData[i].petal_length + wvi[4] * testData[i].petal_width;
       setosa = sigm(setosa);
       versicolor = sigm(versicolor);
       virginica = sigm(virginica);
       if (setosa >= versicolor && setosa >= virginica) {
           otv = "setosa";
       }
       else if (versicolor >= setosa && versicolor >= virginica) {
           otv = "versicolor";
       }
       else otv = "virginica";
       if (otv == testData[i].class_type) {
           num++;
       }
       else {
           cout <<"right ansv: " << testData[i].class_type<<" wrong ansv: " << otv << endl;
           int sum = setosa + versicolor + virginica;
           cout << setosa*100/sum << "% " << versicolor * 100 / sum << "% " << virginica * 100 / sum <<"%" << endl;
       } 
    }
    float q = num / testData.size() * 100;
    return q;
}

int main() {
    srand(time(NULL));
    float q = 0;
    vector<Iris> irisData = readData("iris.data");
    norm(irisData);
    vector<Iris> trainData;
    vector<Iris> testData;
    vector<Iris> validData;
    split(irisData, trainData, testData,validData, 0.15,0.15);
    
        float* weights_setosa = new float [5] {0, 0, 0, 0, 0};
        float* weights_versicolor = new float [5] {0, 0, 0, 0, 0};
        float* weights_virginica = new float [5] {0, 0, 0, 0, 0};

        float validQ = 0, validQ1 = 0, num = 0; //обучение на валидационной группе
        while (num != 5) {
            train(weights_setosa, trainData, "setosa");
            train(weights_versicolor, trainData, "versicolor");
            train(weights_virginica, trainData, "virginica");
            validQ = test(validData, weights_setosa, weights_versicolor, weights_virginica);
            cout << validQ << endl;
            if (validQ == validQ1) num++; else num = 0;
            validQ1 = validQ;
        }

            q = test(testData, weights_setosa, weights_versicolor, weights_virginica);//итог обучения
            cout << q << "%" << endl;
        
}