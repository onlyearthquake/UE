#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"
class UCapsuleComponent;
UCLASS()
class AWeapon : public AActor
{
	GENERATED_BODY()
public:	
	AWeapon();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	AActor* OwnActor;
protected:
	virtual void BeginPlay() override;
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	float DamageValue;
	virtual void Attack(AActor* TargetActor);
	virtual void Tick(float DeltaTime) override;
};

