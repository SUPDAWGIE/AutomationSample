// Copyright Epic Games, Inc. All Rights Reserved.

using System.Linq;
using UnrealBuildTool;

public class AutomationSample : ModuleRules
{
    public AutomationSample(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "AITestSuite" });
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Json", "JsonUtilities", "UMG", "SlateCore" });

        PublicIncludePaths.Add("AutomationSample");

        if (Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            PrivateDependencyModuleNames.Add("FunctionalTesting");
        }

        if (base.Target.ProjectDefinitions.Contains("UNOPTIMIZED_CODE"))
        {
            OptimizeCode = CodeOptimization.Never;
        }
    }
}