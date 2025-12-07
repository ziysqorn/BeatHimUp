// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "../../GameplayAbilities/GA_Move.h"


AMainCharacter::AMainCharacter()
{
	this->PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	CineCameraComp = CreateDefaultSubobject<UCineCameraComponent>(FName("CineCameraComponent"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(FName("AttackComponent"));
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	CharacterAttributeSet = CreateDefaultSubobject<UAttributeSet_PlayableCharacter>("GameplayAttributeSet");
	if (SpringArmComp) {
		SpringArmComp->SetupAttachment(this->RootComponent);
		if (CineCameraComp) CineCameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
	}
	SetupStimulusSource();
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupMappingContext();

	if (HasAuthority()) {
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
			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove")), 1, -1, this));

			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dodge")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dodge")), 1, -1, this));

			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Hurt")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Hurt")), 1, -1, this));

			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead")), 1, -1, this));

			if (AWeapon* RightWeapon = WeaponComponent->GetRightWeapon()) {
				if (GADataAsset->GameplayAbilitySubclassMap.Find(RightWeapon->GetAbilitySubclass())) {
					AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(RightWeapon->GetAbilitySubclass()), 1, -1, RightWeapon));
				}
			}

			if (AWeapon* LeftWeapon = WeaponComponent->GetLeftWeapon()) {
				if (GADataAsset->GameplayAbilitySubclassMap.Find(LeftWeapon->GetAbilitySubclass())) {
					AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(LeftWeapon->GetAbilitySubclass()), 1, -1, LeftWeapon));
				}
			}
		}
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
		if (IsValid(IA_RightWeapon)) EIComponent->BindAction(IA_RightWeapon, ETriggerEvent::Triggered, this, &AMainCharacter::RightWeaponTriggered);
		if (IsValid(IA_RightWeapon)) EIComponent->BindAction(IA_RightWeapon, ETriggerEvent::Completed, this, &AMainCharacter::Server_RightWeaponCompleted);
		if (IsValid(IA_LeftWeapon)) EIComponent->BindAction(IA_LeftWeapon, ETriggerEvent::Triggered, this, &AMainCharacter::LeftWeaponTriggered);
		if (IsValid(IA_LeftWeapon)) EIComponent->BindAction(IA_LeftWeapon, ETriggerEvent::Completed, this, &AMainCharacter::Server_LeftWeaponCompleted);
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
		if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove"))) {
			if (FGameplayAbilitySpec* GASpec = AbilitySystemComp->FindAbilitySpecFromClass(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove")))) {
				if (UGA_Move* GA_Move = Cast<UGA_Move>(GASpec->GetPrimaryInstance())) {
					FVector directionValue = value.Get<FVector>();
					GA_Move->SetInputDirectionValue(directionValue);

					AbilitySystemComp->TryActivateAbility(GASpec->Handle);
				}
			}
		}
	}
}

void AMainCharacter::RightWeaponTriggered()
{
	if (WeaponComponent && AbilitySystemComp) {
		if (AWeapon* RightWeapon = WeaponComponent->GetRightWeapon()) {
			if (GADataAsset->GameplayAbilitySubclassMap.Find(RightWeapon->GetAbilitySubclass())) {
				AbilitySystemComp->TryActivateAbilityByClass(*GADataAsset->GameplayAbilitySubclassMap.Find(RightWeapon->GetAbilitySubclass()));
			}
		}
	}
}


void AMainCharacter::Server_RightWeaponCompleted_Implementation()
{
}

void AMainCharacter::LeftWeaponTriggered()
{
	if (WeaponComponent && AbilitySystemComp) {
		if (AWeapon* LeftWeapon = WeaponComponent->GetLeftWeapon()) {
			if (GADataAsset->GameplayAbilitySubclassMap.Find(LeftWeapon->GetAbilitySubclass())) {
				AbilitySystemComp->TryActivateAbilityByClass(*GADataAsset->GameplayAbilitySubclassMap.Find(LeftWeapon->GetAbilitySubclass()));
			}
		}
	}
}

void AMainCharacter::Server_LeftWeaponCompleted_Implementation()
{
	if (WeaponComponent && AbilitySystemComp) {
		if (AWeapon* LeftWeapon = WeaponComponent->GetLeftWeapon()) {
			LeftWeapon->CancelWeaponAbility();
		}
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
			FRotator BoxRotation = CameraForwardDir.Rotation();
			FVector EndLocation = CineCameraComp->GetComponentLocation() + CameraForwardDir * 1200.0f + CineCameraComp->GetComponentLocation() + CameraForwardDir * 2000.0f;
			if (GetWorld()->SweepSingleByObjectType(Hit, CineCameraComp->GetComponentLocation() + CameraForwardDir * 1200.0f, EndLocation, BoxRotation.Quaternion(), ObjectFilter, FCollisionShape::MakeBox(DetectBoxExtent), AdditionParams)) {
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

void AMainCharacter::Hurt_Implementation(const float& remainHealth, const float& totalHealth, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AbilitySystemComp) {
		if (FMath::IsNearlyEqual(remainHealth, 0.0f, 1.0E-4)) {
			if (IsValid(GADataAsset) && GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead"))) {
				AbilitySystemComp->TryActivateAbilityByClass(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead")));
			}
		}
		else
		{
			FVector VecFromThisToDamageCauser = DamageCauser->GetActorLocation() - this->GetActorLocation();
			float dotProduct = FVector::DotProduct(this->GetActorForwardVector(), VecFromThisToDamageCauser);
			if (dotProduct < 0) {
				FGameplayTagContainer tagContainer;
				tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Block")));
				AbilitySystemComp->CancelAbilities(&tagContainer);
			}
			if (IsValid(GADataAsset) && GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Hurt"))) {
				AbilitySystemComp->TryActivateAbilityByClass(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Hurt")));
			}
		}
	}
}


void AMainCharacter::SetupStimulusSource()
{
	StimulusSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimulusSourceComp"));
	if (IsValid(StimulusSourceComp)) {
		StimulusSourceComp->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSourceComp->RegisterWithPerceptionSystem();
	}
}

