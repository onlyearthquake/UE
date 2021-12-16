#include "Weapon/FarWeapon.h"

#include "Items/BaseProfile.h"


void AFarWeapon::Attack(AActor* TargetActor)
{
	const FVector MuzzleLocation = GetActorLocation() + InitOffeset;
	
	//Target
	const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	//Params.bNoFail = true;
	ABaseProfile* InitProfile = GetWorld()->SpawnActorDeferred<ABaseProfile>(ProfileClass,this->GetTransform());
	InitProfile->OwnActor = OwnActor;
	InitProfile->DamageValue = DamageValue;
	InitProfile->SetActorLocation(MuzzleLocation);
	InitProfile->SetActorRotation(MuzzleRotation);
	InitProfile->FinishSpawning(this->GetTransform());
	//ABaseProfile* InitProfile = GetWorld()->SpawnActor<ABaseProfile>(ProfileClass, MuzzleLocation, MuzzleRotation,Params);
	
	//InitProfile->OwnActor = OwnActor;
	//InitProfile->DamageValue = DamageValue;

}
