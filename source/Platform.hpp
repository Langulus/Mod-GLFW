///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Window.hpp"
#include <Flow/Verbs/Create.hpp>


namespace GLFW
{

   ///                                                                        
   ///   Native window/dialog management module                               
   ///                                                                        
   /// Manages and produces native windows, dialogs, file selection browsers, 
   /// color pickers, etc. Uses GLFW library as a backend.                    
   ///                                                                        
   struct Platform final : A::PlatformModule {
      LANGULUS(ABSTRACT) false;
      LANGULUS_BASES(A::PlatformModule);
      LANGULUS_VERBS(Verbs::Create);

   private:
      // List of created windows                                        
      TFactory<GLFW::Window> mWindows;

   public:
      Platform(Runtime*, Describe);
      ~Platform();

      bool Update(Time);
      void Create(Verb&);
   };

} // namespace GLFW