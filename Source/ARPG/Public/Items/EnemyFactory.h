// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Actor.h"
#include "State/EnemyWave.h"
#include "EnemyFactory.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class UDataTable;
class ABaseEnemyAI;
struct FRandomStream;
UCLASS()
class ARPG_API AEnemyFactory : public AActor
{
	GENERATED_BODY()
public:	
	AEnemyFactory();
	UFUNCTION()
	void OnEnemyDie(AActor* Enemy);
protected:
	UPROPERTY(EditDefaultsOnly,Category="Data")
	bool bUseEnvQuery;
	
	FRandomStream* RandomStream; 
	UPROPERTY(EditDefaultsOnly,Category= "Data")
	UDataTable* WaveData;
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UEnvQuery* SpawnBotQuery;
	UPROPERTY(EditDefaultsOnly,Category= "Data")
	TArray<TSubclassOf<ABaseEnemyAI>> EnemyClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Data")
	TArray<ABaseEnemyAI*> CurrentEnemyArray;
	
	UPROPERTY(EditDefaultsOnly,Category="Data")
	FVector GenerateOffset;
	virtual void BeginPlay() override;
	void Generate();
	bool GenerateEnemy(FVector4 Position);
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Data")
	float ResetTime;

	int CurrentIndex = 0;

	void ReadData();

	FEnemyWaveData CurrentData;
	
	TArray<FEnemyWaveData*> WaveDatas;
public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};
