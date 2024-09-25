///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright (c) 2015 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Main.hpp"
#include <Flow/Verbs/Interact.hpp>
#include "../source/Event.inl"
#include <catch2/catch.hpp>


/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Events", "[event]") {
   static_assert(sizeof(Event) == sizeof(Events::WindowClose), "Size mismatch");
   static_assert(sizeof(Event) == sizeof(Keys::A), "Size mismatch");
   static_assert(CT::Event<Event>, "Event isn't considered CT::Event");
   static_assert(CT::Event<Events::WindowClose>, "Events::WindowClose isn't considered CT::Event");
   static_assert(CT::Event<Keys::A>, "Keys::A isn't considered CT::Event");

   Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and dispatch an event #") + std::to_string(repeat)) {
         //TODO also handle it 
         Thing root;
         Verbs::Interact interact {
            Events::WindowFocus {nullptr}
         };

         WHEN("The event is posted") {
            root.Do(interact);

            THEN("Various traits change") {
               REQUIRE(true);
            }
         }

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

