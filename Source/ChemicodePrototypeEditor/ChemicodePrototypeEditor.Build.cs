// copyright lolol

using UnrealBuildTool;

public class ChemicodePrototypeEditor : ModuleRules
{
	public ChemicodePrototypeEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ChemicodePrototype", "Slate" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}