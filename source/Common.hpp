///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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

#define VERBOSE_GLFW(...)     Logger::Verbose(Self(), __VA_ARGS__)
#define VERBOSE_GLFW_TAB(...) const auto tab = Logger::Verbose(Self(), __VA_ARGS__, Logger::Tabs {})

/// Include GLFW                                                              
#if LANGULUS_OS(WINDOWS)
   #define GLFW_EXPOSE_NATIVE_WIN32
#elif LANGULUS_OS(LINUX)
   #define GLFW_EXPOSE_NATIVE_X11
#else
   #error Expose GLFW native access here, before the include to glfw
#endif

#include <GLFW/glfw3.h>

