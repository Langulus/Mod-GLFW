///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"


///                                                                           
///   GLFW monitor                                                            
///                                                                           
struct Monitor final : A::Platform, ProducedFrom<::Platform> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) ::Platform;
   LANGULUS_BASES(A::Platform);

public:
   Monitor(::Platform*, const Descriptor&);

   void Refresh() final {}
};
