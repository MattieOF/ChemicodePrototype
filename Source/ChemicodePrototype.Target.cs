// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ChemicodePrototypeTarget : TargetRules
{
	public ChemicodePrototypeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new[] { "ChemicodePrototype" } );
		if (bBuildEditor)
			ExtraModuleNames.AddRange( new[] { "ChemicodePrototypeEditor" } );
	}
}
