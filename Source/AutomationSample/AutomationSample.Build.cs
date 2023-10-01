// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutomationSample : ModuleRules
{
    public AutomationSample(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.Add("AutomationSample");
    }
}