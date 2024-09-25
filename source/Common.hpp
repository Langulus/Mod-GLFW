///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Platform.hpp>


namespace GLFW
{
   using namespace Langulus;

   struct Platform;
   struct Cursor;
   struct Monitor;
   struct Window;
}

#if 0
   #define VERBOSE_GLFW(...)     Logger::Verbose(Self(), __VA_ARGS__)
   #define VERBOSE_GLFW_TAB(...) const auto tab = Logger::VerboseTab(Self(), __VA_ARGS__)
#else
   #define VERBOSE_GLFW(...)     LANGULUS(NOOP)
   #define VERBOSE_GLFW_TAB(...) LANGULUS(NOOP)
#endif

/// Include GLFW                                                              
#if LANGULUS_OS(WINDOWS)
   #define GLFW_EXPOSE_NATIVE_WIN32
#elif LANGULUS_OS(LINUX)
   #define GLFW_EXPOSE_NATIVE_X11
#else
   #error Expose GLFW native access here, before the include to glfw
#endif

#include <GLFW/glfw3.h>

