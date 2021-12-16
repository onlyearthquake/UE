// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "AttackAction.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UAttackAction : public UAction
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category="AttackParams")
	float DelayTime;
	class ABaseWeapon* OwnWeapon;
	virtual void StartAction(AActor* TargetActor) override;
};
