// copyright lolol

using UnrealBuildTool;

public class ChemicodePrototype : ModuleRules
{
	public ChemicodePrototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		// RenderCore and RHI are used for getting vertex positions for usage in the CameraPlane
		PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore", "RHI" });
	}
}
