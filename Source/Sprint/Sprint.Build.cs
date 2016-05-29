// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sprint : ModuleRules
{
	public Sprint(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG"});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore"});
	}
}
