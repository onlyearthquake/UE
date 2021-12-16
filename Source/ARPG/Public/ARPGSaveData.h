
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ARPGSaveData.generated.h"

USTRUCT(BlueprintType)
struct FSaveKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Score;
};

UCLASS()
class ARPG_API UARPGSaveData : public USaveGame
{

	GENERATED_BODY()
public:
	int MaxSaveCount = 4;
	UFUNCTION(BlueprintCallable)
	void AddNewRecord(FString Name,float Score);
	UPROPERTY(BlueprintReadOnly)
	TArray<FSaveKey> Scores;
};
