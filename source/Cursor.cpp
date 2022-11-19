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
///   @param descriptor - cursor descriptor                                   
Cursor::Cursor(const Any& descriptor)
   : Unit {MetaOf<Cursor>(), descriptor} {

}

/// Move-construct cursor                                                     
///   @param other - the cursor to move                                       
Cursor::Cursor(Cursor&& other) noexcept
   : Unit {Forward<Unit>(other)} {

}

/// Move-copy cursor                                                          
///   @param other - the cursor to move                                       
Cursor& Cursor::operator = (Cursor&& other) noexcept {
   Unit::operator = (Forward<Unit>(other));
   return *this;
}

/// Cursor destruction                                                        
Cursor::~Cursor() {

}
