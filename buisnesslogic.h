#ifndef BUISNESSLOGIC_H
#define BUISNESSLOGIC_H
#include <string>
using namespace std;

struct TableData
{
    string** tableInMatrix;
    int countOfLines;
    int countOfColumns;
};

struct Metrics
{
    double min;
    double average;
    double median;
};

enum operationType
{
    getTable,
    calculateMetrics,
};

struct sourceData
{
    string path;
    string region;
    int column;
};

struct resultData
{
    Metrics metrics;
    TableData tableDataByRegion;
    bool isOpen;
};

resultData command(operationType operationType, sourceData* source);

#endif//BUISNESSLOGIC_H
