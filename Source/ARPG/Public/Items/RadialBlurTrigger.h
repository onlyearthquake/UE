// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadialBlurTrigger.generated.h"

class UMaterial;
class APostProcessVolume;
UCLASS()
class ARPG_API ARadialBlurTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARadialBlurTrigger();

protected:
	UPROPERTY(EditDefaultsOnly,Category= "Setting")
	UMaterial* BlurMat;
	
	UMaterialInstanceDynamic* BlurMID;
	
	APostProcessVolume* PostProcessVolumeActor;

	UPawnMovementComponent* PlayerMoveComp;

	float RushTime;

	bool HasOpenPostProcess = false;

	UPROPERTY(EditDefaultsOnly,Category="Setting")
	float TheresoldFaction;

	UPROPERTY(EditDefaultsOnly,Category="Setting")
	float TriggerTime;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Trigger();
	void Shutdown();
};
