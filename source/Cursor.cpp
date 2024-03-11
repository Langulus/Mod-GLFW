///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Cursor.hpp"
#include "Platform.hpp"


namespace GLFW
{

   /// Cursor construction                                                    
   ///   @param producer - cursor owner                                       
   ///   @param descriptor - cursor descriptor                                
   Cursor::Cursor(GLFW::Platform* producer, const Neat& descriptor)
      : A::Platform {MetaOf<Cursor>()}
      , ProducedFrom {producer, descriptor} {
      VERBOSE_GLFW("Initializing...");
      Couple(descriptor);
      VERBOSE_GLFW("Initialized");
   }

} // namespace GLFW
