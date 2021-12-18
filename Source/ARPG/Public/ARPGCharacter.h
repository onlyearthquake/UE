// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"State/EStateEnum.h"
#include "ARPGCharacter.generated.h"

class UAttributeComponent;
class UWorldWidget;
class AWeapon;
UCLASS(config = Game)
class AARPGCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AARPGCharacter();
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="LogicState")
	TEnumAsByte<EStateEnum> eStateEnum = EStateEnum::IDLE;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LogicState")
	float SpeedValue = 0.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComp;

	UWorldWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarClass;
	UWorldWidget* ActiveManaBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ManaBarClass;


	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* Weapon;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	TMap<FString,float> CDMap;
protected:

	void MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta);

	void Roll();

	void Rush();

	void StopRush();

	void Attack();

	UFUNCTION()
	void ReturnToIdle();
	FTimerHandle TimerHandle_RecoverToIdle;
	FTimerDelegate Delegate;
protected:
	virtual  void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual  void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

