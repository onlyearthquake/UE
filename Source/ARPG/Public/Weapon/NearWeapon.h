
#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "NearWeapon.generated.h"

class USoundCue;
UCLASS()
class ARPG_API ANearWeapon : public AWeapon
{
	GENERATED_BODY()
	ANearWeapon();
protected:

	bool bAbleToUse = false;
	UFUNCTION()
	void ReturnToIdle();
	
	void PlayEffect()const;

	TArray<AActor*> CacheActors;
	
	FTimerHandle TimerHandle_AttackDelay;

	virtual void BeginPlay() override;
public:
	virtual void Attack(AActor* TargetActor) override;
	void DisableWeapon();
	virtual void SetAbleToUse(float TimeValue);
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
