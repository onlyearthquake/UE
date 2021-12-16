// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Action.generated.h"

class UActionComponent;
UCLASS()
class ARPG_API UAction : public UObject
{
	GENERATED_BODY()
	
protected:

	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer AdditionalTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockTags;
	
	UActionComponent* ActionComp = nullptr;
	
	float TimeStarted;

	bool bIsRunning;
	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionComponent* GetOwningComponent() const;
public:
	UAction();
	bool IsRunning() const;
	void Initialize(UActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION( Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(Category = "Action")
	virtual void StartAction(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual void StopAction(AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

};
