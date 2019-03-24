#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include "curl/curl.h"
#include "Getdata.hpp"
#include "MyDate.h"


using namespace std;

size_t write_data(void *ptr, int size, int nmemb, FILE *stream)
{
    //ptr will point at market data
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    //size is the page's size
    //nmemb is the amounts of page
    
    //if successful, written = size*nmemb, < disk_full
    return written;
}

struct MemoryStruct {
    char *memory;
    size_t size;
};

//Dynamic memory allocation for C
void *myrealloc(void *ptr, size_t size)
{
    /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
    if (ptr)
        return realloc(ptr, size); //allocate
    else
        return malloc(size); //expand
}


size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
    
    mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1); //memory allocation
    if (mem->memory) { //copy data from I/O buffer to memory[mem->size]
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    //copy to the end --> append new data at the end
    return realsize;
}

string getTimeinSeconds(string Time)
{
    std::tm t = {0};
    std::istringstream ssTime(Time); //input string
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S")) //similar to cin, date struct for time
    {
        sprintf (time, "%ld", mktime(&t));
        return string(time);
    }
    else
    {
        cout << "Parse failed\n";
        return "";
    }
}

int getIWB(vector<Date>& mydate, vector<double>& price)//, string& sCrumb, string& sCookies)
{
    string startTime = getTimeinSeconds("2016-6-1T16:00:00");
    string endTime = getTimeinSeconds("2018-3-31T16:00:00");
    vector<string> symbolList;
    symbolList.push_back(string("IWB"));

    vector<string>::iterator itr = symbolList.begin();
    
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    
    //file pointer to create file that store the data
    FILE *fp;
    
    //name of files
    const char outfilename[FILENAME_MAX] = "Output.txt";
    const char resultfilename[FILENAME_MAX] = "Results.txt";
    
    /* declaration of an object CURL */
    CURL *handle;
    
    /* result of the whole process */
    CURLcode result;
    
    /* the first functions */
    /* set up the program environment that libcurl needs */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
    handle = curl_easy_init();
    
    /* if everything's all right with the easy handle... */
    if (handle)
    {
        while (true)
        {
            string sCookies, sCrumb;
            if (sCookies.length() == 0 || sCrumb.length() == 0) //don't have cookie or crumb
            {
                fp = fopen(outfilename, "w");
                //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
                curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history"); //give handle the URL
                
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
                curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
                
                curl_easy_setopt(handle, CURLOPT_ENCODING, "");
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
                //I/O buffer
                //write_data is a function, processing data and save that in file output.txt
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);//fp is the file that we want to write in
                result = curl_easy_perform(handle);
                fclose(fp);
                
                /* Check for errors */
                if (result != CURLE_OK)
                {
                    /* if errors have occurred, tell us what is wrong with 'result'*/
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    return 1;
                }
                
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
                // we first write data in memory in a MemoryStruct data
                
                /* perform, then store the expected code in 'result'*/
                result = curl_easy_perform(handle);
                
                /* Check for errors */
                if (result != CURLE_OK)
                {
                    /* if errors have occured, tell us what is wrong with 'result'*/
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    return 1;
                }
                
                char cKey[] = "CrumbStore\":{\"crumb\":\""; //"crumb" is the key
                char *ptr1 = strstr(data.memory, cKey); //Returns a pointer to the first occurrence of str2 in str1, or a null pointer if str2 is not part of str1.
                char *ptr2 = ptr1 + strlen(cKey);
                char *ptr3 = strstr(ptr2, "\"}");
                if ( ptr3 != NULL )
                    *ptr3 = NULL;
                
                sCrumb = ptr2;
                
                fp = fopen("cookies.txt", "r");
                char cCookies[100];
                if (fp) {
                    while (fscanf(fp, "%s", cCookies) != EOF);
                    fclose(fp);
                }
                else
                    cerr << "cookies.txt does not exists" << endl;
                
                sCookies = cCookies;
                free(data.memory);
                data.memory = NULL;
                data.size= 0;
            }
            
            if (itr == symbolList.end())
                break;
            
            string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
            string symbol = *itr;
            string urlB = "?period1=";
            string urlC = "&period2=";
            string urlD = "&interval=1d&events=history&crumb=";
            string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
            const char * cURL = url.c_str();
            const char * cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
            curl_easy_setopt(handle, CURLOPT_URL, cURL);
            fp = fopen(resultfilename, "ab");
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
            result = curl_easy_perform(handle);
            fclose(fp);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            result = curl_easy_perform(handle);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            stringstream sData;
            sData.str(data.memory); // we treat all data in memory as a string
            string line;
            getline(sData, line);
            //cout << line << endl; //output title
            
            for (; getline(sData, line); )
            {
                //cout << line << endl;
                stringstream sstr(line); //we need to deal with stringstram class object (sstr)
                string token;
                int be=0;
                while(getline(sstr, token, ','))
                {
                    if (be == 5)
                    { price.push_back(stod(token)); }
                    else if (be == 0 )
                    { mydate.push_back(Date(token)); }
                    be += 1;
                }
            }
            
            //for (int ii = 0; ii < mydate.size(); ++ii) cout<<price[ii]<<endl; //test purpose
            
            
            itr++;
        }
        free(data.memory);
        data.size= 0;
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }
    
    /* cleanup since you've used curl_easy_init */
    curl_easy_cleanup(handle);
    
    /* this function releases resources acquired by curl_global_init() */
    curl_global_cleanup();
    
    return 0;
}


