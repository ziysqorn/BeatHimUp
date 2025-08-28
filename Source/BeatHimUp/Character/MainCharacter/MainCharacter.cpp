// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	this->PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(FName("AttackComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(FName("WeaponComponent"));
	if (SpringArmComp) {
		SpringArmComp->SetupAttachment(this->RootComponent);
		if (CameraComp) CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupMappingContext();

	AttackComponent->SetAttackStrategy(NewObject<USwordShieldAttack>());


	WeaponComponent->SetupWeaponsOnHands(
		*WeaponComponent->GetWeaponSubclassByName(FName("Shield")),
		*WeaponComponent->GetWeaponSubclassByName(FName("Sword")),
		FName("LeftHand_Shield"), 
		FName("RightHand_Weapon")
	);
}

void AMainCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (IA_Move) EIComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainCharacter::MoveTriggered);
		if (IA_Look) EIComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
	}
}

void AMainCharacter::SetupMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			EISubsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void AMainCharacter::MoveTriggered(const FInputActionValue& value)
{
	FVector directionValue = value.Get<FVector>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(DirectionY, directionValue.Y);
	AddMovementInput(DirectionX, directionValue.X);
}

void AMainCharacter::Look(const FInputActionValue& value)
{
	FVector lookDirectionVal = value.Get<FVector>();
	AddControllerYawInput(lookDirectionVal.X);
	AddControllerPitchInput(lookDirectionVal.Y);
}


