#include "MPlatform.hpp"

/// Error message relay																			
///	@param error - error id																	
///	@param description - text to display												
inline void GLFWErrorRelay(int error, const char* description) {
	pcLogError << "GLFW Error #" << error << ": " << description;
}

/// Module construction																			
///	@param system - the platform creator												
///	@param handle - the library handle													
MPlatform::MPlatform(CRuntime* system, PCLIB handle)
	: AModulePlatform {MetaData::Of<MPlatform>(), system, handle}
	, mWindows {this} {
	pcLogSelfVerbose << "Initializing...";

	// Initialize GLFW																	
	if (!glfwInit())
		throw Except::Platform(pcLogSelfError << "Error initializing GLFW");

	// Bind our logger																	
	glfwSetErrorCallback(GLFWErrorRelay);

	pcLogSelfVerbose << "Initialized";
	ClassValidate();
}

/// Module destruction																			
MPlatform::~MPlatform() {
	glfwTerminate();
}

/// Module update routine																		
///	@param dt - time from last update													
void MPlatform::Update(PCTime) {
	// Retrieve and dispatch OS events												
	glfwPollEvents();

	// Update all enabled windows														
	for (auto& window : mWindows) {
		if (window.IsClassEnabled())
			window.Update();
	}
}

/// Get the number of active windows														
///	@return the number of active windows												
pcptr MPlatform::GetNumberOfActiveWindows() const {
	pcptr result = 0;
	for (const auto& window : mWindows) {
		if (window.IsClassEnabled())
			++result;
	}

	return result;
}

/// Create/Destroy platform components, such as native windows						
///	@param verb - the creation/destruction verb										
void MPlatform::Create(Verb& verb) {
	mWindows.Create(verb);
}

/// Solve missing arguments by displaying dialogs for input.						
/// Used as a fallback routine, if a missing argument propagates all				
/// the way to this module. Attempts to fill the gaps via dialogs.				
///	@param verb - verb to solve															
void MPlatform::SolveMissing(Verb*) {
	//TODO();
}

/// Open a browser for selecting one or more files										
///	@param context - context in which to forward the verb							
///	@param verb - the verb to solve and forward										
///	@return true if able to solve and forward											
bool MPlatform::InputFiles(AContext*, const Verb*) {
	// 1. Modal dialog																	
	// 2. Clone verb																		
	// 3. Assign new argument															
	// 4. Dispatch verb																	
	TODO();
	return false;
}

/// Open an input window for text. No parsing performed.								
///	@param context - context in which to forward the verb							
///	@param verb - the verb to solve and forward										
///	@return true if able to solve and forward											
bool MPlatform::InputText(AContext*, const Verb*) {
	// 1. Modal dialog																	
	// 2. Clone verb																		
	// 3. Assign new argument															
	// 4. Dispatch verb																	
	TODO();
	return false;
}

/// Open a color pick window.																	
///	@param context - context in which to forward the verb							
///	@param verb - the verb to solve and forward										
///	@return true if able to solve and forward											
bool MPlatform::InputColor(AContext*, const Verb*) {
	// 1. Modal dialog																	
	// 2. Clone verb																		
	// 3. Assign new argument															
	// 4. Dispatch verb																	
	TODO();
	return false;
}

/// Open a text input window. Text is parsed as GASM, and forwarded.				
///	@param context - context in which to forward the verb							
///	@param verb - the verb to solve and forward										
///	@return true if able to solve and forward											
bool MPlatform::InputGASM(AContext*, const Verb*) {
	// 1. Modal dialog																	
	// 2. Parse input																		
	// 3. Check input compatibility with argument								
	// 4. Clone verb																		
	// 5. Assign new argument															
	// 6. Dispatch verb																	
	TODO();
	return false;
}
