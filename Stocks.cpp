#include <stdio.h>
#include <math.h>
#include "Stocks.h"
#include "Matrix.h"

void stock::setprice(vector<Date>& mydate_, vector<double>& price_, vector<double>& bench_price_)
{
    //bool t;
    for ( int j = 0 ; j < mydate_.size() ; ++j ){
        if ( mydate_[j] == eps_date){
            int i = j - 120;
            int k = j + 120;
            if ( i < 0 || k > mydate_.size())
            { cout<<ticker<<" error "<<j<<endl; }//break
            else
            {
                price[0] = price_[i];
                bench_price[0] = bench_price_[i];
    
                for (int l = 0 ; l < 240 ; ++l)
                {
                    price[l+1] = price_[i+l+1];
                    bench_price[l+1] = bench_price_[i+l+1];
                    ret[l] = price[l+1]/price[l] - 1;
                    bench_ret[l] = bench_price[l+1]/bench_price[l] - 1;
                }
                AR = ret - bench_ret;
            }
            break;
        }
    }
}
string stock::GetLabel()
{
    string result;
    if (surprise == -1) result = "miss";
    else if (surprise == 0) result = "meet";
    else result = "beat" ;
    return result;
}

