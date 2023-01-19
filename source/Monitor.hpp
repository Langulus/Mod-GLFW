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
///   Native monitor                                                          
///                                                                           
class Monitor : public Unit {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) Platform;
public:
   Monitor(const Any&);
   Monitor(Monitor&&) noexcept;
   ~Monitor();

   Monitor& operator = (Monitor&&) noexcept;

   void Refresh() final {}
};
