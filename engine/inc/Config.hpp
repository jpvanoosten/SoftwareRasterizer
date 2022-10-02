#pragma once

#if defined(_WIN32)
    #define RS_ENGINE_IMPORT __declspec( dllimport )
    #define RS_ENGINE_EXPORT __declspec( dllexport )
#endif

#if defined( Engine_EXPORTS )
    #define ENGINE_API RS_ENGINE_EXPORT
    #define ENGINE_EXTERN
#elif defined( Engine_IMPORTS )
    #define ENGINE_API RS_ENGINE_IMPORT
    #define ENGINE_EXTERN extern
#else
    #define ENGINE_API
    #define ENGINE_EXTERN
#endif