int get(map<string, stock>& mystocks)
{
    string startTime = getTimeinSeconds("2016-6-1T16:00:00");
    string endTime = getTimeinSeconds("2018-3-31T16:00:00");
    
    vector<Date> IWB_date;
    vector<double> IWB_price;
    getIWB(IWB_date, IWB_price); //get benchmark data
    map<string, stock>::iterator itr = mystocks.begin();
    
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    
    //file pointer to create file that store the data
    FILE *fp;
    
    //name of files
    const char outfilename[FILENAME_MAX] = "Output.txt";
    //const char resultfilename[FILENAME_MAX] = "Results.txt";
    
    /* declaration of an object CURL */
    CURL *handle;
    
    /* result of the whole process */
    CURLcode result;
    
    /* the first functions */
    /* set up the program environment that libcurl needs */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
    handle = curl_easy_init();
    
    /* if everything's all right with the easy handle... */
    if (handle)
    {
        string sCookies, sCrumb;
        while (true)
        {
            if (sCookies.length() == 0 || sCrumb.length() == 0) //don't have cookie or crumb
            {
                fp = fopen(outfilename, "w");
                //give handle the URL
                curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
                curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
                
                curl_easy_setopt(handle, CURLOPT_ENCODING, "");
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
                //I/O buffer
                //write_data is a function, processing data and save that in file output.txt
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);//fp is the file that we want to write in
                result = curl_easy_perform(handle);
                fclose(fp);
                
                // Check for errors
                if (result != CURLE_OK)
                {
                    // if errors have occurred, tell us what is wrong with 'result'
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    return 1;
                }
                
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
                // we first write data in memory in a MemoryStruct data
                
                // perform, then store the expected code in 'result'
                result = curl_easy_perform(handle);
                
                // Check for errors
                if (result != CURLE_OK)
                {
                    // if errors have occured, tell us what is wrong with 'result'
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    return 1;
                }
                
                char cKey[] = "CrumbStore\":{\"crumb\":\""; //"crumb" is the key
                char *ptr1 = strstr(data.memory, cKey); //Returns a pointer to the first occurrence of str2 in str1, or a null pointer if str2 is not part of str1.
                char *ptr2 = ptr1 + strlen(cKey);
                char *ptr3 = strstr(ptr2, "\"}");
                if ( ptr3 != NULL )
                    *ptr3 = NULL;
                
                sCrumb = ptr2;
                
                fp = fopen("cookies.txt", "r");
                char cCookies[100];
                if (fp) {
                    while (fscanf(fp, "%s", cCookies) != EOF);
                    fclose(fp);
                }
                else
                    cerr << "cookies.txt does not exists" << endl;
                
                sCookies = cCookies;
                free(data.memory);
                data.memory = NULL;
                data.size= 0;
            }
            
            //if (itr == symbolList.end())
            if (itr == mystocks.end() )// || mm > 25)
            {
                cout<<"Retrieve Complete"<<endl;
                break;
            }
            
            free(data.memory);
            data.memory = NULL;
            data.size= 0;
            
            string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
            //string symbol = *itr;
            string symbol = itr->first;
            string urlB = "?period1=";
            string urlC = "&period2=";
            string urlD = "&interval=1d&events=history&crumb=";
            string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
            const char * cURL = url.c_str();
            const char * cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
            curl_easy_setopt(handle, CURLOPT_URL, cURL);
            //fp = fopen(resultfilename, "ab");
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            //curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
            //result = curl_easy_perform(handle);
            //fclose(fp);
            
            /* Check for errors */
            /*
            if (result != CURLE_OK)
            {
                // if errors have occurred, tell us what is wrong with 'result'
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            */
            
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            result = curl_easy_perform(handle);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            /////////////////////////////////////////////////////////////////////////////////
            stringstream sData;
            sData.str(data.memory); // we treat all data in memory as a string
            string line;
            getline(sData, line);
            //cout << line << endl; //output title
            
            vector<double> price;
            for (; getline(sData, line); )
            {
                stringstream sstr(line); //we need to deal with stringstream class object (sstr)
                string token;
                int be=0;
                while(getline(sstr, token, ','))
                {
                    if (be == 5) //the sixth is adjusted price
                    {
                        if (token != "null")
                        {
                            if (stof(token) == 0)
                            {
                                float temp = price.back();
                                price.push_back(temp);
                            }
                            else
                            { price.push_back(stof(token)); }
                        }
                        else
                        {
                            double temp;
                            temp = price.back();
                            price.push_back(temp);
                            //cout<<line<<endl;
                        }
                        
                    }
                    be += 1;
                }
            }
            itr->second.setprice(IWB_date, price, IWB_price);
            
            itr++;
        }
        free(data.memory);
        data.size= 0;
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }
    
    /* cleanup since you've used curl_easy_init */
    curl_easy_cleanup(handle);
    
    /* this function releases resources acquired by curl_global_init() */
    curl_global_cleanup();
    return 0;
}
