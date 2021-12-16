// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "Components/ActorComponent.h"
#include "ARPGGameMode.h"
#include "ARPGGameState.h"

UAttributeComponent::UAttributeComponent()
{
	Faction = EFactionKind::Enemy;
	//MaxHealth = MaxMana = 100;
	//Health = MaxHealth;
	//Mana = MaxMana;
}

float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UAttributeComponent::GetMana() const
{
	return Mana;
}

float UAttributeComponent::GetMaxMana() const
{
	return MaxMana;
}

void UAttributeComponent::HealthChange(AActor* TargetActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged() || Health <= 0)
	{
		//when it die not trig
		return;
	}
	Health += Delta;
	//UE_LOG(LogTemp,Log,TEXT("%d"),Health)
	if(Health <= 0)
	{
		AARPGGameMode* GameMode = Cast<AARPGGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnActorKill(TargetActor);
		}
	}
	Health = FMath::Clamp<float>(Health,0,MaxHealth);

	OnHealthChange.Broadcast(TargetActor, this, Health, Delta);
}

void UAttributeComponent::ManaChange(AActor* TargetActor, float Delta)
{
	
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Mana = MaxMana;
}

void UAttributeComponent::InitMaxValue(float HealthVal, float ManaValue)
{
	this->MaxHealth = HealthVal;
	this -> MaxMana = ManaValue;
	Health = MaxHealth;
	Mana = MaxMana;
}

UAttributeComponent* UAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UAttributeComponent>(FromActor->GetComponentByClass(UAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UAttributeComponent::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UAttributeComponent* TargetAttribute = UAttributeComponent::GetAttributes(TargetActor);
	UAttributeComponent* CauserAttribute = UAttributeComponent::GetAttributes(DamageCauser);
	if (CauserAttribute != nullptr && TargetAttribute != nullptr)
	{
		if(CauserAttribute->Faction == TargetAttribute->Faction)
		{
			return false;
		}
		TargetAttribute->HealthChange(TargetActor, -DamageAmount);
		return true;
	}
	return false;
}

bool UAttributeComponent::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			// Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}

