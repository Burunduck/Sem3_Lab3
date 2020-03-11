#include "buisnesslogic.h"
#include "operations.h"


resultData command(operationType operationType, sourceData* source)
{
    resultData result;
    if(operationType == getTable)
    {
        result = getTableByRegion(source);
    }
    else if(operationType == calculateMetrics)
    {
        result = calcMetrics(source);
    }

    return result;
}
