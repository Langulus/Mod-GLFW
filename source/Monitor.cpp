///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Monitor.hpp"
#include "Platform.hpp"


namespace GLFW
{

   /// Monitor construction                                                   
   ///   @param producer - monitor owner                                      
   ///   @param descriptor - monitor descriptor                               
   Monitor::Monitor(GLFW::Platform* producer, const Neat& descriptor)
      : A::Platform {MetaOf<GLFW::Monitor>(), descriptor}
      , ProducedFrom {producer, descriptor} { }

}