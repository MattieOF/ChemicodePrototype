// copyright lolol

using UnrealBuildTool;

public class ChemicodePrototype : ModuleRules
{
	public ChemicodePrototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		// UMG used by ResourceInfoWidget
		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
