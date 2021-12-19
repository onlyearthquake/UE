// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RadialBlurTrigger.h"

#include "Engine/PostProcessVolume.h"
#include "GameFramework/PawnMovementComponent.h"

ARadialBlurTrigger::ARadialBlurTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARadialBlurTrigger::BeginPlay()
{
	Super::BeginPlay();
	if(ensure(BlurMat))
	{
		BlurMID = UMaterialInstanceDynamic::Create(BlurMat, this, FName("RadialBlurMID"));
	}
	const int32 num = GetWorld()->PostProcessVolumes.Num();
	if (num > 0)
	{
		PostProcessVolumeActor = Cast<APostProcessVolume>(GetWorld()->PostProcessVolumes[0]);
		if(ensure(PostProcessVolumeActor))
		{
			PostProcessVolumeActor->bEnabled = true;
			PostProcessVolumeActor->BlendWeight = 1.0f;
			PostProcessVolumeActor->bUnbound = true;
		}
	}
}

void ARadialBlurTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(ensure(Player))
	{
		PlayerMoveComp = Player->GetMovementComponent();
	}
	if(PlayerMoveComp->GetMaxSpeed() * 0.9f < PlayerMoveComp->Velocity.Size())
	{
		if(!HasOpenPostProcess)
		{
			RushTime += DeltaTime;
			if(RushTime > TriggerTime)
			{
				Trigger();
			}
		}
	}
	else
	{
		RushTime = 0.0f;
		if(HasOpenPostProcess)
		{
			Shutdown();
		}
	}
	
}

void ARadialBlurTrigger::Trigger()
{
	if (PostProcessVolumeActor != nullptr)
	{
		PostProcessVolumeActor->AddOrUpdateBlendable(BlurMID, 1);
		HasOpenPostProcess = true;
	}
}

void ARadialBlurTrigger::Shutdown()
{
	if (PostProcessVolumeActor != nullptr)
	{
		PostProcessVolumeActor->AddOrUpdateBlendable(BlurMID, 0);
		HasOpenPostProcess = false;
	}
}