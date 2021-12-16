// Copyright Epic Games, Inc. All Rights Reserved.

#include "ARPGGameMode.h"
#include "ARPGCharacter.h"
#include "ARPGGameState.h"
#include "ARPGPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AARPGGameMode::OnActorKill(AActor* DeadActor)
{
	AARPGCharacter* Character = Cast<AARPGCharacter>(DeadActor);
	if(Character != nullptr)
	{
		AARPGPlayerController* PlayerController = Cast<AARPGPlayerController>(GetWorld()->GetFirstPlayerController());
		if(ensure(PlayerController) && PlayerController->Reborn())
		{
			//If Has Reborn Times
			PlayerController->GetPawn()-> Destroy();
			PlayerController->UnPossess();
			RestartPlayer(PlayerController);
		}
		else
		{
			//Show End UI 
			UUserWidget*  EndUI = CreateWidget<UUserWidget>(GetWorld(),EndUIClass,TEXT("EndUI"));
			EndUI->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(),true);
		}
	}
	else
	{
		//Add The Score
		//TODO:Different Enemy Different Scores
		GameScore += 5.0f;
	}
}

AARPGGameMode::AARPGGameMode()
{
	GameStateClass= AARPGGameState::StaticClass();
}
