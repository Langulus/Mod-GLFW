///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Event.hpp"


namespace Langulus
{

   /// Construct from internal type                                           
   ///   @param state - the state to set                                      
   LANGULUS(INLINED)
   constexpr EventState::EventState(const Type& state) noexcept
      : mState {state} {}

   /// Check if the event isn't default (isn't immediate)                     
   LANGULUS(INLINED)
   constexpr EventState::operator bool() const noexcept {
      return mState != Enum::Point;
   }

   /// Operator for combining states (nondestructively)                       
   ///   @param state - the state combination to add                          
   ///   @return the combined state                                           
   LANGULUS(INLINED)
   constexpr EventState EventState::operator + (const EventState& state) const noexcept {
      return {mState | state.mState};
   }

   /// Operator for excluding states (nondestructively)                       
   ///   @param state - the state combination to remove                       
   ///   @return the combined state                                           
   LANGULUS(INLINED)
   constexpr EventState EventState::operator - (const EventState& state) const noexcept {
      return {mState & (~state.mState)};
   }

   /// Operator for adding (destructively) states to this one                 
   ///   @param state - the state combination to add                          
   ///   @return a reference to this state                                    
   LANGULUS(INLINED)
   constexpr EventState& EventState::operator += (const EventState& state) noexcept {
      mState |= state.mState;
      return *this;
   }

   /// Operator for removing (destructively) states from this one             
   ///   @param state - the state combination to remove                       
   ///   @return a reference to this state                                    
   LANGULUS(INLINED)
   constexpr EventState& EventState::operator -= (const EventState& state) noexcept {
      mState &= ~state.mState;
      return *this;
   }

   /// Operator for checking if a combination of states is enabled            
   ///   @param state - the state combination to check                        
   ///   @return true if all the provided states are enabled                  
   LANGULUS(INLINED)
   constexpr bool EventState::operator & (const EventState& state) const noexcept {
      return state == (mState & state.mState);
   }

   /// Operator for checking if a combination of states is disabled           
   ///   @param state - the state combination to check                        
   ///   @return true if all the provided states are disabled                 
   LANGULUS(INLINED)
   constexpr bool EventState::operator % (const EventState& state) const noexcept {
      return 0 == (mState & state.mState);
   }

   /// Is this event immediate (has no beginning and end)                     
   LANGULUS(INLINED)
   constexpr bool EventState::IsPoint() const noexcept {
      return mState == Enum::Point;
   }

   /// Is this the beginning of an event?                                     
   LANGULUS(INLINED)
   constexpr bool EventState::IsBegin() const noexcept {
      return mState == Enum::Begin;
   }

   /// Is this the end of a started event?                                    
   LANGULUS(INLINED)
   constexpr bool EventState::IsEnd() const noexcept {
      return mState == Enum::End;
   }

   /// Reset the state                                                        
   LANGULUS(INLINED)
   constexpr void EventState::Reset() noexcept {
      mState = 0;
   }


   /// Default event construction creates a timestamp                         
   LANGULUS(INLINED)
   Event::Event()
      : mTimestamp {SteadyClock::Now()} {}

   /// Copy-construction creates a new timestamp                              
   ///   @attention creates a new timestamp                                   
   ///   @param other - event properties and payload to shallow-copy          
   LANGULUS(INLINED)
   Event::Event(const Event& other)
      : mType {other.mType}
      , mState {other.mState}
      , mTimestamp {SteadyClock::Now()}
      , mPayload {other.mPayload} {}

   /// Move-construction                                                      
   ///   @param other - event properties and payload to move                  
   LANGULUS(INLINED)
   Event::Event(Event&& other)
      : mType {other.mType}
      , mState {other.mState}
      , mTimestamp {other.mTimestamp}
      , mPayload {::std::move(other.mPayload)} {}

   /// Instantiate an event of a specific type, manually                      
   /// This constructor also generates the timestamp                          
   ///   @param a... - any number of arguments to carry in the event          
   template<class... T> LANGULUS(INLINED)
   Event::Event(T&&...a) requires (::std::constructible_from<Any, T&&...>)
      : Event {} {
      if constexpr (CT::Semantic<T...>) {
         using S = FirstOf<T...>;
         using ST = TypeOf<S>;

         if constexpr (CT::Event<ST> and sizeof...(T) == 1) {
            // Semantic-construction from Event                         
            ((mType = a->mType), ...);
            ((mState = a->mState), ...);
            if constexpr (S::Move)
               ((mTimestamp = a->mTimestamp), ...);
            ((mPayload = S::Nest(a->mPayload)), ...);
         }
         else {
            // Semantic-construction from anything else - just forward  
            // it to the payload                                        
            mPayload = Any {Forward<T>(a)...};
         }
      }
      else {
         // Whatever arguments are there, forward them to payload       
         mPayload = Any {Forward<T>(a)...};
      }
   }

} // namespace Langulus