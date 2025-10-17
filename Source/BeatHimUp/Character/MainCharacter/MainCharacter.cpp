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
	CharacterAttributeSet = CreateDefaultSubobject<UAttributeSet_PlayableCharacter>("GameplayAttributeSet");
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, GetControlRotation().ToString());
}

void AMainCharacter::Hurt(const float& remainHealth, const float& totalHealth)
{
	NetMulticast_Hurt(remainHealth, totalHealth);
}

void AMainCharacter::NetMulticast_Hurt_Implementation(const float& remainHealth, const float& totalHealth)
{
	if (AbilitySystemComp) {
		if (IsValid(HumanoidMontagesDataAsset)) {
			if (USkeletalMeshComponent* mesh = GetMesh()) {
				if (UAnimInstance* animInstance = mesh->GetAnimInstance()) {
					if (FMath::IsNearlyEqual(remainHealth, 0.0f, 1.0E-4)) {
						AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
						GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "Character died");
					}
					else
					{
						AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Hurt")));
						UAnimMontage* montageToPlay = *HumanoidMontagesDataAsset->HumanoidMontagesMap.Find(FName("Hurt"));
						animInstance->Montage_Play(montageToPlay);
						FOnMontageEnded montageEndedDel = FOnMontageEnded::CreateUObject(this, &AMainCharacter::HurtMontageEnded);
						animInstance->Montage_SetEndDelegate(montageEndedDel, montageToPlay);
						GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "Character hurt");
					}
				}
			}
		}
	}
}

void AMainCharacter::HurtMontageEnded(UAnimMontage* Montage, bool isInterrupted)
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, "HurtMontageEnded");
	AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Hurt")));
}

