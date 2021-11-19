#include "gaussNewton.h"
#include <chrono>

GaussNewton::GaussNewton(std::vector<double> parameter)
{
    parameters_ = parameter;
}

GaussNewton::~GaussNewton()
{
}

void GaussNewton::solver(std::vector<double> argus, std::vector<double> dependentVariables, double epsilon = 1e-5, int maxIer = 1000)
{
    observeNum_ = argus.size();
    argus_ = argus;
    dependentVariables_ = dependentVariables;
    bool isConvergent = false;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    for (int i = 0; i < maxIer; ++i)
    {
        Eigen::MatrixXd jacobi;
        Eigen::MatrixXd fx;
        compute_Jacobi_Fx(jacobi, fx);
        Eigen::MatrixXd H = jacobi.transpose() * jacobi;
        Eigen::MatrixXd g = -jacobi.transpose()*fx;
    }

    // int row = jacobiMs.at(0).rows();
    // // int col = jacobiM.cols();

    // for (int iter = 0; iter < iterationTime_; ++iter)
    // {
    //     MatrixXd H;
    //     H.resize(row, row);
    //     H.setZero();
    //     MatrixXd g;
    //     g.resize(row, 1);
    //     g.setZero();
    //     cost = 0.0;
    //     for(int j = 0; j < jacobiMs.size(); ++j)
    //     {
    //         H += jacobiMs.at(j) * jacobiMs.at(j).transpose();
    //         g = -jacobiMs.at(j) * fValues.at(j);
    //         cost += fValues.at(j) * fValues.at(j);
    //     }

    //     MatrixXd dx;
    //     dx.resize(row, 1);
    //     dx.setZero();
    //     H.ldlt().solve(g); //求解线性方程 Hx = b

    //     if (isnan(dx[0]))
    //     {
    //         std::cout << "result is nan!" << std::endl;
    //         break;
    //     }
    //     if (iter > 0 && cost >= costValue_)
    //     {
    //         std::cout << "cost: " << cost << " >= last cost: " << costValue_ << std::endl;
    //         break;
    //     }

    //     for (int i = 0; i < row; ++i)
    //     {
    //         stateValue_(i, 0) += dx(i, 0);
    //     }

    //     lastCost = cost;

    //     std::cout << "total cost: " << cost << "\t\tupdate: " << dx.transpose() << "\t\testimated para: " << stateValue_.transpose() << std::endl;
    // }

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> tmUsed = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    std::cout << "solve time cost: " << tmUsed.count() << "seconds." << std::endl;
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