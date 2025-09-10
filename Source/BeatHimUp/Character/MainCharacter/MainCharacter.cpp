// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "../../GameplayAbilities/GA_Move.h"


AMainCharacter::AMainCharacter()
{
	this->PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(FName("AttackComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(FName("WeaponComponent"));
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	if (SpringArmComp) {
		SpringArmComp->SetupAttachment(this->RootComponent);
		if (CameraComp) CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupMappingContext();


	if (WeaponComponent) {
		WeaponComponent->SetupWeaponsOnHands(
			*WeaponComponent->GetWeaponSubclassByName(FName("Shield")),
			*WeaponComponent->GetWeaponSubclassByName(FName("Sword")),
			FName("LeftHand_Shield"),
			FName("RightHand_Weapon")
		);
	}
	if (AbilitySystemComp && GADataAsset) {
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		AbilitySystemComp->AffectedAnimInstanceTag = NAME_None;
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_SwordAndShieldAttack")), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove")), 1, -1, this));
	}
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
		if (IA_Attack) EIComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AMainCharacter::AttackTriggered);
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
	if (AbilitySystemComp) {
		if (FGameplayAbilitySpec* GASpec = AbilitySystemComp->FindAbilitySpecFromClass(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove")))) {
			if (UGA_Move* GA_Move = Cast<UGA_Move>(GASpec->GetPrimaryInstance())) {
				FVector directionValue = value.Get<FVector>();
				GA_Move->SetInputDirectionValue(directionValue);

				AbilitySystemComp->TryActivateAbility(GASpec->Handle);
			}
		}
	}
}

void AMainCharacter::AttackTriggered()
{
	if (AbilitySystemComp) {
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Attack")));
		AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AMainCharacter::Look(const FInputActionValue& value)
{
	FVector lookDirectionVal = value.Get<FVector>();
	AddControllerYawInput(lookDirectionVal.X);
	AddControllerPitchInput(lookDirectionVal.Y);
}


