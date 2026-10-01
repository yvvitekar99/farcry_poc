#include "Components/HealthComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, bIsDead);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner && Owner->HasAuthority())
	{
		Health = MaxHealth;
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || bIsDead)
	{
		return;
	}

	AActor* Instigator = InstigatedBy ? InstigatedBy->GetPawn() : DamageCauser;
	SetHealth(Health - Damage, Instigator);
}

void UHealthComponent::Heal(float Amount)
{
	if (Amount <= 0.f || bIsDead || !GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	SetHealth(Health + Amount, nullptr);
}

void UHealthComponent::SetHealth(float NewHealth, AActor* Instigator)
{
	const float OldHealth = Health;
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);

	const float Delta = Health - OldHealth;
	if (!FMath::IsNearlyZero(Delta))
	{
		OnHealthChanged.Broadcast(this, Health, Delta);
	}

	if (Health <= 0.f && !bIsDead)
	{
		bIsDead = true;
		OnDeath.Broadcast(this, Instigator);
	}
}

void UHealthComponent::OnRep_Health(float OldHealth)
{
	OnHealthChanged.Broadcast(this, Health, Health - OldHealth);
}

void UHealthComponent::OnRep_IsDead()
{
	if (bIsDead)
	{
		// The killer isn't replicated; clients only need to react to the death itself.
		OnDeath.Broadcast(this, nullptr);
	}
}
