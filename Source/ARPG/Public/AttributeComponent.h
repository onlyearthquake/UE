// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/FactionKind.h"
#include "AttributeComponent.generated.h"


UCLASS()
class ARPG_API UAttributeComponent : public UActorComponent
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UAttributeComponent*, OwningComp, float, NewValue, float, Delta);

	GENERATED_BODY()
public:
	UAttributeComponent();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TEnumAsByte<EFactionKind> Faction;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxMana() const;
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);
	
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnHealthChange;
	
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnManaChange;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	float Mana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxMana;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void HealthChange(AActor* TargetActor,float Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ManaChange(AActor* TargetActor,float Delta);

	virtual void BeginPlay() override;
public:
	void InitMaxValue(float HealthVal,float ManaValue = 0.0f);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UAttributeComponent* GetAttributes(AActor* FromActor);
};
