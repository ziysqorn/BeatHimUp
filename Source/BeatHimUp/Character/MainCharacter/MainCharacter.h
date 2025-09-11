// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "../../ActorComponent/AttackComponent/AttackComponent.h"
#include "../../AttributeSet/AttributeSet_PlayableCharacter.h"
#include "../../DataAsset/GameplayAbilityDataAsset.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainCharacter : public ABaseCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMainCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComp;
	}
protected:
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Camera|SpringArmComponent")
	USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Camera|CameraComponent")
	UCameraComponent* CameraComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|AttackComponent")
	UAttackComponent* AttackComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|AbilitySystemComponent")
	UAbilitySystemComponent* AbilitySystemComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAttributeSet")
	UAttributeSet_PlayableCharacter* CharacterAttributeSet = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties|DataAssets|GameplayAbilityDataAsset")
	UGameplayAbilityDataAsset* GADataAsset = nullptr;

	//Editor Properties
	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input")
	UInputMappingContext* PlayerMappingContext = nullptr;


	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_Move = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_Look = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_Attack = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetupMappingContext();

	void MoveTriggered(const FInputActionValue& value);

	void AttackTriggered();

	void Look(const FInputActionValue& value);
};
