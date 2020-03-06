#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "buisnesslogic.h"
resultData getTableByRegion (sourceData *source);
resultData calcMetrics(sourceData *source);
int countOfSubstrInStr(string substr, string str);
string* split(string delim, string str);
resultData checkFile(sourceData *source);
string** giveMemory(int countOfLines, int countOfColumns);
TableData loadTableData(string tableDataInString, int countOfLines, int countOfColumns);

#endif // OPERATIONS_H
