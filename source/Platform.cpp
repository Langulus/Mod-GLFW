///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Platform.hpp"

LANGULUS_DEFINE_MODULE(
   GLFW::Platform, 9, "GLFW",
   "Native window/dialog module, using GLFW as backend", "",
   GLFW::Platform, GLFW::Window, GLFW::Cursor, GLFW::Monitor
)


namespace GLFW
{

   /// Error message relay                                                    
   ///   @param error - error id                                              
   ///   @param description - text to display                                 
   void ErrorRelay(int error, const char* description) {
      Logger::Error("GLFW Error code ", error, ": ", description);
   }

   /// Module construction                                                    
   ///   @param runtime - the runtime that owns the module                    
   ///   @param descriptor - instructions for configuring the module          
   Platform::Platform(Runtime* runtime, const Neat&)
      : Resolvable {this}
      , Module {runtime}
      , mWindows {this} {
      VERBOSE_GLFW("Initializing...");

      // Bind our logger first                                          
      glfwSetErrorCallback(ErrorRelay);

      // Initialize GLFW                                                
      if (not glfwInit())
         LANGULUS_THROW(Construct, "Error initializing GLFW");

      VERBOSE_GLFW("Initialized");
   }

   /// Module destruction                                                     
   Platform::~Platform() {
      // Destroy windows first                                          
      mWindows.Reset();
      // Terminate GLFW                                                 
      glfwTerminate();
   }

   /// Module update routine                                                  
   ///   @param dt - time from last update                                    
   bool Platform::Update(Time) {
      // Retrieve and dispatch OS events                                
      glfwPollEvents();

      // Update all opened windows                                      
      auto openedWindows = 0;
      for (auto& window : mWindows) {
         if (window.IsClosed())
            continue;

         window.Update();
         ++openedWindows;
      }

      return openedWindows > 0;
   }

   /// Create/Destroy platform components, such as native windows             
   ///   @param verb - the creation/destruction verb                          
   void Platform::Create(Verb& verb) {
      mWindows.Create(verb);
   }

} // namespace GLFW