///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Window.hpp"

///                                                                           
///   Native window/dialog management module                                  
///                                                                           
/// Manages and produces native windows, dialogs, file selection browsers,    
/// color pickers, etc. Uses GLFW library as a backend.                       
///                                                                           
class Platform : public Module {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(Module);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created windows                                           
   TFactory<Window> mWindows;

   // Number of opened windows                                          
   Count mOpenedWindows {};

public:
   Platform(Runtime*, const Any&);
   ~Platform();

   void Update(Time) final;

   void Create(Verb&);
};

