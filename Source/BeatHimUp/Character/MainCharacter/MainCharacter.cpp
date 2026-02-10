// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "../../GameplayAbilities/GA_Move.h"
#include "../../GameplayAbilities/GA_Dead.h"
#include "../../CustomGameState/MainGameState.h"
#include "../../PlayerState/MainPlayerState.h"
#include "../../Controller/MainController/MainController.h"


AMainCharacter::AMainCharacter()
{
	this->PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	CineCameraComp = CreateDefaultSubobject<UCineCameraComponent>(FName("CineCameraComponent"));
	WidgetComp = CreateDefaultSubobject<UHealthbarWidgetComponent>(FName("HealthbarWidgetComponent"));
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	ItemComp = CreateDefaultSubobject<UItemComponent>(FName("ItemComponent"));
	CharacterAttributeSet = CreateDefaultSubobject<UAttributeSet_PlayableCharacter>("GameplayAttributeSet");
	if (SpringArmComp) {
		SpringArmComp->SetupAttachment(this->GetMesh());
		if (CineCameraComp) CineCameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
	}
	if (WidgetComp) {
		WidgetComp->SetOwnerNoSee(true);
		WidgetComp->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
	SetupStimulusSource();
}

void AMainCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (UAbilitySystemComponent* ASC = this->GetAbilitySystemComponent()) {
		TagContainer = ASC->GetOwnedGameplayTags();
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupMappingContext();

	SetupGameplay();

	if (!HasAuthority()) {
		if (IsValid(WidgetComp)) {
			WidgetComp->SetCustomOwner(this);
			WidgetComp->SetupHealthbarUI();
		}
	}

	if (GetLocalRole() == ENetRole::ROLE_Authority || GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
		OnLockTargetDel.AddUObject(this, &AMainCharacter::SetMovementAfterLockTarget);
	}
}

void AMainCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	
	if (!HasAuthority()) {
		RotateToLockTarget(deltaTime);
	}

	BillboardingWidgetCompByClient();
}

void AMainCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (IsLocallyControlled()) {
		if (EndPlayReason == EEndPlayReason::EndPlayInEditor || EndPlayReason == EEndPlayReason::Quit) {
			SaveCharacterStats();
		}
	}

	Super::EndPlay(EndPlayReason);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (AMainController* MainController = this->GetController<AMainController>()) {
			if (IsValid(IA_Move)) EIComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainCharacter::MoveTriggered);
			if (IsValid(IA_Look)) EIComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
			if (IsValid(IA_RightWeapon)) EIComponent->BindAction(IA_RightWeapon, ETriggerEvent::Triggered, this, &AMainCharacter::RightWeaponTriggered);
			if (IsValid(IA_RightWeapon)) EIComponent->BindAction(IA_RightWeapon, ETriggerEvent::Completed, this, &AMainCharacter::Server_RightWeaponCompleted);
			if (IsValid(IA_LeftWeapon)) EIComponent->BindAction(IA_LeftWeapon, ETriggerEvent::Triggered, this, &AMainCharacter::LeftWeaponTriggered);
			if (IsValid(IA_LeftWeapon)) EIComponent->BindAction(IA_LeftWeapon, ETriggerEvent::Completed, this, &AMainCharacter::Server_LeftWeaponCompleted);
			if (IsValid(IA_Dodge)) EIComponent->BindAction(IA_Dodge, ETriggerEvent::Triggered, this, &AMainCharacter::DodgeTriggered);
			if (IsValid(IA_LockTarget)) EIComponent->BindAction(IA_LockTarget, ETriggerEvent::Triggered, this, &AMainCharacter::Server_LockTargetTriggered);
			if (IsValid(IA_SwitchItem)) EIComponent->BindAction(IA_SwitchItem, ETriggerEvent::Triggered, this, &AMainCharacter::SwitchItemTriggered);
			if (IsValid(IA_UseItem)) EIComponent->BindAction(IA_UseItem, ETriggerEvent::Triggered, this, &AMainCharacter::UseItemTriggered);
			if (IsValid(IA_PauseGame)) EIComponent->BindAction(IA_PauseGame, ETriggerEvent::Triggered, MainController, &AMainController::PauseGame);
		}
	}
}

void AMainCharacter::SetupMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			EISubsystem->AddMappingContext(PlayerMappingContext, 1);
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

