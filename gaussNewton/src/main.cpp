#include "gaussNewton.h"
using namespace std;

int main(int argc, char const *argv[])
{
#if SOLVEOMIGA
    double omiga = 2;
    std::vector<double> para = {4};
    const int iterateTime = 100;
    cv::RNG rng(cv::getTickCount());
    std::vector<double> x, y;
    const int N = 5;
    x.resize(N);
    y.resize(N);
    for(int i = 0; i < N; ++i)
    {
        x[i] = rng.uniform(5.0, 10.0);
        y[i] = omiga*x[i] + rng.gaussian(0.05);
    }
#else
    double a = 0.5;
    double b = 0.5;
    double c = 2.0;
    std::vector<double> para = {0.3, 0.4, 1.8};
    const int N = 100;
    cv::RNG rng(cv::getTickCount());
    std::vector<double> x, y;
    x.resize(N);
    y.resize(N);
    std::cout << "real para: " << a <<" " << b << " " << c << std::endl;
    for(int i = 0; i < N; ++i)
    {
        x[i] = rng.uniform(0.0, 1.0);
        y[i] = exp(a * x[i] * x[i] + b * x[i] + c) + rng.gaussian(0.05);
    }
#endif  
    GaussNewton gnSolve(para);
    gnSolve.solver(x, y, iterateTime, 1.0e-10);

    return 0;
}
   