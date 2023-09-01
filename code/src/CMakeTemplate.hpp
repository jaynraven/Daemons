#ifndef CMAKETEMPLATE_HPP
#define CMAKETEMPLATE_HPP
#include "CMakeTemplateSDK.hpp"

class CMakeTemplate : public CMakeTemplateSDK
{
public:
    CMakeTemplate();
    virtual ~CMakeTemplate();

    virtual int Run(DataType data_type, callback_func_ callback);
    virtual void Stop();
};

#endif // CMAKETEMPLATE_HPP