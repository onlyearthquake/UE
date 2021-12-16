// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "GameplayTagContainer.h"
#include "ActionComponent.h"


void UAction::Initialize(UActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


bool UAction::CanStart(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	const UActionComponent* Comp = GetOwningComponent();
	
	if (Comp->ActiveTags.HasAny(BlockTags))
	{
		return false;
	}

	return true;
}


void UAction::StartAction(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));

	UActionComponent* OwnActionComp = GetOwningComponent();	
	OwnActionComp->ActiveTags.AppendTags(AdditionalTags);

	TimeStarted = GetWorld()->TimeSeconds;
	
	bIsRunning = true;
	OwnActionComp->OnActionStarted.Broadcast(GetOwningComponent(), this);
}


void UAction::StopAction(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	UActionComponent* OwnActionComp = GetOwningComponent();
	OwnActionComp->ActiveTags.RemoveTags(AdditionalTags);
	bIsRunning = false;
	OwnActionComp->OnActionStopped.Broadcast(GetOwningComponent(), this);
}



UActionComponent* UAction::GetOwningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(USActionComponent::StaticClass());

	return ActionComp;
}

UAction::UAction()
{
}

bool UAction::IsRunning() const
{
	return bIsRunning;
}
