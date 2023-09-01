#ifndef CMAKETEMPLATESDK_HPP
#define CMAKETEMPLATESDK_HPP

// #if defined(DLL_EXPORT)
//     #if defined(_MSC_VER) || defined(__MINGW32__)
//         #define API extern "C" __declspec(dllexport)
//     #elif defined(__GNUC__) || defined(__clang__)
//         #define API extern "C" __attribute__((visibility("default")))
//     #endif
// #else
//     #if defined(_MSC_VER) || defined(__MINGW32__)
//         #define API extern "C" __declspec(dllimport)
//     #endif
// #endif

#ifdef _WIN32
#ifdef DLL_EXPORT
#define LIBRARY_API extern "C" __declspec(dllexport)
#else
#define LIBRARY_API extern "C" __declspec(dllimport)
#endif
#else
#ifdef DLL_EXPORT
#define LIBRARY_API extern "C" __attribute__((visibility ("default")))
#else
#define LIBRARY_API extern "C" 
#endif
#endif


typedef struct 
{
    int type;
    const char* data;  
} DataType;

typedef int (*callback_func_) (int progress, const char* msg);

class CMakeTemplateSDK
{
public:
    virtual int Run(DataType data_type, callback_func_ callback) = 0;
    virtual void Stop() = 0;
};


/**
 * @brief 初始化对象
 * 
 * @param log_path 
 * @return API* 
 */
LIBRARY_API CMakeTemplateSDK* InitCMakeTemplateSDK(const char* log_path);

/**
 * @brief 析构对象
 * 
 * @return API 
 */
LIBRARY_API void DestoryCMakeTemplateSDK(CMakeTemplateSDK* obj);

#endif // CMAKETEMPLATESDK_HPP