// copyright lolol

using UnrealBuildTool;

public class ChemicodePrototype : ModuleRules
{
	public ChemicodePrototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = true;
	
		// UMG used by ResourceInfoWidget
		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "CableComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
