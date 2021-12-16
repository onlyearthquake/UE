#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "BaseProfile.generated.h"

class USoundBase;
class UProjectileMovementComponent;
UCLASS()
class ARPG_API ABaseProfile : public AInteractiveItem
{
	GENERATED_BODY()
	ABaseProfile();
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* HitSound;

	void PlayEffect()const;

	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere,Category= "Data")
	AActor* OwnActor;
	UPROPERTY(VisibleAnywhere,Category= "Data")
	float DamageValue;
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
