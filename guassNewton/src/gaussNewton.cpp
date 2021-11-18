#include "gaussNewton.h"
#include <chrono>

GaussNewton::GaussNewton(MatrixXd stateValue, int iterTime)
{
    init(stateValue, iterTime);
}

GaussNewton::~GaussNewton()
{
}

void GaussNewton::init(MatrixXd stateValue, int iterTime)
{
    stateValue_ = stateValue;
    iterationTime_ = iterTime;
}

void GaussNewton::solver(MatrixXd jacobiM, double fValue)
{
    int row = jacobiM.rows();
    // int col = jacobiM.cols();
    MatrixXd H;
    H.resize(row, row);
    H.setZero();
    MatrixXd g;
    g.resize(row, 1);
    g.setZero();
    double cost =  0;

    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

    for (int iter = 0; iter < iterationTime_; ++iter)
    {
        H = jacobiM * jacobiM.transpose();
        g = -jacobiM * fValue;
        cost += fValue * fValue;

        MatrixXd dx;
        dx.resize(row, 1);
        dx.setZero();
        H.ldlt().solve(g); //求解线性方程 Hx = b
        if (isnan(dx[0]))
        {
            std::cout << "result is nan!" << std::endl;
            break;
        }
        if(iter > 0 && cost >= costValue_)
        {   
            std::cout << "cost: " << cost << " >= last cost: " << costValue_ << std::endl;
            break;
        }

        for(int i = 0; i < row; ++i)
        {
            stateValue_(i, 0) += dx(i, 0);
        }

        costValue_ = cost;

        std::cout << "total cost: " << cost << "\t\tupdate: " << dx.transpose() << "\t\testimated para: " << stateValue_.transpose() << std::endl;
    }

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> tmUsed = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    std::cout << "solve time cost: " << tmUsed.count() << "seconds." << std::endl;
    std::cout << "estimated: " << stateValue_.transpose() << std::endl;

}