///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Cursor.hpp"
#include "Platform.hpp"


/// Cursor construction                                                       
///   @param producer - cursor owner                                          
///   @param descriptor - cursor descriptor                                   
Cursor::Cursor(::Platform* producer, const Neat& descriptor)
   : A::Platform {MetaOf<Cursor>(), descriptor}
   , ProducedFrom {producer, descriptor} { }
