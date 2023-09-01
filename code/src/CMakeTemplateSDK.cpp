#include "CMakeTemplateSDK.hpp"
#include "CMakeTemplate.hpp"

CMakeTemplateSDK* InitCMakeTemplateSDK(const char* log_path)
{
	return new CMakeTemplate();
}

void DestoryCMakeTemplateSDK(CMakeTemplateSDK* obj)
{
	if (obj)
	{
		delete obj;
	}
}

