// copyright lolol

using UnrealBuildTool;

public class ChemicodePrototype : ModuleRules
{
	public ChemicodePrototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = false;
	
		// UMG used by ResourceInfoWidget
		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "CableComponent", "Slate" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
