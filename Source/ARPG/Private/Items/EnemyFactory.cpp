// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EnemyFactory.h"

#include "AttributeComponent.h"
#include "Engine/DataTable.h"
#include "StaticMeshAttributes.h"
#include "AI/BaseEnemyAI.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Weapon/Weapon.h"

AEnemyFactory::AEnemyFactory()
{
	PrimaryActorTick.bCanEverTick = true;
	RandomStream = new FRandomStream();
}

void AEnemyFactory::OnEnemyDie(AActor* Enemy)
{
	ABaseEnemyAI* BaseEnemyAI = Cast<ABaseEnemyAI>(Enemy);
	if(BaseEnemyAI)
	{
		if(CurrentEnemyArray.Contains(BaseEnemyAI))
		{
			CurrentEnemyArray.Remove(BaseEnemyAI);
			if(CurrentEnemyArray.Num() == 0)
			{
				//TODO:OnClear
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("不存在该敌人 有鬼"))
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("无效Actor!"))
	}
}

void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	//WaveData = LoadObject<UDataTable>(NULL, UTF8_TO_TCHAR("DataTable'/Game/MyTable.MyTable'"));
	if(WaveData != nullptr)
	{
		WaveData->GetAllRows(TEXT("ggg"),OUT WaveDatas);
		ReadData();
		Generate();
	}
}

void AEnemyFactory::Generate()
{
	if(bUseEnvQuery)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AEnemyFactory::OnBotSpawnQueryCompleted);
		}
	}
	else GenerateEnemy(FVector::ZeroVector);
}

bool AEnemyFactory::GenerateEnemy(FVector4 Position)
{
	if(EnemyClass.Num() <= 0)
	{
		return false;
	}
	FDateTime CurrentTime = FDateTime::Now();
	RandomStream->Initialize(CurrentTime.ToUnixTimestamp());
	const int32 RandomIndex = RandomStream->RandRange(0,EnemyClass.Num() - 1);
	if(!bUseEnvQuery){
		const FVector Offset = GenerateOffset * FVector(RandomStream->RandRange(0,1.0),RandomStream->RandRange(0,1.0),RandomStream->RandRange(0,1.0));
		Position = GetActorLocation() + Offset;
	}
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	ABaseEnemyAI* GeneratingEnemyAI = GetWorld()->SpawnActor<ABaseEnemyAI>(EnemyClass[RandomIndex], Position,GetActorRotation(),Params);
	if(ensure(GeneratingEnemyAI))
	{
		GeneratingEnemyAI->GetWeapon()->DamageValue = CurrentData.Damage;
		CurrentEnemyArray.Add(GeneratingEnemyAI);
		UAttributeComponent::GetAttributes(GeneratingEnemyAI)->InitMaxValue(CurrentData.HP);
		//UE_LOG(LogTemp,Log,TEXT("HP:%d"),CurrentData.HP)
		CurrentData.CreateNum--;
	}
	return true;
}

void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(ensure(WaveData))
	{
		ResetTime -= DeltaTime;
		if(ResetTime <= 0)
		{
			if(CurrentData.CreateNum > 0)
			{
				ResetTime = CurrentData.CreateInterval;
				Generate();
			}
			else
			{
				ReadData();
			}
		}
	}
}

void AEnemyFactory::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{	
		GenerateEnemy(Locations[0]);
	}
}


void  AEnemyFactory::ReadData()
{
	if(ensure(WaveData))
	{
		CurrentData = *WaveDatas[CurrentIndex];
		CurrentIndex++;
		if(CurrentIndex > WaveDatas.Num())
		{
			//TODO:End The Battle Or Continue
			Destroy();
		}
		ResetTime = CurrentData.CreateInterval;
	}
}
