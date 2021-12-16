// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class ARPG_API AInteractiveItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractiveItem();

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Componet")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Componet")
	UStaticMeshComponent* MeshComponent;
	UFUNCTION(BlueprintImplementableEvent)
	void OnActorIn(AActor* TargetActor,const FHitResult & SweepResult);
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
