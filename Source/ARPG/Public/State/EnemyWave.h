#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyWave.generated.h"

USTRUCT(BlueprintType)
struct FEnemyWaveData : public FTableRowBase
{
	
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	float CreateInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	int CreateNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	int32 Damage;
};