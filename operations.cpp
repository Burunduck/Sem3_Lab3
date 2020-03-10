#include "operations.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int countOfSubstrInStr(string substr, string str)
{
    int count = 0;
    int n = 0;
    while ((n = str.find(substr, n) + 1) != 0)
    {
        count++;
    }
    return count;
}

string* split(string delim, string str)
{
    int len = 0;
    int from = 0;
    int to = 0;
    string* outArr = new string [len = countOfSubstrInStr(delim, str) + 1];
    for(int i = 0; i < len; i ++)
    {
        to = str.find(delim, from);
        outArr[i] = str.substr(from, to - from);
        from = to + 1;
    }

   return outArr;
}

resultData checkFile(sourceData *source)
{
    ifstream inFileStream;
    resultData result;
    inFileStream.open(source->path);
    if(inFileStream.is_open())
    {
        result.isOpen = true;
    }
    else
    {
        result.isOpen = false;
    }
    return result;
}

string** giveMemory(int countOfLines, int countOfColumns)
{
    string** tableInArrayOfString  = new string* [countOfLines];
    for(int i = 0; i < countOfLines; i++)
    {
        tableInArrayOfString[i] = new string [countOfColumns];
    }
    return tableInArrayOfString;
}

TableData loadTableData(string tableDataInString, int countOfLines, int countOfColumns)
{
    string** tableData = giveMemory(countOfLines, countOfColumns);
    string* lines = split("\n", tableDataInString);
    for(int i = 0; i < countOfLines; i++)
    {
        string* currentArrayOfWords = split(",", lines[i]);
        for(int j = 0; j < countOfColumns; j++)
        {
            tableData[i][j] = currentArrayOfWords[j];
        }
    }
    TableData result;
    result.tableInMatrix = tableData;
    result.countOfLines = countOfLines;
    result.countOfColumns = countOfColumns;
    return result;
}



resultData getTableByRegion(sourceData *source)
{
    resultData result;
    ifstream inFileStream;
    string currentLine;
    string tableDataInstring = "";
    inFileStream.open(source->path);
    result = checkFile(source);
    if(result.isOpen)
    {
        getline(inFileStream, currentLine);
        string* arrLine = split(",", currentLine);
        int countOfColumns = countOfSubstrInStr(",", currentLine) + 1;
        tableDataInstring += currentLine + "\n";
        int countOfLines = 1;
        while (!inFileStream.eof())
        {
            getline(inFileStream, currentLine);
            arrLine = split(",", currentLine);
            if(arrLine[1] == source->region)
            {
                tableDataInstring += currentLine + "\n";
                countOfLines++;
            }
        }
        result.tableDataByRegion = loadTableData(tableDataInstring, countOfLines, countOfColumns);
    }
    else
    {
        cout << "Error with opening file.csv" << endl;
    }

    return result;
}

resultData calcMetrics(sourceData *source)
{
    resultData result = getTableByRegion(source);
    double sum = 0;
    double max = atof(result.tableDataByRegion.tableInMatrix[1][source->column].c_str());
    int counter = 0;
    double min = atof(result.tableDataByRegion.tableInMatrix[1][source->column].c_str());
    vector<double> vectorForMedian;
    for(int i = 1; i < result.tableDataByRegion.countOfLines; i++)
    {
        sum +=  atof(result.tableDataByRegion.tableInMatrix[i][source->column].c_str());
        counter++;
        if(atof(result.tableDataByRegion.tableInMatrix[i][source->column].c_str()) < min)
        {
            min = atof(result.tableDataByRegion.tableInMatrix[i][source->column].c_str());
        }
        if(atof(result.tableDataByRegion.tableInMatrix[i][source->column].c_str()) > max)
        {
            max = atof(result.tableDataByRegion.tableInMatrix[i][source->column].c_str());
        }
        vectorForMedian.push_back(atof(result.tableDataByRegion.tableInMatrix[i][source->column].c_str()));
    }

    result.metrics.average = sum / (counter);
    result.metrics.min = min;
    result.metrics.max = max;
    sort(vectorForMedian.begin(), vectorForMedian.end());
    if((counter % 2) ==0)
    {
        result.metrics.median = vectorForMedian[counter/2];
    }
    else
    {
        result.metrics.median = (vectorForMedian[counter/2] + vectorForMedian[counter/2 - 1]) / 2.0;
    }

    return  result;
}

