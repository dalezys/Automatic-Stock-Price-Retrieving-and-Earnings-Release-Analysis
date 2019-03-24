#ifndef Stocks_h
#define Stocks_h

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "MyDate.h"
#include "Matrix.h"


class stock{
    //This class stores info about a single stock
private:
    string ticker;
    Date eps_date;
    
    vector<double> bench_price;
    vector<double> ret; //return
    vector<double> bench_ret;
    vector<Date> mydate;
    
public:
    int surprise;
    double est, act;
    vector<double> price;
    vector<double> AR;
    
    stock() {}
    stock(string ticker_, Date eps_date_, double act_, double est_, int surprise_):ticker(ticker_), eps_date(eps_date_), act(act_), est(est_), surprise(surprise_)
    { price.resize(241); bench_price.resize(241); ret.resize(240); bench_ret.resize(240);}
    //stock(const stock& stock);
    ~stock() {}
    
    void setprice(vector<Date>& mydate_, vector<double>& price_, vector<double>& bench_price_);
    string epsdate(){ return eps_date.GetDate();}
    string GetLabel();
    
};

#endif /* Stocks_h */
