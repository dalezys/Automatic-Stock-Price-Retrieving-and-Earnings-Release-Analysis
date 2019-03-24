#ifndef Getdata_hpp
#define Getdata_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <map>
#include "curl/curl.h"
#include "Stocks.h"
#include "MyDate.h"


using namespace std;

int get(map<string, stock>& mystocks);//void);
string getTimeinSeconds(string Time);
size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data);
void *myrealloc(void *ptr, size_t size);
size_t write_data(void *ptr, int size, int nmemb, FILE *stream);

#endif /* Getdata_hpp */
