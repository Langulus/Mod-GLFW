#include "MPlatform.hpp"

//TODO automate this


/// Reflect the module																			
REFLECT_BEGIN(MPlatform)
	REFLECT_INFO("Platform management module.")
	REFLECT_BEGIN_BASES
		REFLECT_BASE(AModulePlatform)
	REFLECT_END_BASES
	REFLECT_BEGIN_MEMBERS
		REFLECT_MEMBER_TRAIT(mWindows, Factory)
	REFLECT_END_MEMBERS
	REFLECT_BEGIN_ABILITIES
		REFLECT_ABILITY(Create)
	REFLECT_END_ABILITIES
REFLECT_END

LANGULUS_DEFINE_MODULE(MPlatform);


/// Reflect the window component																
REFLECT_BEGIN(CWindow)
	REFLECT_INFO("platform window component")
	REFLECT_PRODUCER(MPlatform)
	REFLECT_BEGIN_BASES
		REFLECT_BASE(AWindow)
	REFLECT_END_BASES
REFLECT_END


/// Links the module with the framework													
void LANGULUS_MODULE_LINKER() {
	MetaData::REGISTER<MPlatform>();
	MetaData::REGISTER<CWindow>();

	MetaData::REGISTER<Events::WindowClosed>();
	MetaData::REGISTER<Events::WindowFileDrop>();
	MetaData::REGISTER<Events::WindowMouseEnter>();
	MetaData::REGISTER<Events::WindowResolutionChanged>();
	MetaData::REGISTER<Events::WindowMinimized>();
	MetaData::REGISTER<Events::WindowFocus>();
	MetaData::REGISTER<Events::WindowResized>();
	MetaData::REGISTER<Events::WindowMoved>();
}

/// Unlinks the module																			
void LANGULUS_MODULE_DESTROYER() {
	MetaData::UNREGISTER<Events::WindowClosed>();
	MetaData::UNREGISTER<Events::WindowFileDrop>();
	MetaData::UNREGISTER<Events::WindowMouseEnter>();
	MetaData::UNREGISTER<Events::WindowResolutionChanged>();
	MetaData::UNREGISTER<Events::WindowMinimized>();
	MetaData::UNREGISTER<Events::WindowFocus>();
	MetaData::UNREGISTER<Events::WindowResized>();
	MetaData::UNREGISTER<Events::WindowMoved>();

	MetaData::UNREGISTER<CWindow>();
	MetaData::UNREGISTER<MPlatform>();
}
