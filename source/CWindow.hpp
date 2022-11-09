#pragma once
#include <PCFW.hpp>

LANGULUS_DECLARE_EVENT(WindowClosed, 
	"Event that occurs when a platform window closes");
LANGULUS_DECLARE_EVENT(WindowFileDrop, 
	"Event that occurs when you drop a file inside a platform window");
LANGULUS_DECLARE_EVENT(WindowMouseEnter, 
	"Event that occurs when mouse enters or leaves a platform window");
LANGULUS_DECLARE_EVENT(WindowResolutionChanged, 
	"Event that occurs when a platform system changes display resolution");
LANGULUS_DECLARE_EVENT(WindowMinimized, 
	"Event that occurs when a platform window is minimized");
LANGULUS_DECLARE_EVENT(WindowFocus, 
	"Event that occurs when a platform window is focused or unfocused");
LANGULUS_DECLARE_EVENT(WindowResized, 
	"Event that occurs when a platform window is resized");
LANGULUS_DECLARE_EVENT(WindowMoved, 
	"Event that occurs when a platform window is moved");

using namespace PCFW;

class MPlatform;
struct GLFWwindow;


///																									
///	THE CROSS PLATFORM WINDOW COMPONENT													
///																									
/// Encapsulates an OS window via GLFW														
///																									
class CWindow : public AWindow, public TProducedFrom<MPlatform> {
private:
	// The window handle (GLFW specific)											
	Own<GLFWwindow*> mGLFWWindow;
	// If true, means no Vulkan														
	bool mOwnDC = true;
	// Relative scrolling accumulator												
	vec2 mScrollChange;

public:
	REFLECT(CWindow);
	CWindow(MPlatform*);
	CWindow(CWindow&&) noexcept;
	CWindow& operator = (CWindow&&) noexcept;
	~CWindow();

	void Refresh() final;
	void BindContext() final;
	NOD() bool IsInFocus() const final;
	NOD() bool IsMinimized() const final;
	NOD() bool IsMouseOver() const final;
	NOD() bool IsInteractable() const final;

	void Initialize();
	void Uninitalize();
	void Update();
	void SetSize(int x, int y);

	inline void PushTextInput(const Text& text) {
		mTextInput += text;
	}

	inline void AccumulateScroll(const vec2& offset) noexcept {
		mScrollChange += offset;
	}

};
