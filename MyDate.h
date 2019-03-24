#ifndef Mydate_h
#define Mydate_h

#include <string>
#include <sstream>
using namespace std;

class Date {
private:
    int year, month, day;
    
public:
    //constructors and desturctor
    Date() {}//default constructor
    Date(string& str_, double a)
    {
            stringstream temp(str_);
            string token;
            vector<int> mydate;
            while(getline(temp, token, '/'))
            { mydate.push_back(stoi(token)); }
            month = mydate[0]; day = mydate[1]; year = 2000+mydate[2];
    }
    Date(string& str_)
    {
            stringstream temp(str_);
            string token;
            vector<int> mydate;
            while(getline(temp, token, '-'))
            { mydate.push_back(stoi(token)); }
            year = mydate[0]; month = mydate[1]; day = mydate[2];
    }
    Date(int year_, int month_, int day_) :year(year_), month(month_), day(day_){}
    Date(const Date& date) //copy constructor
    { year = date.year; month = date.month; day = date.day; }
    ~Date() {}
    
    
    string GetDate() //get a string of the date
    {
        return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    }
    bool operator==(const Date& obj) const
    { return (( this->year == obj.year ) && ( this->month == obj.month ) && (this->day == obj.day)); }

};

#endif /* Mydate_h */
