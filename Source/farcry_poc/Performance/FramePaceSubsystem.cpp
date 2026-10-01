#include "Performance/FramePaceSubsystem.h"

#include "farcry_poc.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformFramePacer.h"

namespace FramePace
{
	static int32 TargetFramePace = 0;

	// The frame pacer only exists after the RHI starts, but device profiles set this cvar
	// earlier during boot, so changes are ignored until the subsystem says it's safe.
	static bool bReadyToApply = false;

	static void OnFramePaceChanged(IConsoleVariable* /*Variable*/)
	{
		if (bReadyToApply)
		{
			UFramePaceSubsystem::ApplyFramePace();
		}
	}

	static FAutoConsoleVariableRef CVarFramePace(
		TEXT("fc.FramePace"),
		TargetFramePace,
		TEXT("Target frame rate passed to the platform frame pacer (e.g. 30). 0 = leave the engine default."),
		FConsoleVariableDelegate::CreateStatic(&OnFramePaceChanged),
		ECVF_Default);
}

void UFramePaceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FramePace::bReadyToApply = true;
	ApplyFramePace();
}

void UFramePaceSubsystem::Deinitialize()
{
	FramePace::bReadyToApply = false;

	Super::Deinitialize();
}

void UFramePaceSubsystem::ApplyFramePace()
{
	if (FramePace::TargetFramePace <= 0)
	{
		return;
	}

#if PLATFORM_ANDROID
	if (!FPlatformRHIFramePacer::IsEnabled())
	{
		UE_LOG(LogFarCry, Warning, TEXT("fc.FramePace=%d ignored: frame pacer not initialized."), FramePace::TargetFramePace);
		return;
	}
#endif

	const int32 AppliedPace = FPlatformRHIFramePacer::SetFramePace(FramePace::TargetFramePace);
	UE_LOG(LogFarCry, Log, TEXT("Frame pace requested %d, applied %d."), FramePace::TargetFramePace, AppliedPace);
}
