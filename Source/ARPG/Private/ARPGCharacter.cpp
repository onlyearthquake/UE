
#include "ARPGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AttributeComponent.h"
#include "Blueprint/UserWidget.h"
#include "WorldWidget.h"
#include "Weapon/Weapon.h"

AARPGCharacter::AARPGCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; 	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 
	
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));
	AttributeComp->OnHealthChange.AddDynamic(this, &AARPGCharacter::OnHealthChanged);
	AttributeComp->Faction = EFactionKind::Player;
	
	Delegate.BindUFunction(this, "ReturnToIdle");
}

void AARPGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AARPGCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Weapon->Destroy();
}

void AARPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AARPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARPGCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Roll",IE_Pressed,this,&AARPGCharacter::Roll);
	PlayerInputComponent->BindAction("Rush",IE_Pressed,this,&AARPGCharacter::Rush);
	PlayerInputComponent->BindAction("Rush",IE_Released,this,&AARPGCharacter::StopRush);
	PlayerInputComponent->BindAction("Attack",IE_Pressed,this,&AARPGCharacter::Attack);
	CDMap.Add("Attack",GetGameTimeSinceCreation());
	CDMap.Add("Dodge",GetGameTimeSinceCreation());
}

void AARPGCharacter::Roll()
{
	if(eStateEnum != EStateEnum::IDLE)
		return;
	float OldTime = CDMap["Dodge"];
	if(GetGameTimeSinceCreation() - OldTime <= 1.0f)
	{
		return;
	}
	CDMap["Dodge"] = GetGameTimeSinceCreation();
	eStateEnum = EStateEnum::DODGE;
	this->SetCanBeDamaged(false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_RecoverToIdle, Delegate, 0.5f, false);
}

void AARPGCharacter::Rush()
{
	SpeedValue = 1.5f;
}

void AARPGCharacter::StopRush()
{
	SpeedValue = 0.5f;
}

void AARPGCharacter::Attack()
{
	if(eStateEnum == EStateEnum::ATTACK||eStateEnum==EStateEnum::ATTACKED)
	{
		return;
	}
	//TODO:寻敌
	Weapon->Attack(nullptr);
	
	eStateEnum = EStateEnum::ATTACK;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_RecoverToIdle, Delegate, 0.5f, false);
}

void AARPGCharacter::ReturnToIdle()
{
	eStateEnum = EStateEnum::IDLE;
	this->SetCanBeDamaged(true);
}

void AARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	ActiveHealthBar = CreateWidget<UWorldWidget>(GetWorld(), HealthBarClass);
	if (ActiveHealthBar)
	{
		ActiveHealthBar->AttachedActor = this;
		ActiveHealthBar->AddToViewport();
	}
	
	ActiveManaBar = CreateWidget<UWorldWidget>(GetWorld(), ManaBarClass);
	if (ActiveManaBar)
	{
		ActiveManaBar->AttachedActor = this;
		ActiveManaBar->AddToViewport();
	}
	
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	Weapon->OwnActor = this;
	Weapon->AttachToComponent(this->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"hand_rSocket");
}

void AARPGCharacter::MoveForward(float Value)
{
	if(eStateEnum != EStateEnum::IDLE)
	{
		return;
	}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
	
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value * SpeedValue);
	}
}

void AARPGCharacter::MoveRight(float Value)
{
	if(eStateEnum != EStateEnum::IDLE)
	{
		return;
	}
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AARPGCharacter::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// Damaged
	if (Delta < 0.0f)
	{
		eStateEnum = EStateEnum::ATTACKED;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_RecoverToIdle, Delegate, 0.4f, false);
		//TODO::受击特效加入
	}

	// Die
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}