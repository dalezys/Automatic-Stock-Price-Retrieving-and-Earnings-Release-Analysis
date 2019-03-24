#ifndef Bootstrapping_h
#define Bootstrapping_h

#include <map>
#include <string>
#include <vector>
#include "Stocks.h"
using namespace std;

class Bootstrapping
{
public:
    vector<vector<vector<double> > > AR_M;
    vector <int> shuffle1;// creat a vector for shuffle of miss group
    vector <int> shuffle2;// creat a vector for shuffle of meet group
    vector <int> shuffle3;// creat a vector for shuffle of beat group
    
    Bootstrapping(map<int, vector<string>>& all);
    void sampling(); //create a fuction to shuffle the shuffle1, shuffle2, shuffle3
    void AARandCAAR(map<string, stock>& StockMap, map<int, vector<string> >& Map_surprise);
    
};

#endif /* Bootstrapping_h */
