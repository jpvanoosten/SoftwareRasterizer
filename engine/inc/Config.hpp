#pragma once

#if defined(_WIN32)
    #define SR_IMPORT __declspec( dllimport )
    #define SR_EXPORT __declspec( dllexport )
#endif

#if defined( SoftwareRasterizer_EXPORTS )
    #define SR_API SR_EXPORT
    #define SR_EXTERN
#elif defined( SoftwareRasterizer_IMPORTS )
    #define SR_API SR_IMPORT
    #define SR_EXTERN extern
#else
    #define SR_API
    #define SR_EXTERN
#endif