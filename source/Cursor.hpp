///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Monitor.hpp"


///                                                                           
///   GLFW cursor                                                             
///                                                                           
struct Cursor final : A::Platform, ProducedFrom<::Platform> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) ::Platform;
   LANGULUS_BASES(A::Platform);

public:
   Cursor(::Platform*, const Neat&);

   void Refresh() final {}
};
