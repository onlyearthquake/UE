

#include "Items/BaseProfile.h"

#include "AttributeComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseProfile::ABaseProfile()
{
	
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.0f;

}

void ABaseProfile::PlayEffect() const
{
	if(ensure(HitParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),HitParticle,GetActorLocation());
	}
	if(ensure(HitSound))
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),HitSound,GetActorLocation());
	}
	
}

void ABaseProfile::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseProfile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == OwnActor)
	{
		return;
	}
	Super::OnActorOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(OtherActor != nullptr)
	{
		UAttributeComponent::ApplyDamage(OwnActor,OtherActor,DamageValue);
		PlayEffect();
		Destroy();
	}
}
