
#include "Weapon/NearWeapon.h"

#include "AttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

ANearWeapon::ANearWeapon()
{
}

void ANearWeapon::ReturnToIdle()
{
	bAbleToUse = false;
	CacheActors.Reset();
}

void ANearWeapon::PlayEffect() const
{
	EffectComp ->Activate();
	AudioComp ->Activate();
}

void ANearWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this,&ANearWeapon::OnActorOverlap);
}

void ANearWeapon::Attack(AActor* TargetActor)
{
	Super::Attack(TargetActor);
	SetAbleToUse(0.6f);
}

void ANearWeapon::SetAbleToUse(float TimeValue)
{
	bAbleToUse = true;
	
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "ReturnToIdle");

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, TimeValue, false);
}

void ANearWeapon::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bAbleToUse || CacheActors.Contains(OtherActor) || OtherActor == OwnActor)
	{
		return;
	}
	if(UAttributeComponent::ApplyDamage(OwnActor,OtherActor,DamageValue))
	{
		//UE_LOG(LogTemp,Log,TEXT("Damage!"))
		CacheActors.Add(OtherActor);
		PlayEffect();
	}
}
