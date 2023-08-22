///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <catch2/catch.hpp>

/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Window creation", "[window]") {
   Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         Thing root;
         root.SetName("ROOT");

         // Create runtime at the root                                  
         root.CreateRuntime();

         // Load GLFW module                                            
         root.LoadMod("GLFW");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The window is created via token") {
            auto window = root.CreateUnitToken("Window");

            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE(window.GetCount() == 1);
               REQUIRE(window.IsSparse());
               REQUIRE(window.CastsTo<A::Window>());
            }
         }
      #endif
         
         WHEN("The window is created via abstraction") {
            auto window = root.CreateUnit<A::Window>();

            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE(window.GetCount() == 1);
               REQUIRE(window.IsSparse());
               REQUIRE(window.CastsTo<A::Window>());
            }
         }

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

