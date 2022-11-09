#include "MPlatform.hpp"

/// Callback predefinitions																	
static void pcWindowClosedCallback		(GLFWwindow*);
static void pcWindowKeyCallback			(GLFWwindow*, int key, int scancode, int action, UNUSED() int mods);
static void pcWindowMovedCallback		(GLFWwindow*, int x, int y);
static void pcWindowResizedCallback		(GLFWwindow*, int x, int y);
static void pcWindowFocusCallback		(GLFWwindow*, int focused);
static void pcWindowMinimizeCallback	(GLFWwindow*, int iconified);
static void pcWindowResolutionCallback	(GLFWwindow*, int x, int y);
static void pcWindowCursorEnterCallback(GLFWwindow*, int entered);
static void pcWindowMouseKeyCallback	(GLFWwindow*, int button, int action, UNUSED() int mods);
static void pcWindowMouseScrollCallback(GLFWwindow*, double xoffset, double yoffset);
static void pcWindowTextInputCallback	(GLFWwindow*, uint codepoint);
#if PC_OS != PC_OS_LINUX
	static void pcWindowFileDropCallback(GLFWwindow*, int count, const char** paths);
#endif

/// Window construction																			
///	@param producer - the producer of the window										
CWindow::CWindow(MPlatform* producer)
	: AWindow {MetaData::Of<CWindow>()}
	, TProducedFrom {producer} {
	ClassValidate();
}

/// Move-construct window																		
///	@param other - the window to move													
CWindow::CWindow(CWindow&& other) noexcept
	: AWindow {pcForward<AWindow>(other)}
	, TProducedFrom {pcForward<TProducedFrom>(other)}
	, mGLFWWindow {pcMove(other.mGLFWWindow)}
	, mOwnDC {other.mOwnDC}
	, mScrollChange {other.mScrollChange} {
	if (mGLFWWindow)
		glfwSetWindowUserPointer(mGLFWWindow, this);
}

/// Move-copy window																				
///	@param other - the window to move													
CWindow& CWindow::operator = (CWindow&& other) noexcept {
	AWindow::operator = (pcForward<AWindow>(other));
	TProducedFrom::operator = (pcForward<TProducedFrom>(other));
	mGLFWWindow = pcMove(other.mGLFWWindow);
	mOwnDC = other.mOwnDC;
	mScrollChange = other.mScrollChange;
	if (mGLFWWindow)
		glfwSetWindowUserPointer(mGLFWWindow, this);
	return *this;
}

/// Window destruction																			
CWindow::~CWindow() {
	Uninitalize();
}

