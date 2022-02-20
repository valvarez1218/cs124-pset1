#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

double EuclidDist(vector<double> coords1, vector<double> coords2) {
    if (coords1.size() != coords2.size()) {
        exit(0);
    }
    double rad = 0;
    for (int i = 0; i < coords1.size(); i++) {
        double diff = coords1[i] - coords2[i];
        rad += (diff * diff);
    }
    return sqrt(rad);
}

int main(void) {
    vector<double> coords1 {0.0, 1.0};
    vector<double> coords2 {1.0, 0.0};

    cout << EuclidDist(coords1, coords2) << endl;

    return 0;
}