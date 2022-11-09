#pragma once
#include "Window.hpp"

///                                                                           
///   Native window/dialog management module                                  
///                                                                           
///   Manages and produces windows, dialogs, file selection browsers,         
/// color pickers, etc. Uses GLFW library as a backend.                       
///                                                                           
class Platform : public Module {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(Module);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created windows												         
   TFactory<Window> mWindows;

public:
   Platform(Runtime*, const Any&);
   ~Platform();

   virtual void Update(Time) final;

   void Create(Verb&);
};

LANGULUS_DEFINE_MODULE(Platform, 9, "Platform"
   , "Native window/dialog module, implemented via GLFW", ""
   , Platform, Window
)