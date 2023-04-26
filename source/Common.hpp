///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Entity/External.hpp>

using namespace Langulus;
using namespace Langulus::Flow;
using namespace Langulus::Anyness;
using namespace Langulus::Entity;
using namespace Langulus::Math;

/// Include GLFW                                                              
#if LANGULUS_OS(WINDOWS)
   #define GLFW_EXPOSE_NATIVE_WIN32
#elif LANGULUS_OS(LINUX)
   #define GLFW_EXPOSE_NATIVE_X11
#else
   #error Expose GLFW native access here, before the include to glfw
#endif

#include <GLFW/glfw3.h>

struct Platform;
struct Window;
