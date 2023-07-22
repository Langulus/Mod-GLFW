///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <catch2/catch.hpp>

#if LANGULUS_FEATURE(MEMORY_STATISTICS)
static bool statistics_provided = false;
static Allocator::Statistics memory_statistics;
#endif

/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Window creation", "[window]") {
   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         Thing root;
         root.AddTrait(Traits::Name {"ROOT"_text});

         // Create runtime at the root                                  
         root.CreateRuntime();

         // Load GLFW module                                            
         root.LoadMod("GLFW");

         WHEN("The window is created") {
            auto window = root.CreateUnitToken("Window");

            for (int repeat2 = 0; repeat2 != 10; ++repeat2)
               root.Update(Time::zero());

            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE_FALSE(!window);
               REQUIRE(window.IsSparse());
               REQUIRE(window.CastsTo<A::Window>());
            }
         }
         
         #if LANGULUS_FEATURE(MEMORY_STATISTICS)
            // Detect memory leaks                                      
            if (statistics_provided) {
               if (memory_statistics != Fractalloc.GetStatistics()) {
                  memory_statistics = Fractalloc.GetStatistics();
                  Fractalloc.DumpPools();
                  FAIL("Memory leak detected");
               }
            }

            memory_statistics = Fractalloc.GetStatistics();
            statistics_provided = true;
         #endif
      }
   }
}

