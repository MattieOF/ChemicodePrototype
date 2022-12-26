#include "ChemicodePrototypeEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FChemicodePrototypeEditorModule, MyGameEditor);

DEFINE_LOG_CATEGORY(LogChemicodeEditor)

#define LOCTEXT_NAMESPACE "ChemicodePrototypeEditor"

void FChemicodePrototypeEditorModule::StartupModule()
{
	UE_LOG(LogChemicodeEditor, Warning, TEXT("ChemicodeEditor: Log Started"));
}

void FChemicodePrototypeEditorModule::ShutdownModule()
{
	UE_LOG(LogChemicodeEditor, Warning, TEXT("ChemicodeEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE
