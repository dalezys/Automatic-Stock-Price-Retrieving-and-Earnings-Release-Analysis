#include "Matrix.h"
#include <cmath>
#include <iostream>
using namespace std;

Vector operator*(const double& a,const Vector& V)
{
    int n = V.size();
    Vector W(n);
    for (int j = 0; j < n; ++j)
    { W[j] = V[j] * a; }
    return W;
}

Vector operator+(const double& a,const Vector& V)
{
    int n = V.size();
    Vector W(n);
    for (int j = 0; j < n; ++j)
    { W[j] = V[j] + a; }
    return W;
}

Vector operator+(const Vector& V,const Vector& W)
{
    unsigned long d = V.size();
    Vector U(d);
    for (int j=0; j<d; j++) U[j] = V[j] + W[j];
    return U;
}

Vector operator*(const Vector& V,const Vector& W)
{
    int n = V.size();
    int m = W.size();
    if (n != m){cout<<"* op, dim mismatch"<<endl;}
    Vector M(n);
    for (int j = 0; j < n; ++j)
    { M[j] = V[j] * W[j]; }
    return M;
}

Vector exp(const Vector& V)
{
    int n = V.size();
    Vector W(n);
    for (int j = 0; j < n; ++j)
    { W[j] = exp(V[j]); }
    return W;
}

double operator^(const Vector& V,const Vector& W)        // scalar product
{
    double sum = 0.0;
    int n = V.size(); //dimension
    for (int j = 0; j < n; ++j)
    { sum = sum + V[j]*W[j]; }
    return sum;
}

Vector operator-(const Vector& V,const Vector& W){
    int n = V.size();
    int m = W.size();
    if (n != m){cout<<"- op, dim mismatch"<<endl;}
    Vector M(n);
    for (int j = 0; j < n; ++j)
    { M[j] = V[j] - W[j]; }
    return M;
}
