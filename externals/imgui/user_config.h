#pragma once

#if defined( _WIN32 ) || defined( __CYGWIN__ )
    #define IMGUI_IMPORT __declspec( dllimport )
    #define IMGUI_EXPORT __declspec( dllexport )
#elif defined( __GNUC__ ) && __GNUC__ >= 4
    #define IMGUI_IMPORT __attribute__( ( visibility( "default" ) ) )
    #define IMGUI_EXPORT __attribute__( ( visibility( "default" ) ) )
#else
    #define IMGUI_IMPORT
    #define IMGUI_EXPORT
#endif

#if defined( imgui_EXPORTS )
    #define IMGUI_API IMGUI_EXPORT
#elif defined( imgui_IMPORTS )
    #define IMGUI_API    IMGUI_IMPORT
#else
    #define IMGUI_API
#endif
