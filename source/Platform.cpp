#include "Platform.hpp"

/// Error message relay                                                       
///   @param error - error id                                                 
///   @param description - text to display                                    
inline void GLFWErrorRelay(int error, const char* description) {
   Logger::Error() << "GLFW Error #" << error << ": " << description;
}

namespace PCFW
{

   constexpr DataID ukInvalid = udAny;

   ///                                                                        
   ///   Converting from GLFW specific constants, to Langulus equivalents     
   ///                                                                        
   constexpr DataID GLFWKeyboardTable[GLFW_KEY_LAST + 1] = {
      // 0 - 31 - unused																
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      Keys::Space::ID,

      // 33 - 38 - unused																
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      Keys::Quote::ID,			//39

      // 40 - 43 - unused																
      ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      Keys::Comma::ID,
      Keys::Minus::ID,
      Keys::Period::ID,
      Keys::Slash::ID,
      Keys::Key0::ID, Keys::Key1::ID, Keys::Key2::ID, Keys::Key3::ID, Keys::Key4::ID,
      Keys::Key5::ID, Keys::Key6::ID, Keys::Key7::ID, Keys::Key8::ID, Keys::Key9::ID,
      ukInvalid,		//58
      Keys::Semicolon::ID,
      ukInvalid,		//60
      Keys::NumpadEqual::ID, //?

      // 62 - 64 - unused																
      ukInvalid, ukInvalid, ukInvalid,
      Keys::A::ID, Keys::B::ID, Keys::C::ID, Keys::D::ID, Keys::E::ID,
      Keys::F::ID, Keys::G::ID, Keys::H::ID, Keys::I::ID, Keys::J::ID,
      Keys::K::ID, Keys::L::ID, Keys::M::ID, Keys::N::ID, Keys::O::ID,
      Keys::P::ID, Keys::Q::ID, Keys::R::ID, Keys::S::ID, Keys::T::ID,
      Keys::U::ID, Keys::V::ID, Keys::W::ID, Keys::X::ID, Keys::Y::ID, Keys::Z::ID,
      Keys::LeftBracket::ID, Keys::Hack::ID, Keys::RightBracket::ID,

      // 94 - 95 - unused																
      ukInvalid, ukInvalid,
      Keys::Tilde::ID,

      // 97 - 255 - unused																
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,

      Keys::Escape::ID, Keys::Enter::ID, Keys::Tab::ID, Keys::Back::ID, Keys::Insert::ID, Keys::Delete::ID, Keys::Right::ID,
      Keys::Left::ID, Keys::Down::ID, Keys::Up::ID, Keys::PageUp::ID, Keys::PageDown::ID, Keys::Home::ID, Keys::End::ID,

      // 270 - 279 - unused															
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      Keys::CapsLock::ID, Keys::ScrollLock::ID, Keys::NumLock::ID, Keys::Print::ID, Keys::Pause::ID,

      // 285 - 289 - unused															
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      Keys::F1::ID, Keys::F2::ID, Keys::F3::ID, Keys::F4::ID, Keys::F5::ID, Keys::F6::ID,
      Keys::F7::ID, Keys::F8::ID, Keys::F9::ID, Keys::F10::ID, Keys::F11::ID, Keys::F12::ID, Keys::F13::ID,
      Keys::F14::ID, Keys::F15::ID, Keys::F16::ID, Keys::F17::ID, Keys::F18::ID, Keys::F19::ID,
      Keys::F20::ID, Keys::F21::ID, Keys::F22::ID, Keys::F23::ID, Keys::F24::ID,
      ukInvalid, // 314 (F25) ? 

      // 315 - 319 - unused															
      ukInvalid, ukInvalid, ukInvalid, ukInvalid, ukInvalid,
      Keys::Num0::ID, Keys::Num1::ID, Keys::Num2::ID, Keys::Num3::ID, Keys::Num4::ID,
      Keys::Num5::ID, Keys::Num6::ID, Keys::Num7::ID, Keys::Num8::ID, Keys::Num9::ID,

      Keys::NumDecimal::ID, Keys::NumDivide::ID, Keys::NumMultiply::ID, Keys::NumSubtract::ID,
      Keys::NumAdd::ID, Keys::Enter::ID, Keys::NumpadEqual::ID,

      // 337 - 339 - unused															
      ukInvalid, ukInvalid, ukInvalid,
      Keys::LShift::ID, Keys::LControl::ID, Keys::LAlt::ID,

      ukInvalid,	// GLFW_KEY_LEFT_SUPER 343 ?
      Keys::RShift::ID, Keys::RControl::ID, Keys::RAlt::ID,

      ukInvalid,	// GLFW_KEY_RIGHT_SUPER 347 ?
      ukInvalid,	// GLFW_KEY_MENU 348
   };


   ///																								
   /// A TABLE FOR CONVERSION FROM GLFW MOUSE EVENTS TO PC STANDARD EVENTS		
   ///																								
   constexpr DataID GLFWMouseTable[8] = {
      Keys::MouseLeft::ID, Keys::MouseRight::ID, Keys::MouseMiddle::ID,
      Keys::Mouse4::ID, Keys::Mouse5::ID, Keys::Mouse6::ID, Keys::Mouse7::ID, Keys::Mouse8::ID
   };


   ///																								
   /// A TABLE FOR CONVERSION FROM GLFW JOYSTICK EVENTS TO PC STANDARD EVENTS	
   ///																								
   constexpr DataID GLFWJoystickTable[16] = {
      Keys::Joystick1::ID, Keys::Joystick2::ID, Keys::Joystick3::ID, Keys::Joystick4::ID,
      Keys::Joystick5::ID, Keys::Joystick6::ID, Keys::Joystick7::ID, Keys::Joystick8::ID,
      Keys::Joystick9::ID, Keys::Joystick10::ID, Keys::Joystick11::ID, Keys::Joystick12::ID,
      Keys::Joystick13::ID, Keys::Joystick14::ID, Keys::Joystick15::ID, Keys::Joystick16::ID
   };

} // namespace PCFW


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
Platform::Platform(Runtime* runtime, const Any& descriptor)
   : Module {MetaOf<Platform>(), runtime}
   , mWindows {this} {
   pcLogSelfVerbose << "Initializing...";

   // Initialize GLFW                                                   
   if (!glfwInit())
      throw Except::Platform(pcLogSelfError << "Error initializing GLFW");

   // Bind our logger                                                   
   glfwSetErrorCallback(GLFWErrorRelay);

   pcLogSelfVerbose << "Initialized";
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

   // Update all enabled windows                                        
   for (auto& window : mWindows) {
      if (window.IsClassEnabled())
         window.Update();
   }
}

/// Get the number of active windows                                          
///   @return the number of active windows                                    
pcptr Platform::GetNumberOfActiveWindows() const {
   pcptr result = 0;
   for (const auto& window : mWindows) {
      if (window.IsClassEnabled())
         ++result;
   }

   return result;
}

/// Create/Destroy platform components, such as native windows                
///   @param verb - the creation/destruction verb                             
void Platform::Create(Verb& verb) {
   mWindows.Create(verb);
}