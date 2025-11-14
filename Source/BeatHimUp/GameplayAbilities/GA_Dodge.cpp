// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dodge.h"
#include "../Character/BaseCharacter/BaseCharacter.h"

UGA_Dodge::UGA_Dodge()
{
	MovementInputDodgeMap.Empty();
	MovementInputDodgeMap.Add({ 0, 0 }, FName("Dodge_Backward"));
	MovementInputDodgeMap.Add({ 1, 0 }, FName("Dodge_Forward"));
	MovementInputDodgeMap.Add({ -1, 0 }, FName("Dodge_Backward"));
	MovementInputDodgeMap.Add({ 0, 1 }, FName("Dodge_Right"));
	MovementInputDodgeMap.Add({ 0, -1 }, FName("Dodge_Left"));
	MovementInputDodgeMap.Add({ 1, 1 }, FName("Dodge_RightForward"));
	MovementInputDodgeMap.Add({ 1, -1 }, FName("Dodge_LeftForward"));
	MovementInputDodgeMap.Add({ -1, 1 }, FName("Dodge_RightBack"));
	MovementInputDodgeMap.Add({ -1, -1 }, FName("Dodge_LeftBack"));
}

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(ActorInfo->OwnerActor.Get())) {
			if (!BaseCharacter->GetLockedOnTarget().IsValid()) {
				Client_RequestRotateOwnerAndDodge();
			}
			else {
				Client_RequestDirectionalDodgeMontageAndPlay();
			}
		}
	}
}

void UGA_Dodge::DodgeEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Dodge::Server_RotateOwnerByMovementInputAndDodge_Implementation(FVector LastInputMovementVector)
{
	if (CurrentActorInfo) {
		if (ACharacter* character = Cast<ACharacter>(CurrentActorInfo->OwnerActor)) {
			if (LastInputMovementVector.X == 0.0f && LastInputMovementVector.Y == 0.0f) {
				Server_PlayDodgeTask(FName("Dodge_Backward"));
				Client_PlayDodgeTask(FName("Dodge_Backward"));
			}
			else {
				character->SetActorRotation(LastInputMovementVector.Rotation());
				Server_PlayDodgeTask(FName("Dive_Forward"));
				Client_PlayDodgeTask(FName("Dive_Forward"));
			}
		}
	}
}

void UGA_Dodge::Server_ChooseAndPlayDodgeMontage_Implementation(FVector2D LastInputMovementVector)
{

	int8 DirectionX = 0, DirectionY = 0;
	if (LastInputMovementVector.X > 0) DirectionX = 1;
	else if (LastInputMovementVector.X < 0) DirectionX = -1;

	if (LastInputMovementVector.Y > 0) DirectionY = 1;
	else if (LastInputMovementVector.Y < 0) DirectionY = -1;

	FName DodgeMontageName = *MovementInputDodgeMap.Find({ DirectionX, DirectionY });
	Server_PlayDodgeTask(DodgeMontageName);
	Client_PlayDodgeTask(DodgeMontageName);
}

void UGA_Dodge::Client_RequestRotateOwnerAndDodge_Implementation()
{
	if (CurrentActorInfo) {
		if (ACharacter* character = Cast<ACharacter>(CurrentActorInfo->OwnerActor)) {
			Server_RotateOwnerByMovementInputAndDodge(character->GetLastMovementInputVector());
		}
	}
}

void UGA_Dodge::Client_RequestDirectionalDodgeMontageAndPlay_Implementation()
{
	if (CurrentActorInfo) {
		if (ACharacter* character = Cast<ACharacter>(CurrentActorInfo->OwnerActor)) {
			if (APlayerController* PlayerController = character->GetController<APlayerController>()) {
				if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
					if (UEnhancedPlayerInput* PlayerInput = EISubsystem->GetPlayerInput()) {
						if (IsValid(IA_Move)) {
							const FInputActionValue ActionValue = PlayerInput->GetActionValue(IA_Move);
							Server_ChooseAndPlayDodgeMontage(ActionValue.Get<FVector2D>());
						}
					}
				}
			}
		}
	}
}

void UGA_Dodge::Client_PlayDodgeTask_Implementation(FName DodgeMontageName)
{
	if (IsValid(DA_DodgeMontage)) {
		TObjectPtr<UAnimMontage> DodgeMontage = *DA_DodgeMontage->DodgeMontagesMap.Find(DodgeMontageName);
		if (IsValid(DodgeMontage)) {
			if (UAbilityTask_PlayMontageAndWait* PlayDodgeMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayDodgeMontageAndWait"), DodgeMontage)) {
				PlayDodgeMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Dodge::DodgeEnd);
				PlayDodgeMontageAndWait->ReadyForActivation();
			}
		}
	}
}

void UGA_Dodge::Server_PlayDodgeTask_Implementation(FName DodgeMontageName)
{
	if (IsValid(DA_DodgeMontage)) {
		TObjectPtr<UAnimMontage> DodgeMontage = *DA_DodgeMontage->DodgeMontagesMap.Find(DodgeMontageName);
		if (IsValid(DodgeMontage)) {
			if (UAbilityTask_PlayMontageAndWait* PlayDodgeMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayDodgeMontageAndWait"), DodgeMontage)) {
				PlayDodgeMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Dodge::DodgeEnd);
				PlayDodgeMontageAndWait->ReadyForActivation();
			}
		}
	}
}

bool UGA_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return CheckCost(Handle, ActorInfo);
}
