#pragma once

#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChemicodeEditor, All, All)

class FChemicodePrototypeEditorModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
