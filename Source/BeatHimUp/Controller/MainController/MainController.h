// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/CustomHUD/CustomHUD.h"
#include "../../AttributeSet/AttributeSet_PlayableCharacter.h"
#include "MainController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainController();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties|HUDSubclass")
	TSubclassOf<UCustomHUD> MainHUDSubclass;

	UPROPERTY()
	UCustomHUD* MainHUD = nullptr;

	void BeginPlay() override;
	void OnPossess(APawn* aPawn) override;

	UFUNCTION()
	float GetPawnHealthPercentage() {
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetPawn())) {
			if (UAbilitySystemComponent* ASComp = ASI->GetAbilitySystemComponent()) {
				if (const UAttributeSet_PlayableCharacter* AS_PlayerableChar = Cast<UAttributeSet_PlayableCharacter>(ASComp->GetSet<UAttributeSet_PlayableCharacter>())) {
					return AS_PlayerableChar->GetHealthPercentage();
				}
			}
		}
		return 0.0f;
	}

	UFUNCTION()
	float GetPawnStaminaPercentage() {
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetPawn())) {
			if (UAbilitySystemComponent* ASComp = ASI->GetAbilitySystemComponent()) {
				if (const UAttributeSet_PlayableCharacter* AS_PlayerableChar = Cast<UAttributeSet_PlayableCharacter>(ASComp->GetSet<UAttributeSet_PlayableCharacter>())) {
					return AS_PlayerableChar->GetStaminaPercentage();
				}
			}
		}
		return 0.0f;
	}

	UFUNCTION(Client, Reliable)
	void AddHUD();
};
