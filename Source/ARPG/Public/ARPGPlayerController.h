
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AARPGPlayerController();
	bool Reborn();
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int RebornTime;
};
