using UnrealBuildTool;

public class farcry_poc : ModuleRules
{
	public farcry_poc(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Lets code include headers by folder, e.g. "Components/HealthComponent.h".
		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput"
		});

		// Added as systems come online: AIModule, NavigationSystem, GameplayTasks,
		// StateTreeModule, GameplayStateTreeModule, UMG.
	}
}
