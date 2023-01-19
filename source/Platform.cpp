///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Platform.hpp"

LANGULUS_DEFINE_MODULE(
   Platform, 9, "GLFW",
   "Native window/dialog module, using GLFW as backend", "",
   Platform, Window, Cursor, Monitor
)

/// Error message relay                                                       
///   @param error - error id                                                 
///   @param description - text to display                                    
void ErrorRelay(int error, const char* description) {
   Logger::Error("GLFW Error code ", error, ": ", description);
}

/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
Platform::Platform(Runtime* runtime, const Any&)
   : Module {MetaOf<Platform>(), runtime}
   , mWindows {this} {
   Logger::Verbose(Self(), "Initializing...");

   // Bind our logger first                                             
   glfwSetErrorCallback(ErrorRelay);

   // Initialize GLFW                                                   
   if (!glfwInit())
      LANGULUS_THROW(Construct, "Error initializing GLFW");

   Logger::Verbose(Self(), "Initialized");
}

/// Module destruction                                                        
Platform::~Platform() {
   glfwTerminate();
}

/// Module update routine                                                     
///   @param dt - time from last update                                       
void Platform::Update(Time) {
   // Retrieve and dispatch OS events                                   
   glfwPollEvents();

   // Update all opened windows                                         
   mOpenedWindows = 0;
   for (auto& window : mWindows) {
      if (window.IsClosed())
         continue;

      window.Update();
      ++mOpenedWindows;
   }
}

/// Create/Destroy platform components, such as native windows                
///   @param verb - the creation/destruction verb                             
void Platform::Create(Verb& verb) {
   mWindows.Create(verb);
}