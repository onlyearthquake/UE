#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "State/EStateEnum.h"
#include "BaseEnemyAI.generated.h"

class UWorldWidget;
class UAttributeComponent;
class UPawnSensingComponent;
class AWeapon;
UCLASS()
class ARPG_API ABaseEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemyAI();
	virtual void Attack(AActor* TargetActor);
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarClass;
	
	UWorldWidget* ActiveHealthBar = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocket = "Weapon_H";
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	FName TargetActorKey;

	UPROPERTY(EditDefaultsOnly,Category="OtherData")
	float RewardValue;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AI")
	TEnumAsByte<EAIStateEnum> eAIState;
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn); 

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void OnEnemyDied();
	void CreateHealthBar();
	AWeapon* Weapon;
public:
	AWeapon* GetWeapon();
	
	void ReturnToIdle();
	UFUNCTION(BlueprintCallable, Category = "AI")
	void BeAttacked();
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;
	
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
