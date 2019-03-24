#ifndef gnuplot_h
#define gnuplot_h

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace std;

void plotResults_Stock(vector<double>* yData, int dataSize) {
    cout<<"press enter to get the graph"<<endl;
    getchar();
    FILE *gnuplotPipe,*tempDataFile;
    const char *tempDataFileName;
    int i;
    tempDataFileName = "Stock price";
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe,"plot \"%s\" title 'Stock Price' with linespoints\n",tempDataFileName);
        fflush(gnuplotPipe);
        tempDataFile = fopen(tempDataFileName,"w");
        for (i=0; i < dataSize; i++) {
            fprintf(tempDataFile," %d %lf\n",i - (dataSize - 121) ,(*yData)[i]);
        }
        fclose(tempDataFile);
        printf("press enter to continue...");
        getchar();
        remove(tempDataFileName);
        fprintf(gnuplotPipe,"exit \n");
    } else {
        printf("gnuplot not found...");
    }
}

void plotResults_Ret(vector<vector<vector<double> > > *matrix, int a, int b, int dataSize) {
    
    /* a MUST be 0, 1 or 2, b MUST be 0 or 1 */
    cout<<"press enter to get the graph"<<endl;
    getchar();
    FILE *gnuplotPipe,*tempDataFile;
    const char *tempDataFileName;
    int i;
    
    tempDataFileName = "AAR or CAAR result";
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe,"plot \"%s\" title 'AAR or CAAR result' with linespoints\n",tempDataFileName);
        fflush(gnuplotPipe);
        tempDataFile = fopen(tempDataFileName,"w");
        for (i=0; i < dataSize; i++) {
            fprintf(tempDataFile," %d %lf\n",i - (dataSize - 121) ,(*matrix)[a][b][i]);
        }
        fclose(tempDataFile);
        printf("press enter to continue...");
        getchar();
        remove(tempDataFileName);
        fprintf(gnuplotPipe,"exit \n");
    } else {
        printf("gnuplot not found...");
    }
}

void plotCAAR(vector<vector<vector<double> > > *matrix, int dataSize) {
    
    /* a MUST be 0, 1 or 2, b MUST be 0 or 1 */
    cout<<"press enter to get the graph"<<endl;
    getchar();
    int i;
    FILE * temp = fopen("caar", "w");
    
    for(i=0; i < 240; i++)
        fprintf(temp, "%d %lf %lf %lf\n", (i-119) , (*matrix)[0][1][i], (*matrix)[1][1][i], (*matrix)[2][1][i]);
    
    fclose(temp);   // close file if it won't be used anymore
    
    // open interface to gnuplot. Option -p lets plot windows survive after program exits
    FILE * gnuplotPipe = popen("/opt/local/bin/gnuplot","w");
    // send commands to gnuplot one by one.
    fprintf(gnuplotPipe, "%s \n", "set title 'CAAR'");      // define the graph title
    fprintf(gnuplotPipe, "%s \n", "plot 'caar' u 1:2 title 'Miss' w lp");       // plot miss group caar
    // we obtained a plot of Miss group caar
    // then add another two plots to the current graph
    fprintf(gnuplotPipe, "%s \n", "replot 'caar' u 1:3 title 'Meet' w lp");
    fprintf(gnuplotPipe, "%s \n", "replot 'caar' u 1:4 title 'Beat' w lp");
    
    fflush(gnuplotPipe);      // plots the data with the current commands

    printf("press enter to continue...");
    getchar();

    fprintf(gnuplotPipe, "exit \n");
    pclose(gnuplotPipe);
}

#endif /* gnuplot_h */
