// Game target: packaged builds (Windows, Android, later consoles).

using UnrealBuildTool;

public class farcry_pocTarget : TargetRules
{
	public farcry_pocTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("farcry_poc");
	}
}
