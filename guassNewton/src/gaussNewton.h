#ifndef GAUSSNEWTON_H
#define GAUSSNEWTON_H
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <vector>
using namespace std;
using namespace Eigen;

class GaussNewton
{
public:
    GaussNewton(std::vector<double> parameter);
    ~GaussNewton();

    void solver(std::vector<double> argus, std::vector<double> dependentVariables, double epsilon = 1e-5, int maxIer = 1000);
private:
    void compute_Jacobi_Fx(Eigen::MatrixXd &jacobi, Eigen::MatrixXd &fx);
    virtual double setBias(int n, int biasMod);
    virtual double setEquationValue(double n);

private:
    int observeNum_ = 0; // num of data
    std::vector<double> parameters_; // num of unknown para.
    std::vector<double> argus_; // x's values.
    std::vector<double> dependentVariables_; // y's values
};


#endif // !GAUSSNEWTON_H