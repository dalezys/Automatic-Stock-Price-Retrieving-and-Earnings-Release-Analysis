#ifndef Matrix_h
#define Matrix_h
#include <vector>

using namespace std;


typedef vector<double> Vector;
typedef vector<Vector> Matrix;

//Operator overloading
Vector operator*(const double& a,const Vector& V);
Vector operator+(const double& a,const Vector& V);
Vector operator+(const Vector& V,const Vector& W);
Vector operator-(const Vector& V,const Vector& W);
Vector operator*(const Vector& V,const Vector& W);
Vector exp(const Vector& V);
double operator^(const Vector& V,const Vector& W); //scalar operator, dot operation
#endif