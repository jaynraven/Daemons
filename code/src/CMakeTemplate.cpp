#include "CMakeTemplate.hpp"
#include "stdio.h"

CMakeTemplate::CMakeTemplate()
{

}

CMakeTemplate::~CMakeTemplate()
{

}

int CMakeTemplate::Run(DataType data_type, callback_func_ callback)
{
    printf(data_type.data);
    return 0;
}

void CMakeTemplate::Stop()
{

}