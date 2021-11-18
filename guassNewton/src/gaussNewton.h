#ifndef GAUSSNEWTON_H
#define GAUSSNEWTON_H
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

class GaussNewton
{
public:
    GaussNewton(MatrixXd stateValue, int iterTime);
    ~GaussNewton();

    void solver(MatrixXd jacobiM, double fValue);
private:
    void init(MatrixXd stateValue, int iterTime);

private:
    int iterationTime_ = 0;
    double costValue_ = 0.0;

    MatrixXd stateValue_;
    MatrixXd jacobiM_;
};


#endif // !GAUSSNEWTON_H