/// Initialize the window																		
void CWindow::Initialize() {
	if (mGLFWWindow)
		return;

	// Make it visible																	
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	mOwnDC = false;

	// Create the canvas																	
	mGLFWWindow = glfwCreateWindow(
		int(mSize[0]), int(mSize[1]),
		const_cast<char*>(mTitle.Terminate().GetRaw()),
		nullptr, nullptr
	);

	if (!mGLFWWindow)
		throw Except::Platform(pcLogSelfError << "Failed to initialize");

	// Set the callbacks and user pointers for the canvas pipe				
	glfwSetWindowCloseCallback(mGLFWWindow, pcWindowClosedCallback);
	glfwSetKeyCallback(mGLFWWindow, pcWindowKeyCallback);
	glfwSetWindowPosCallback(mGLFWWindow, pcWindowMovedCallback);
	glfwSetWindowSizeCallback(mGLFWWindow, pcWindowResizedCallback);
	glfwSetWindowFocusCallback(mGLFWWindow, pcWindowFocusCallback);
	glfwSetWindowIconifyCallback(mGLFWWindow, pcWindowMinimizeCallback);
	glfwSetFramebufferSizeCallback(mGLFWWindow, pcWindowResolutionCallback);
	glfwSetCursorEnterCallback(mGLFWWindow, pcWindowCursorEnterCallback);
	glfwSetMouseButtonCallback(mGLFWWindow, pcWindowMouseKeyCallback);
	glfwSetScrollCallback(mGLFWWindow, pcWindowMouseScrollCallback);
	glfwSetCharCallback(mGLFWWindow, pcWindowTextInputCallback);
	#if PC_OS != PC_OS_LINUX
		glfwSetDropCallback(mGLFWWindow, pcWindowFileDropCallback);
	#endif

	// Set the user pointer inside mWindow to carry this component			
	glfwSetWindowUserPointer(mGLFWWindow, this);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(mGLFWWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	// Activate rendering context, so that other modules react				
	if (mOwnDC)
		glfwMakeContextCurrent(mGLFWWindow);

	AWindow::mNativeWindowHandle = pcGetNativeWindowPointer(mGLFWWindow);
	AWindow::mRenderer = GetOwners()[0]->CreateUnit<ARenderer>(this);
}

/// Uninitialize the window																	
void CWindow::Uninitalize() {
	if (mGLFWWindow) {
		glfwDestroyWindow(mGLFWWindow);
		mGLFWWindow = nullptr;
		AWindow::mRenderer.Reset();
		AWindow::mNativeWindowHandle.Reset();
	}
}

/// Bind context																					
void CWindow::BindContext() {
	if (mOwnDC) {
		glfwMakeContextCurrent(mGLFWWindow);
		glfwSwapBuffers(mGLFWWindow);
	}
}

/// Refresh the window component, initializing it of needed							
void CWindow::Refresh() {
	Initialize();

	if (mGLFWWindow) {
		// Attempt extracting title from owners									
		for (auto owner : GetOwners()) {
			auto name = owner->GetName();
			if (!name.IsEmpty()) {
				// Break on first find													
				mTitle = pcMove(name);
				break;
			}
		}

		glfwSetWindowTitle(mGLFWWindow, mTitle.Terminate().GetRaw());
	}
}

/// Update the window																			
void CWindow::Update() {
	if (!mGLFWWindow)
		return;

	mMousePosition.Update();
	mMouseScroll.Update();

	if (IsInteractable() && IsMouseOver()) {
		// Handle mouse movement														
		double mouseX, mouseY;
		glfwGetCursorPos(mGLFWWindow, &mouseX, &mouseY);
		mMousePosition.Current() = vec2(real(mouseX), real(mouseY));

		// Check if mouse position has changed, and add specific events	
		auto md = mMousePosition.Delta();
		if (!pcNear(md[0], real(0))) {
			// Horizontal mouse movement												
			auto interact = Verb::From<Verbs::Interact>(
				{}, Any::Wrap(Keys::MouseMoveHorizontal::ID, md[0]));
			DoInHierarchy(interact, SeekStyle::DownFromHere);
		}

		if (!pcNear(md[1], real(0))) {
			// Vertical mouse movement													
			auto interact = Verb::From<Verbs::Interact>(
				{}, Any::Wrap(Keys::MouseMoveVertical::ID, md[1]));
			DoInHierarchy(interact, SeekStyle::DownFromHere);
		}

		// Handle mouse scroll															
		mMouseScroll.Current() += mScrollChange;
		mScrollChange = {};

		// Check if mouse scroll has changed, and add specific events		
		auto ms = mMouseScroll.Delta();
		if (!pcNear(ms[0], real(0))) {
			// Horizontal scrolling														
			auto interact = Verb::From<Verbs::Interact>(
				{}, Any::Wrap(Keys::MouseScrollHorizontal::ID, real(ms[0])));
			DoInHierarchy(interact, SeekStyle::DownFromHere);
		}

		if (!pcNear<real>(ms[1], 0)) {
			// Vertical scrolling														
			auto interact = Verb::From<Verbs::Interact>(
				{}, Any::Wrap(Keys::MouseScrollVertical::ID, real(ms[1])));
			DoInHierarchy(interact, SeekStyle::DownFromHere);
		}
	}

	if (!mTextInput.IsEmpty()) {
		// Interact using the text inputs											
		//auto interact = uvInteract({}, PCT(buffer, count));
		//canvas->DoInHierarchy(interact);mTextInput
		mTextInput.Clear();
	}
}

/// Resize the window																			
///	@param x - horizontal size																
///	@param y - vertical size																
void CWindow::SetSize(int x, int y) {
	const vec2 newSize {x, y};
	if (AWindow::mSize != newSize && AWindow::mRenderer) {
		AWindow::mRenderer->Resize(newSize);
		AWindow::mSize = newSize;
	}
}

/// Check if window is in focus																
bool CWindow::IsInFocus() const {
	return glfwGetWindowAttrib(mGLFWWindow, GLFW_FOCUSED) == GLFW_TRUE;
}

/// Check if window is in focus																
bool CWindow::IsMinimized() const {
	return glfwGetWindowAttrib(mGLFWWindow, GLFW_ICONIFIED) == GLFW_TRUE;
}

/// Check if window is in focus																
bool CWindow::IsMouseOver() const {
	return glfwGetWindowAttrib(mGLFWWindow, GLFW_HOVERED) == GLFW_TRUE;
}

/// Check if window interacts with piception												
bool CWindow::IsInteractable() const {
	return IsClassEnabled() && IsInFocus() && !IsMinimized();
}

/// Get the CWindow associated with a GLFW window										
inline auto GetUnit(GLFWwindow* window) {
	return static_cast<CWindow*>(glfwGetWindowUserPointer(window));
}


///																									
/// CALLBACKS																						
///																									
/// On window close																				
///	@param window - the event's owner													
void pcWindowClosedCallback(GLFWwindow* window) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;

	glfwHideWindow(window);
	canvas->ClassDisable();
	auto interact = Verb::From<Verbs::Interact>(
		{}, Events::WindowClosed::ID);
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On key/mouse press/move																	
///	@param window - the event's owner													
///	@param key - the pressed/released key												
///	@param scancode - i don't know what the fuck this is							
///	@param action - pressed, released, repeated, etc...							
///	@param mods - i guess it has something to do with key combinations		
void pcWindowKeyCallback(GLFWwindow* window, int key, int, int action, int) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;
	if (action != GLFW_PRESS && action != GLFW_RELEASE)
		return;

	auto interact = Verb::From<Verbs::Interact>(
		{}, PCFW::GLFWKeyboardTable[key]);
	if (action == GLFW_RELEASE)
		interact.Invert();
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window moved																				
///	@param window - the event's owner													
///	@param x - new position (horizontal screen offset in pixels)				
///	@param y - new position (vertical screen offset in pixels)					
void pcWindowMovedCallback(GLFWwindow* window, int x, int y) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;

	auto interact = Verb::From<Verbs::Interact>(
		{}, Any::Wrap(Events::WindowMoved::ID, vec2(real(x), real(y))));
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window resized																			
///	@param window - the event's owner													
///	@param x - new scale (width in pixels)												
///	@param y - new scale (height in pixels)											
void pcWindowResizedCallback(GLFWwindow* window, int x, int y) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;

	canvas->SetSize(x, y);
	auto interact = Verb::From<Verbs::Interact>(
		{}, Any::Wrap(Events::WindowResized::ID, vec2(real(x), real(y))));
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window focused or not																	
///	@param window - the event's owner													
///	@param focused - focused state														
void pcWindowFocusCallback(GLFWwindow* window, int focused) {
	auto canvas = GetUnit(window);
	if (canvas->IsClassDisabled())
		return;

	auto interact = Verb::From<Verbs::Interact>(
		{}, Events::WindowFocus::ID);
	if (!focused)
		interact.Invert();
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window minimized or not																
///	@param window - the event's owner													
///	@param iconified - iconification state												
void pcWindowMinimizeCallback(GLFWwindow* window, int iconified) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;

	auto interact = Verb::From<Verbs::Interact>(
		{}, Events::WindowMinimized::ID);
	if (!iconified)
		interact.Invert();
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On window resolution change																
///	@param window - the event's owner													
///	@param x - new resolution (width in pixels)										
///	@param y - new resolution (height in pixels)										
void pcWindowResolutionCallback(GLFWwindow* window, int x, int y) {
	auto canvas = GetUnit(window);
	if (canvas->IsClassDisabled())
		return;

	auto interact = Verb::From<Verbs::Interact>(
		{}, Any::Wrap(Events::WindowResolutionChanged::ID, vec2(real(x), real(y))));
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On mouse enter window																		
///	@param window - the event's owner													
///	@param entered - zero if leave, one if entered									
void pcWindowCursorEnterCallback(GLFWwindow* window, int entered) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;

	auto interact = Verb::From<Verbs::Interact>(
		{}, Events::WindowMouseEnter::ID);
	if (!entered)
		interact.Invert();
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// On mouse key																					
///	@param window - the event's owner													
///	@param button - button that was pressed or realeased							
///	@param action - the action that the button performed							
///	@param mods - mods for button combinations										
void pcWindowMouseKeyCallback(GLFWwindow* window, int button, int action, int) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;
	if (action != GLFW_PRESS && action != GLFW_RELEASE)
		return;

	auto interact = Verb::From<Verbs::Interact>(
		{}, PCFW::GLFWMouseTable[button]);
	if (action == GLFW_RELEASE)
		interact.Invert();
	canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
}

/// Returns last written UTF-32 character, affected by mod keys, language		
/// and all localization stuff on your platform.										
///	@param window - the event's owner													
///	@param codepoint - UTF-32 code point												
void pcWindowTextInputCallback(GLFWwindow* window, uint codepoint) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;

	canvas->PushTextInput(Text::FromCodepoint(codepoint));
}

/// On mouse scroll (supports 2D scrolling)												
///	@param window - the event's owner													
///	@param xoffset - the new mouse x position											
///	@param yoffset - the new mouse y position											
void pcWindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	auto canvas = GetUnit(window);
	if (!canvas->IsInteractable())
		return;

	canvas->AccumulateScroll({ real(xoffset), real(yoffset) });
}

#if PC_OS != PC_OS_LINUX
	/// On file drop to window																	
	///	@param window - the event's owner												
	///	@param count - number of dropped files											
	///	@param paths - deep container with filenames									
	void pcWindowFileDropCallback(GLFWwindow* window, int count, const char** paths) {
		auto canvas = GetUnit(window);
		if (!canvas->IsInteractable())
			return;

		Any package;
		package << Any::Wrap(Events::WindowFileDrop::ID);
		for (int i = 0; i < count; ++i)
			package << Text(paths[i]);

		auto interact = Verb::From<Verbs::Interact>({}, &package);
		canvas->DoInHierarchy(interact, SeekStyle::DownFromHere);
	}
#endif
