// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameMode.generated.h"

class UUserWidget;

UCLASS(minimalapi)
class AARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category="Game End")
	TSubclassOf<UUserWidget> EndUIClass;
	UPROPERTY(BlueprintReadOnly)
	float GameScore;
public:
	void OnActorKill(AActor* DeadActor);

	AARPGGameMode();
};



