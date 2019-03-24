#ifndef ReadData_h
#define ReadData_h

#include<iostream>
#include<fstream>
#include<map>
#include<sstream>
#include<string>
#include "Stocks.h"
#include "MyDate.h"

using namespace std;

int ReadData(map<int, vector<string> > &Map_surprise, map<string, stock> &StockMap, double CR_d, double CR_u)
{
    ifstream infile("data.csv");
    if (infile.is_open())
    {
        string line;
        getline(infile, line);
        while (getline(infile, line))
        {
            string Ticker, Anntime, ActEps, EstEps;
            istringstream lines(line);
            getline(lines, Ticker, ',');
            getline(lines, Anntime, ',');
            getline(lines, ActEps, ',');
            getline(lines, EstEps, '\n');
            double act = stod(ActEps);
            double est = stod(EstEps);
            double surprise = (act - est) / est;
            
            if (surprise > CR_u)
            {
                Map_surprise[1].push_back(Ticker);
                StockMap[Ticker] = stock(Ticker, Date(Anntime,1), act, est, 1);
            }
            else if (surprise < CR_d)
            {
                Map_surprise[-1].push_back(Ticker);
                StockMap[Ticker] = stock(Ticker, Date(Anntime,1), act, est, -1);
            }
            else
            {
                Map_surprise[0].push_back(Ticker);
                StockMap[Ticker] = stock(Ticker, Date(Anntime,1), act, est, 0);
            }
        }
    }
    else {return 1;}
    infile.close();
    
    return 0;
}
    
#endif // !ReadData_h
#pragma once
