#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#define pi 3.14159

using namespace std;

double func(double x) {
    return x * sin(x);
}

int main()
{
    //set = [100, 500, 1000]
    //if (i in set) {}
    //bool condition = i== 1000 || 
    double x, y, ox = pi, oy = 4, N = 0, n = 0, I = pi; 
    double ux, uy;
    for (int i = 0; i < 10001; i++) {
        ux = (float)rand() / (float)RAND_MAX;
        uy = (float)rand() / (float)RAND_MAX;
        x = ox * ux;
        y = oy * uy;
        if (y <= func(x)) n++;
        if (i == 100) cout << "N = " << i << " n = " << n << " Sф = " << ((ox * oy) * n) / i << " I = " << I << " Error = " << abs((((ox * oy) * n) / i) - I) / I << "\n";
        if (i == 500) cout << "N = " << i << " n = " << n << " Sф = " << ((ox * oy) * n) / i << " I = " << I << " Error = " << abs((((ox * oy) * n) / i) - I) / I << "\n";
        if (i == 1000) cout << "N = " << i << " n = " << n << " Sф = " << ((ox * oy) * n) / i << " I = " << I << " Error = " << abs((((ox * oy) * n) / i) - I) / I << "\n";
        if (i == 3000) cout << "N = " << i << " n = " << n << " Sф = " << ((ox * oy) * n) / i << " I = " << I << " Error = " << abs((((ox * oy) * n) / i) - I) / I << "\n";
        if (i == 10000) cout << "N = " << i << " n = " << n << " Sф = " << ((ox * oy) * n) / i << " I = " << I << " Error = " << abs((((ox * oy) * n) / i) - I) / I << "\n";
    }
    
}
