// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "../../GameplayAbilities/GA_Move.h"


AMainCharacter::AMainCharacter()
{
	this->PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	CineCameraComp = CreateDefaultSubobject<UCineCameraComponent>(FName("CineCameraComponent"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(FName("AttackComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(FName("WeaponComponent"));
	HitStopComp = CreateDefaultSubobject<UHitStopComponent>(FName("HitStopComponent"));
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	CharacterAttributeSet = CreateDefaultSubobject<UAttributeSet_PlayableCharacter>("GameplayAttributeSet");
	if (SpringArmComp) {
		SpringArmComp->SetupAttachment(this->RootComponent);
		if (CineCameraComp) CineCameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
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
	if (HasAuthority() && AbilitySystemComp && GADataAsset) {
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		AbilitySystemComp->AffectedAnimInstanceTag = NAME_None;
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_SwordAndShieldAttack")), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove")), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dodge")), 1, -1, this));
	}
}

void AMainCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	RotateToLockTarget(deltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (IsValid(IA_Move)) EIComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainCharacter::MoveTriggered);
		if (IsValid(IA_Look)) EIComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		if (IsValid(IA_Attack)) EIComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AMainCharacter::AttackTriggered);
		if (IsValid(IA_Dodge)) EIComponent->BindAction(IA_Dodge, ETriggerEvent::Triggered, this, &AMainCharacter::DodgeTriggered);
		if (IsValid(IA_LockTarget)) EIComponent->BindAction(IA_LockTarget, ETriggerEvent::Triggered, this, &AMainCharacter::Server_LockTargetTriggered);
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

void AMainCharacter::DodgeTriggered()
{
	if (AbilitySystemComp) {
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Dodge")));
		AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AMainCharacter::Server_LockTargetTriggered_Implementation()
{
	NetMulticast_LockTargetTriggered();
}

void AMainCharacter::NetMulticast_LockTargetTriggered_Implementation()
{
	if (UCharacterMovementComponent* CharMovementComponent = GetCharacterMovement()) {
		if (LockedOnTarget.IsValid()) {
			LockedOnTarget = nullptr;
			CharMovementComponent->MaxWalkSpeed = 400.0f;
			CharMovementComponent->bOrientRotationToMovement = true;
			CharMovementComponent->bUseControllerDesiredRotation = false;
			this->bUseControllerRotationYaw = false;
		}
		else {
			FHitResult Hit;
			FCollisionObjectQueryParams ObjectFilter;
			FCollisionQueryParams AdditionParams;
			ObjectFilter.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
			AdditionParams.AddIgnoredActor(this);
			FVector CameraForwardDir = CineCameraComp->GetForwardVector();
			if (GetWorld()->SweepSingleByObjectType(Hit, CineCameraComp->GetComponentLocation() + CameraForwardDir * 1200.0f, CineCameraComp->GetComponentLocation() + CameraForwardDir * 5000.0f, FQuat(0, 0, 45.0, 0), ObjectFilter, FCollisionShape::MakeBox(DetectBoxExtent), AdditionParams)) {
				if (IsValid(Hit.GetActor())) {
					LockedOnTarget = Hit.GetActor();
					CharMovementComponent->MaxWalkSpeed = 250.0f;
					CharMovementComponent->bOrientRotationToMovement = false;
					CharMovementComponent->bUseControllerDesiredRotation = true;
					this->bUseControllerRotationYaw = true;
				}
			}
		}
	}
}

void AMainCharacter::RotateToLockTarget(float DeltaTime)
{
	if (LockedOnTarget.IsValid()) {
		FVector TargetLocation = LockedOnTarget->GetActorLocation();
		FRotator TargetRotation = (TargetLocation - this->GetActorLocation()).Rotation();
		float TargetSocketOffsetY = 100.0f;
		if (APlayerController* PC = this->GetController<APlayerController>()) {
			FRotator NewRot = FMath::RInterpTo(GetControlRotation(), TargetRotation, DeltaTime, 10.0f);
			PC->SetControlRotation(NewRot);
			if (IsValid(SpringArmComp)) SpringArmComp->SocketOffset.Y = FMath::FInterpTo(SpringArmComp->SocketOffset.Y, TargetSocketOffsetY, DeltaTime, 10.0f);
		}
	}
	else {
		if (IsValid(SpringArmComp)) SpringArmComp->SocketOffset.Y = FMath::FInterpTo(SpringArmComp->SocketOffset.Y, 0.0f, DeltaTime, 1.0f);
	}
}

void AMainCharacter::Look(const FInputActionValue& value)
{
	if (!LockedOnTarget.IsValid()) {
		FVector lookDirectionVal = value.Get<FVector>();
		AddControllerYawInput(lookDirectionVal.X);
		AddControllerPitchInput(lookDirectionVal.Y);
	}
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
						//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "Character died");
					}
					else
					{
						AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Hurt")));
						UAnimMontage* montageToPlay = *HumanoidMontagesDataAsset->HumanoidMontagesMap.Find(FName("Hurt"));
						animInstance->Montage_Play(montageToPlay);
						FOnMontageEnded montageEndedDel = FOnMontageEnded::CreateUObject(this, &AMainCharacter::HurtMontageEnded);
						animInstance->Montage_SetEndDelegate(montageEndedDel, montageToPlay);
						//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "Character hurt");
					}
				}
			}
		}
	}
}

void AMainCharacter::HurtMontageEnded(UAnimMontage* Montage, bool isInterrupted)
{
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, "HurtMontageEnded");
	AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Hurt")));
}

