///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Cursor.hpp"
#include "Platform.hpp"


namespace GLFW
{

   /// Cursor construction                                                    
   ///   @param producer - cursor owner                                       
   ///   @param descriptor - cursor descriptor                                
   Cursor::Cursor(GLFW::Platform* producer, Describe descriptor)
      : Resolvable   {this}
      , ProducedFrom {producer, descriptor} {
      VERBOSE_GLFW("Initializing...");
      Couple(descriptor);
      VERBOSE_GLFW("Initialized");
   }

} // namespace GLFW
