
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "FarWeapon.generated.h"

class ABaseProfile;
UCLASS()
class ARPG_API AFarWeapon : public AWeapon
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category="Data")
	FVector4 InitOffeset;
	UPROPERTY(EditDefaultsOnly,Category="Data")
	TSubclassOf<ABaseProfile> ProfileClass;

	virtual void Attack(AActor* TargetActor)override;
};
