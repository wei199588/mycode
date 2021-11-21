#include "gaussNewton.h"
#include <chrono>

GaussNewton::GaussNewton(std::vector<double> parameter)
{
    parameters_ = parameter;
}

GaussNewton::~GaussNewton()
{
}

void GaussNewton::solver(std::vector<double> argus, std::vector<double> dependentVariables, int maxIer, double epsilon)
{
    observeNum_ = argus.size();
    argus_ = argus;
    dependentVariables_ = dependentVariables;
    bool isConvergent = false;
    unsigned int count = 0;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    for (int i = 0; i < maxIer; ++i)
    {
        Eigen::MatrixXd jacobi;
        Eigen::MatrixXd fx;
        compute_Jacobi_Fx(jacobi, fx);
        Eigen::MatrixXd H = jacobi.transpose() * jacobi;
        Eigen::MatrixXd g = -jacobi.transpose() * fx;

        Eigen::VectorXd deltaX = H.ldlt().solve(g); //求解线性方程 Hx = b
        ++count;

        if (deltaX.norm() < epsilon)
        {
            isConvergent = true;
            break;
        }
        for (int j = 0; j < parameters_.size(); ++j)
        {
            parameters_.at(j) += deltaX(j);
        }
    }
    if (isConvergent)
    {
        std::cout << "converged!" << std::endl;
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        chrono::duration<double> tmUsed = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        std::cout << "iterate times: " << count << ", spend time: " << tmUsed.count() * 1000 << " millisecond." << std::endl;
        std::cout << "------------------------------------" << std::endl;
        std::cout << "estimate para: ";
        for (int j = 0; j < parameters_.size(); ++j)
        {
            std::cout << parameters_.at(j) << " ";
        }
        std:: cout << std::endl;
    }
    else
        std::cout << "disconverged!" << std::endl;
}

void GaussNewton::compute_Jacobi_Fx(Eigen::MatrixXd &jacobi, Eigen::MatrixXd &fx)
{
    jacobi.resize(observeNum_, parameters_.size());
    fx.resize(observeNum_, 1);
    for (int i = 0; i < observeNum_; ++i)
    {
        for (int j = 0; j < parameters_.size(); ++j)
        {
            jacobi(i, j) = setBias(i, j);
        }
        fx(i, 0) = dependentVariables_.at(i) - setEquationValue(i);
    }
}

double GaussNewton::setBias(int n, int biasMod)
{
    double xi = argus_.at(n);
    double a = parameters_.at(0);
    double b = parameters_.at(1);
    double c = parameters_.at(2);

    double ret = 0.0;

    switch (biasMod)
    {
    case 0:
        ret = -xi * xi * exp(a * xi * xi + b * xi + c);
        break;
    case 1:
        ret = -xi * exp(a * xi * xi + b * xi + c);
        break;
    case 2:
        ret = -exp(a * xi * xi + b * xi + c);
        break;
    default:
        break;
    }
    return ret;
}

double GaussNewton::setEquationValue(double n)
{
    double xi = argus_.at(n);
    double a = parameters_.at(0);
    double b = parameters_.at(1);
    double c = parameters_.at(2);
    return exp(a * xi * xi + b * xi + c);
}