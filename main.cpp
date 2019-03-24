#include <stdio.h>
#include<fstream>
#include<sstream>
#include "Getdata.hpp"
#include "MyDate.h"
#include "Stocks.h"
#include "ReadData.h"
#include "Bootstrapping.h"
#include "Mygnuplot.h"

using namespace std;

void Menu(int &option)
{
    cout<<"Enter 1 to retrieve data from yahoo"<<endl;
    cout<<"Enter 2 to get a stock's information"<<endl;
    cout<<"Enter 3 to plot CAAR for 3 groups"<<endl;
    cout<<"Enter 4 to plot AAR or CAAR for one Group"<<endl;
    cout<<"Enter 5 to Exit"<<endl;
    cin>>option;
}

void Pre()
{
    cout<<"\n-------------Retrieving Data-------------"<<endl;
    cout<<"--------------Just a Minute--------------"<<endl;
}

int main()
{
    map<string, stock> mystocks;
    map<int, vector<string> > Map_eps;
    bool run = false;

    int Option;
    switchStart:
    Menu(Option);
        switch(Option)
        {
            case 1:
            {
                if (run)
                { cout<<"you have already retrived data from yahoo~\n"<<endl; }
                else
                {
                    Pre();
                    if (ReadData(Map_eps, mystocks, 0.008, 0.06) == 1){
                        cout<<"error in loading data"<<endl;
                        cout<<"this project is based on an absolute directory"<<endl;
                        return 1;
                    }
                
                    //Retrieve Data from Yahoo Finance
                    get(mystocks);
                    run = true;
                }
                goto switchStart;
            }
            case 2:
            {
                if (!run)
                { cout<<"You should retrieve data first!!\n"<<endl; }
                else
                {
                    string Ticker;
                    cout<<"Enter the ticker of the stock, it should be capitalized: \n";
                    cin>>Ticker;
                    map<string,stock>::iterator itr;
                    itr = mystocks.find(Ticker);
                    if(itr==mystocks.end())
                    {
                        cout<<"We do not find this Ticker in database\n"<<endl;
                        goto switchStart;
                    }
                    cout<<"\nName: "<<Ticker<<endl;
                    cout<<"Label: "<<(itr->second).GetLabel()<<endl;
                    cout<<"Announcement Date: "<<(itr->second).epsdate()<<endl;
                    cout<<"Bloomberg Estimate: "<<(itr->second).est<<endl;
                    cout<<"Actual: "<<(itr->second).act<<"\n"<<endl;
                    plotResults_Stock(&(mystocks[Ticker].price), 241);
                }
                goto switchStart;
            }
            case 3:
            {
                if (!run)
                { cout<<"you should retrieve data first!!\n"<<endl; }
                else
                {
                    Bootstrapping a(Map_eps);
                    a.AARandCAAR(mystocks, Map_eps);
                    plotCAAR(&(a.AR_M), 240);
                }
                goto switchStart;
            }
            case 4:
            {
                if (!run)
                { cout<<"you should retrieve data first!!\n"<<endl; }
                else{
                    string fst, snd;
                    int i, j;
                    cout<<"Enter miss, meet or beat, do not capitalize the word: ";
                    cin>>fst;
                    if (fst == "miss") i = 0;
                    else if (fst == "meet") i = 1;
                    else if (fst == "beat") i = 2;
                    else {cout<<"error in input"<<endl; goto switchStart;}
                    cout<<"\nEnter aar or caar to show this group, do not capitalize the word: ";
                    cin>>snd;
                    if (snd == "aar") j = 0;
                    else if (snd == "caar") j = 1;
                    else {cout<<"error in input"<<endl; goto switchStart;}
                    
                    Bootstrapping a(Map_eps);
                    a.AARandCAAR(mystocks, Map_eps);
                    plotResults_Ret(&(a.AR_M), i, j, 240);
                }
                goto switchStart;
            }
            case 5:
            { break; }
        }
    return 0;
}
