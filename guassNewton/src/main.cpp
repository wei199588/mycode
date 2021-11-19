#include "gaussNewton.h"
using namespace std;

int main(int argc, char const *argv[])
{
    Eigen::Matrix<double, 3, 1> iniState;
    iniState << 2.0, -1.0, 5.0;
    int iterTime = 100;
    GaussNewton gn(iniState, iterTime);
    double ar = 1.0, br = 2.0, cr = 1.0;
    double wSigma = 1.0;
    double  invSigma = 1.0 / wSigma;
    cv::RNG rng;

    vector<double> xData, yData;
    for(int i = 0; i < 100; ++i)
    {
        double x = i/100.0;
        xData.push_back(x);
        yData.push_back(exp(ar*x*x+br*x+cr) + rng.gaussian(wSigma*wSigma));
    }

    std::vector<MatrixXd> Js;
    for(int i = 0; i < 100; ++i)
    {
    
        MatrixXd J;
        J.resize(1, 3);
        J.setZero();
        // J(0,1) = -xData.at(i)*xData.at(i)*exp()
    } 

    return 0;
}
   