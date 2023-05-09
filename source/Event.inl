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

   constexpr EventState::EventState(const Type& state) noexcept
      : mState {state} {}

   constexpr EventState::operator bool() const noexcept {
      return mState != Enum::Point;
   }

   constexpr EventState EventState::operator + (const EventState& state) const noexcept {
      return {mState | state.mState};
   }

   constexpr EventState EventState::operator - (const EventState& state) const noexcept {
      return {mState & (~state.mState)};
   }

   constexpr EventState& EventState::operator += (const EventState& state) noexcept {
      mState |= state.mState;
      return *this;
   }

   constexpr EventState& EventState::operator -= (const EventState& state) noexcept {
      mState &= ~state.mState;
      return *this;
   }

   constexpr bool EventState::operator & (const EventState& state) const noexcept {
      return state == (mState & state.mState);
   }

   constexpr bool EventState::operator % (const EventState& state) const noexcept {
      return 0 == (mState & state.mState);
   }

   constexpr bool EventState::IsPoint() const noexcept {
      return mState == Enum::Point;
   }

   constexpr bool EventState::IsBegin() const noexcept {
      return mState == Enum::Begin;
   }

   constexpr bool EventState::IsEnd() const noexcept {
      return mState == Enum::End;
   }

   constexpr void EventState::Reset() noexcept {
      mState = 0;
   }

   /// Instantiate an event of a specific type                                
   ///   @param type - the type of the event                                  
   ///   @param state - the state of the event                                
   ///   @param payload... - any number of arguments to carry in the event    
   ///   @return the event                                                    
   template<CT::Data... PAYLOAD>
   Event::Event(DMeta type, EventState state, PAYLOAD&&... payload)
      : mType {type}
      , mState {state}
      , mTimestamp {SteadyClock::now()}
      , mPayload {Forward<PAYLOAD>(payload)...} {}

} // namespace Langulus