// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutomationSample : ModuleRules
{
    public AutomationSample(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "AITestSuite" });
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.Add("AutomationSample");
    }
}