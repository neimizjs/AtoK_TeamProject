// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Defence_AtoK : ModuleRules
{
	public Defence_AtoK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Defence_AtoK",
			"Defence_AtoK/Variant_Strategy",
			"Defence_AtoK/Variant_Strategy/UI",
			"Defence_AtoK/Variant_TwinStick",
			"Defence_AtoK/Variant_TwinStick/AI",
			"Defence_AtoK/Variant_TwinStick/Gameplay",
			"Defence_AtoK/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
