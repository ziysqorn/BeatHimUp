// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AAICharacter : public ABaseCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAICharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComp;
	}

	UBehaviorTree* GetBehaviorTree() const {
		return AIBehaviorTree;
	}
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|AbilitySystemComponent")
	UAbilitySystemComponent* AbilitySystemComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties|DataAssets|GameplayAbilityDataAsset")
	UGameplayAbilityDataAsset* GADataAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BehaviorTree")
	UBehaviorTree* AIBehaviorTree = nullptr;

	virtual void BeginPlay() override;
};
