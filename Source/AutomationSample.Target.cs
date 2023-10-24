// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class AutomationSampleTarget : TargetRules
{

    [CommandLine("-UnoptimizedCode")]
    public bool UnoptimizedCode = false;
    public AutomationSampleTarget(TargetInfo Target) : base(Target)
    {
        if (UnoptimizedCode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }

        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        ExtraModuleNames.Add("AutomationSample");
    }
}