void AMainCharacter::Look(const FInputActionValue& value)
{
	if (!LockedOnTarget.IsValid()) {
		FVector lookDirectionVal = value.Get<FVector>();
		AddControllerYawInput(lookDirectionVal.X);
		AddControllerPitchInput(lookDirectionVal.Y);
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
	if (LockedOnTarget.IsValid()) {
		if (AActor* OldTargetActor = LockedOnTarget.Get()) {
			OldTargetActor->OnDestroyed.RemoveAll(this);
			if (IHaveSpecialDeath* HaveSpecialDeath = Cast<IHaveSpecialDeath>(OldTargetActor)) {
				HaveSpecialDeath->OnDeath().RemoveAll(this);
			}
		}
		LockedOnTarget = nullptr;
		if (OnLockTargetDel.IsBound()) {
			OnLockTargetDel.Broadcast(LockedOnTarget.Get());
		}
	}
	else {
		/*FHitResult Hit;
		FCollisionObjectQueryParams ObjectFilter;
		FCollisionQueryParams AdditionParams;
		ObjectFilter.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		AdditionParams.AddIgnoredActor(this);
		FVector CameraForwardDir = CineCameraComp->GetForwardVector();
		FRotator BoxRotation = CameraForwardDir.Rotation();
		FVector EndLocation = CineCameraComp->GetComponentLocation() + CameraForwardDir * 2000.0f;
		if (GetWorld()->SweepSingleByObjectType(Hit, CineCameraComp->GetComponentLocation() + CameraForwardDir * 1200.0f, EndLocation, BoxRotation.Quaternion(), ObjectFilter, FCollisionShape::MakeBox(DetectBoxExtent), AdditionParams)) {
			if (IsValid(Hit.GetActor())) {
				LockedOnTarget = Hit.GetActor();
				if (OnLockTargetDel.IsBound()) {
					OnLockTargetDel.Broadcast(LockedOnTarget.Get());
				}
			}
		}*/
		TArray<FHitResult> Hits;
		FCollisionObjectQueryParams ObjectFilter;
		FCollisionQueryParams AdditionParams;
		ObjectFilter.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		AdditionParams.AddIgnoredActor(this);
		FVector CameraForwardDir = CineCameraComp->GetForwardVector();
		FRotator BoxRotation = CameraForwardDir.Rotation();
		FVector EndLocation = CineCameraComp->GetComponentLocation() + CameraForwardDir * 2000.0f;
		if (GetWorld()->SweepMultiByObjectType(Hits, CineCameraComp->GetComponentLocation() + CameraForwardDir * 1200.0f, EndLocation, BoxRotation.Quaternion(), ObjectFilter, FCollisionShape::MakeBox(DetectBoxExtent), AdditionParams)) {
			for (int i = 0; i < Hits.Num(); ++i) {
				if (IsValid(Hits[i].GetActor()) && this->GetClass() != Hits[i].GetActor()->GetClass()) {
					LockedOnTarget = Hits[i].GetActor();
					if (AActor* TargetActor = LockedOnTarget.Get()) {
						TargetActor->OnDestroyed.AddDynamic(this, &AMainCharacter::OnLockedTargetDestroyed);
						if (IHaveSpecialDeath* HaveSpecialDeath = Cast<IHaveSpecialDeath>(TargetActor)) {
							HaveSpecialDeath->OnDeath().AddUObject(this, &AMainCharacter::OnLockedTargetDestroyed);
						}
					}
					if (OnLockTargetDel.IsBound()) {
						OnLockTargetDel.Broadcast(LockedOnTarget.Get());
					}
					return;
				}
			}
		}
	}
}

void AMainCharacter::RotateToLockTarget(float DeltaTime) 
{
	if (LockedOnTarget.IsValid()) {
		if (IsValid(CineCameraComp)) {
			FVector TargetLocation = LockedOnTarget->GetActorLocation();
			FRotator TargetRotation = (TargetLocation - CineCameraComp->GetComponentLocation()).Rotation();
			float TargetSocketOffsetY = 100.0f;
			//float TargetSocketOffsetZ = 200.0f;
			if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy) {
				FRotator TargetActorRotation = (TargetLocation - GetActorLocation()).Rotation();
				TargetActorRotation.Pitch = 0.f;
				TargetActorRotation.Roll = 0.f;
				FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetActorRotation, DeltaTime, 10.0f);
				SetActorRotation(NewRot);
			}
			else {
				if (APlayerController* PC = this->GetController<APlayerController>()) {
					TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch, -30.0f, 30.0f);
					FRotator NewRot = FMath::RInterpTo(GetControlRotation(), TargetRotation, DeltaTime, 10.0f);
					PC->SetControlRotation(NewRot);
				}
			}
		}
	}
}

void AMainCharacter::SwitchItemTriggered()
{
	if (IsValid(ItemComp)) {
		ItemComp->SwitchUsableItem();
	}
}

void AMainCharacter::UseItemTriggered()
{
	if (AbilitySystemComp) {
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.UseItem")));
		AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AMainCharacter::BillboardingWidgetCompByClient()
{
	if (GetNetMode() == ENetMode::NM_DedicatedServer) return;
	if (IsLocallyControlled()) return;
	if (IsValid(WidgetComp)) {
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()) {
			FRotator CameraRotation;
			FVector CameraLocation;
			FVector WidgetLocation = WidgetComp->GetComponentLocation();
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
			FRotator TargetRotation = (CameraLocation - WidgetLocation).Rotation();
			WidgetComp->SetWorldRotation(TargetRotation);
		}
	}
}

void AMainCharacter::SetupGameplay()
{
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

			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_UseItem")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_UseItem")), 1, -1, this));

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

