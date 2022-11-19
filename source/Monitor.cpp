///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Monitor.hpp"
#include "Platform.hpp"

/// Monitor construction                                                      
///   @param descriptor - monitor descriptor                                  
Monitor::Monitor(const Any& descriptor)
   : Unit {MetaOf<Monitor>(), descriptor} {

}

/// Move-construct monitor                                                    
///   @param other - the monitor to move                                      
Monitor::Monitor(Monitor&& other) noexcept
   : Unit {Forward<Unit>(other)} {

}

/// Move-copy monitor                                                         
///   @param other - the monitor to move                                      
Monitor& Monitor::operator = (Monitor&& other) noexcept {
   Unit::operator = (Forward<Unit>(other));
   return *this;
}

/// Monitor destruction                                                       
Monitor::~Monitor() {

}
