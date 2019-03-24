#include <algorithm>
#include <random>
#include <chrono>
#include "Matrix.h"
#include "Bootstrapping.h"

Bootstrapping::Bootstrapping(map<int, vector<string> >& all)
{
    AR_M.resize(3);
    for (int i = 0 ; i < 3 ; i++)
    {
        AR_M[i].resize(2);
        for (int j = 0 ; j < 2 ; j++)
            AR_M[i][j].resize(240,0);
    }

    // this constructor creates three shuffle vectors according to the number of stocks in each group
    int k1 = all[-1].size();
    int k2 = all[0].size();
    int k3 = all[1].size();
    //initialize shuffle vector with length of k1,k2,k3; shuffle1 will be like {0,1,2,3,...,k1-1}, similar for shuffle2 and shuffle3
    for (int i = 0; i < k1; i++)
    { shuffle1.push_back(i); }
    for (int i = 0; i < k2; i++)
    { shuffle2.push_back(i); }
    for (int i = 0; i < k3; i++)
    { shuffle3.push_back(i); }
}

void Bootstrapping::sampling()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();//set random seed
    shuffle(shuffle1.begin(), shuffle1.end(), std::default_random_engine(seed));//shuffle shuffle1
    shuffle(shuffle2.begin(), shuffle2.end(), std::default_random_engine(seed));//shuffle shuffle2
    shuffle(shuffle3.begin(), shuffle3.end(), std::default_random_engine(seed));//shuffle shuffle3
}

void Bootstrapping::AARandCAAR(map<string, stock>& StockMap, map<int, vector<string> >& Map_surprise)
{
    for (int j = 0; j < 5; j++)
    {
        //do it for 5 times
        sampling();
        
        // add first 100 stocks up
        for (int i = 0; i < 100; i++)
        {
            
            double m = shuffle1[i];
            
            m = shuffle1[i];
            AR_M[0][0] = StockMap[Map_surprise[-1][m]].AR + AR_M[0][0];
            
            m = shuffle2[i];
            AR_M[1][0] = StockMap[Map_surprise[0][m]].AR + AR_M[1][0];

            m = shuffle3[i];
            AR_M[2][0] = StockMap[Map_surprise[1][m]].AR + AR_M[2][0];
        }
    }
    double coef = 0.002;
    AR_M[0][0] = coef * AR_M[0][0];
    AR_M[1][0] = coef * AR_M[1][0];
    AR_M[2][0] = coef * AR_M[2][0];
    
    AR_M[0][1][0] = AR_M[0][0][0];
    AR_M[1][1][0] = AR_M[1][0][0];
    AR_M[2][1][0] = AR_M[2][0][0];
    for (int i = 1; i < 240; i++)
    {
        AR_M[0][1][i] = AR_M[0][1][i - 1] + AR_M[0][0][i];
        AR_M[1][1][i] = AR_M[1][1][i - 1] + AR_M[1][0][i];
        AR_M[2][1][i] = AR_M[2][1][i - 1] + AR_M[2][0][i];
    }
}