void AMainCharacter::Hurt_Implementation(const float& remainHealth, const float& totalHealth, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AbilitySystemComp) {
		if (FMath::IsNearlyEqual(remainHealth, 0.0f, 1.0E-4)) {
			if (IsValid(GADataAsset) && GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead"))) {
				if (FGameplayAbilitySpec* GASpec = AbilitySystemComp->FindAbilitySpecFromClass(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead")))) {
					if (UGA_Dead* GA_Dead = Cast<UGA_Dead>(GASpec->GetPrimaryInstance())) {
						GA_Dead->SetupDeadInfo(EventInstigator, DamageCauser);
						AbilitySystemComp->TryActivateAbility(GASpec->Handle);
					}
				}
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

void AMainCharacter::OnRep_PlayerState()
{
	if (AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>()) {
		if (IsValid(WidgetComp)) {
			MainPlayerState->OnPlayerInfoRepDel.AddUObject(WidgetComp, &UHealthbarWidgetComponent::SetHeadInfo);
			if (MainPlayerState->GetPlayerInfo()) {
				WidgetComp->SetHeadInfo(*MainPlayerState->GetPlayerInfo());
			}
		}
	}
}

void AMainCharacter::OnRep_LockedOnTarget(TWeakObjectPtr<AActor> OldTarget)
{
	Super::OnRep_LockedOnTarget(OldTarget);

	if (OnLockTargetDel.IsBound()) {
		OnLockTargetDel.Broadcast(LockedOnTarget.Get());
	}
}

void AMainCharacter::ExecuteAfterDeathBehaviour(AController* inInstigator, AActor* DamageCauser)
{
	if (OnDeathDel.IsBound()) {
		OnDeathDel.Broadcast(this);
	}
	if (AMainController* MainController = this->GetController<AMainController>()) {
		if (IsLocallyControlled()) {
			SaveCharacterStats();
			if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MainController->GetLocalPlayer())) {
				EISubsystem->RemoveMappingContext(PlayerMappingContext);
			}
		}
		if (HasAuthority()) {
			if (AActor* OldTargetActor = LockedOnTarget.Get()) {
				OldTargetActor->OnDestroyed.RemoveAll(this);
				if (IHaveSpecialDeath* HaveSpecialDeath = Cast<IHaveSpecialDeath>(OldTargetActor)) {
					HaveSpecialDeath->OnDeath().RemoveAll(this);
				}
			}
			LockedOnTarget = nullptr;
			if (OnLockTargetDel.IsBound()) {
				OnLockTargetDel.Broadcast(LockedOnTarget.Get());
			}

			if (AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>()) {
				MainGameState->OnPlayerKilled(this, inInstigator, DamageCauser);
			}
		}
		MainController->SwitchToSpectate();
	}
}

void AMainCharacter::SetMovementAfterLockTarget(AActor* Target)
{
	if (UCharacterMovementComponent* CharMovementComponent = GetCharacterMovement()) {
		if (IsValid(Target)) {
			CharMovementComponent->MaxWalkSpeed = 250.0f;
			CharMovementComponent->bOrientRotationToMovement = false;
			CharMovementComponent->bUseControllerDesiredRotation = true;
			bUseControllerRotationYaw = true;
		}
		else {
			CharMovementComponent->MaxWalkSpeed = 400.0f;
			CharMovementComponent->bOrientRotationToMovement = true;
			CharMovementComponent->bUseControllerDesiredRotation = false;
			bUseControllerRotationYaw = false;
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

void AMainCharacter::SaveCharacterStats()
{
	float MaxHP = 0.f, HP = 0.f, MaxStamina = 0.f;
	int HealthPotionQuantity = 0;
	FString StateStr = TEXT("");
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(GetAttributeSet())) {
		MaxHP = HaveHealthAttr->GetBaseMaxHealth();
		HP = HaveHealthAttr->GetCurrentBaseHealth();
	}

	if (IHaveStaminaAttribute* HaveStaminaAttr = Cast<IHaveStaminaAttribute>(GetAttributeSet())) {
		MaxStamina = HaveStaminaAttr->GetBaseMaxStamina();
	}

	if (IsValid(ItemComp)) {
		if (UUsableItem* HealthPotion = Cast<UUsableItem>(ItemComp->GetItemByName(FName("HealthPotion")))) {
			HealthPotionQuantity = HealthPotion->GetQuantity();
		}
	}

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent()) {
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
			StateStr = TEXT("State.Dead");
		}
	}

	JsonObj->SetNumberField(TEXT("max_hp"), MaxHP);
	JsonObj->SetNumberField(TEXT("hp"), HP);
	JsonObj->SetNumberField(TEXT("max_stamina"), MaxStamina);
	JsonObj->SetNumberField(TEXT("health_potion_quant"), HealthPotionQuantity);
	JsonObj->SetStringField(TEXT("state"), StateStr);

	if (AMainController* MC = this->GetController<AMainController>()) {
		MC->SaveCharacterStats(JsonObj);
	}
}

