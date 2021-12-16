// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "GameplayTagContainer.h"
#include "Action.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UAction> Action : DefaultActions)
		{
			AddAction(GetOwner(), Action);
		}
	}
}


void UActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Stop all
	TArray<UAction*> ActionsCopy = Actions;
	for (UAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UActionComponent::AddAction(AActor* TargetActor, TSubclassOf<UAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UAction* NewAction = NewObject<UAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(TargetActor)))
		{
			NewAction->StartAction(TargetActor);
		}
	}
}


void UActionComponent::RemoveAction(UAction* RemovedAction)
{
	if (!ensure(RemovedAction && !RemovedAction->IsRunning()))
	{
		return;
	}

	Actions.Remove(RemovedAction);
}


UAction* UActionComponent::GetAction(TSubclassOf<UAction> ActionClass) const
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}


bool UActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->CanStart(Instigator))
			{
				Action->StartAction(Instigator);
				return true;
			}
		}
	}

	return false;
}


bool UActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}


