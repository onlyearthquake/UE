
#include "AI/BaseEnemyAI.h"

#include "ARPGCharacter.h"
#include "AttributeComponent.h"
#include "AI/BaseAIController.h"
#include "Weapon/Weapon.h"
#include "BrainComponent.h"
#include "WorldWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

ABaseEnemyAI::ABaseEnemyAI()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>("AttributeComp");
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	TargetActorKey = "TargetActor";
	
}

void ABaseEnemyAI::Attack(AActor* TargetActor)
{
	if(eAIState != EAIStateEnum::IDLE)
	{
		return;
	}
	//转向
	FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
	Dir.Normalize();
	FRotator Rotator = Dir.ToOrientationRotator();
	this->SetActorRotation(Rotator);

	//攻击
	Weapon->Attack(TargetActor);
	eAIState = EAIStateEnum::ATTACK;
}

void ABaseEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	ActiveHealthBar = CreateWidget<UWorldWidget>(GetWorld(), HealthBarClass);
	if (ActiveHealthBar)
	{
		ActiveHealthBar->AttachedActor = this;
		ActiveHealthBar->AddToViewport();
	}
	
	Weapon = GetWeapon();
}
void ABaseEnemyAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ABaseEnemyAI::OnPawnSeen);
	AttributeComp->OnHealthChange.AddDynamic(this, &ABaseEnemyAI::OnHealthChanged);
}
void ABaseEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemyAI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Weapon->Destroy();
}

void ABaseEnemyAI::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
	}
}

void ABaseEnemyAI::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		AIController->GetBlackboardComponent()->SetValueAsBool("IsAttacked",true);
		AARPGCharacter* Player = Cast<AARPGCharacter>(InstigatorActor);
		if (Player != nullptr)
		{
			SetTargetActor(Player);
		}

		// Died
		if (NewHealth <= 0.0f)
		{
			OnEnemyDied();
		}
	}
}

void ABaseEnemyAI::OnEnemyDied()
{
	// stop BT
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBrainComponent()->StopLogic("Killed");
	}

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	
	//Weapon->DisableWeapon();
	SetLifeSpan(7.0f);
	ActiveHealthBar->RemoveFromParent();
}

AWeapon* ABaseEnemyAI::GetWeapon()
{
	if(Weapon == nullptr)
	{
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		Weapon->OwnActor = this;
		Weapon->AttachToComponent(this->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,WeaponSocket);
	}
	return  Weapon;
}

void ABaseEnemyAI::ReturnToIdle()
{
	eAIState = EAIStateEnum::IDLE;
	AAIController* AIController = Cast<AAIController>(GetController());
	AIController->GetBlackboardComponent()->SetValueAsBool("IsAttacked",false);
}

void ABaseEnemyAI::BeAttacked()
{
	eAIState = EAIStateEnum::ATTACKED;
}

void ABaseEnemyAI::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}
AActor* ABaseEnemyAI::GetTargetActor() const
{	
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}