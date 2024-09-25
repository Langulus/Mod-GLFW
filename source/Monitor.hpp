///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>


namespace GLFW
{

   ///                                                                        
   ///   GLFW monitor                                                         
   ///                                                                        
   struct Monitor final : A::Platform, ProducedFrom<GLFW::Platform> {
      LANGULUS(ABSTRACT) false;
      LANGULUS(PRODUCER) GLFW::Platform;
      LANGULUS_BASES(A::Platform);

   public:
      Monitor(GLFW::Platform*, const Neat&);

      void Refresh() final {}
   };

} // namespace GLFW