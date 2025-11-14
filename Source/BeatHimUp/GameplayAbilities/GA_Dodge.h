// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "../DataAsset/DodgeMontagesDataAsset.h"
#include "GA_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_Dodge : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Dodge();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DodgeMontageMap")
	TObjectPtr<UDodgeMontagesDataAsset> DA_DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction | Move")
	TObjectPtr<UInputAction> IA_Move;

	TMap<TPair<int8, int8>, FName> MovementInputDodgeMap;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void DodgeEnd();

	UFUNCTION(Server, Unreliable)
	void Server_RotateOwnerByMovementInputAndDodge(FVector LastInputMovementVector);

	UFUNCTION(Server, Unreliable)
	void Server_ChooseAndPlayDodgeMontage(FVector2D LastInputMovementVector);

	UFUNCTION(Client, Unreliable)
	void Client_RequestRotateOwnerAndDodge();

	UFUNCTION(Client, Unreliable)
	void Client_RequestDirectionalDodgeMontageAndPlay();

	UFUNCTION(Server, Unreliable)
	void Server_PlayDodgeTask(FName DodgeMontageName);

	UFUNCTION(Client, Unreliable)
	void Client_PlayDodgeTask(FName DodgeMontageName);

	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
