///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Monitor.hpp"


///                                                                           
///   Native cursor                                                           
///                                                                           
class Cursor : public Unit {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) Platform;

public:
   Cursor(const Any&);
   Cursor(Cursor&&) noexcept;
   ~Cursor();

   Cursor& operator = (Cursor&&) noexcept;

   void Refresh() final {}
};
