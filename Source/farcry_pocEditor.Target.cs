// Editor target: what the Unreal Editor loads when you open the project.

using UnrealBuildTool;

public class farcry_pocEditorTarget : TargetRules
{
	public farcry_pocEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("farcry_poc");
	}
}
