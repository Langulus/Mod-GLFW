///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <Langulus/Platform.hpp>
#include <catch2/catch.hpp>


/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Window creation", "[window]") {
   static Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         Thing root;
         root.SetName("ROOT");
         root.CreateRuntime();
         root.LoadMod("GLFW");
         
         WHEN("The window is created via abstraction") {
            auto window = root.CreateUnit<A::Window>();
            root.DumpHierarchy();

            REQUIRE(window.GetCount() == 1);
            REQUIRE(window.IsSparse());
            REQUIRE(window.CastsTo<A::Window>());
            REQUIRE(root.GetUnits().GetCount() == 1);
         }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The window is created via token") {
            auto window = root.CreateUnitToken("A::Window");
            root.DumpHierarchy();

            REQUIRE(window.GetCount() == 1);
            REQUIRE(window.IsSparse());
            REQUIRE(window.CastsTo<A::Window>());
            REQUIRE(root.GetUnits().GetCount() == 1);
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

