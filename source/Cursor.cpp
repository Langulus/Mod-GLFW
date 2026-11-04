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
   ///   @param producer cursor owner                                         
   ///   @param descriptor cursor descriptor                                  
   Cursor::Cursor(GLFW::Platform const* producer, Neat const& descriptor)
      : Downcastable {this}
      , ProducedFrom {producer, descriptor} {
      VERBOSE_GLFW("Initializing...");
      Couple(descriptor);
      VERBOSE_GLFW("Initialized");
   }
}
