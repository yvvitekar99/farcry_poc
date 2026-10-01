#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FramePaceSubsystem.generated.h"

/**
 * Applies the target frame rate from the fc.FramePace console variable.
 *
 * On Android, t.MaxFPS and rhi.SyncInterval alone don't cap the frame rate because the
 * platform frame pacer keeps its own target; r.SetFramePace is the reliable path, but it's
 * a command, so device profiles can't set it. Device profiles set fc.FramePace instead
 * (see Config/DefaultDeviceProfiles.ini) and this subsystem forwards it to the pacer once
 * the engine is up. Changing fc.FramePace at runtime re-applies it.
 */
UCLASS()
class FARCRY_POC_API UFramePaceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Pushes fc.FramePace to the platform frame pacer. No-op when fc.FramePace is 0. */
	static void ApplyFramePace();
};
