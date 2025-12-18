// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "../../Controller/MainController/MainController.h"
#include "../../AttributeSet/AttributeSet_PlayableCharacter.h"
#include "../../CustomComponents/HealthbarWidgetComponent.h"
#include "../../Interface/CameraSystemInterface.h"
#include "../../Interface/Damageable.h"
#include "../../Interface/CanCauseDamage.h"
#include "../../Interface/CanUseItem.h"
#include "../../Interface/HaveAttributeSet.h"
#include "../../Interface/HaveSpecialDeath.h"
#include "MainCharacter.generated.h"

/**
 * 
 */

UCLASS()
class BEATHIMUP_API AMainCharacter : public ABaseCharacter, public IAbilitySystemInterface, public ICameraSystemInterface, public IDamageable, public ICanCauseDamage, public ICanUseItem, public IHaveAttributeSet, public IHaveSpecialDeath
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMainCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComp;
	}

	USpringArmComponent* GetSpringArmComp() override {
		return SpringArmComp;
	}

	UCineCameraComponent* GetCineCameraComp() override {
		return CineCameraComp;
	}

	UItemComponent* GetItemComponent() override {
		return ItemComp;
	}
protected:
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Camera|SpringArmComponent")
	USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Camera|CineCameraComponent")
	UCineCameraComponent* CineCameraComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|WidgetComponent")
	UHealthbarWidgetComponent* WidgetComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|AbilitySystemComponent")
	UAbilitySystemComponent* AbilitySystemComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|ItemComponent")
	UItemComponent* ItemComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAttributeSet")
	UAttributeSet_PlayableCharacter* CharacterAttributeSet = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties|DataAssets|GameplayAbilityDataAsset")
	UGameplayAbilityDataAsset* GADataAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage GE Subclass")
	TSubclassOf<UGameplayEffect> DamageGESubclass;

	//Editor Properties
	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input")
	UInputMappingContext* PlayerMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input")
	UInputMappingContext* MC_SpectatorMode = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_Move = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_Look = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_RightWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_LeftWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_Dodge = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_LockTarget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_SwitchItem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_UseItem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_NextSpectatedPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties | Detect Box Extent")
	FVector DetectBoxExtent;

	FOnDeath OnDeathDel;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetupMappingContext();

	void MoveTriggered(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void RightWeaponTriggered();

	UFUNCTION(Server, Reliable)
	void Server_RightWeaponCompleted();

	void DodgeTriggered();

	void LeftWeaponTriggered();

	UFUNCTION(Server, Reliable)
	void Server_LeftWeaponCompleted();

	UFUNCTION(Server, Unreliable)
	void Server_LockTargetTriggered();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_LockTarget();

	void RotateToLockTarget(float DeltaTime);

	void SwitchItemTriggered();

	void UseItemTriggered();

	void BillboardingWidgetCompByClient();

	void SetupGameplay();

	UFUNCTION(NetMulticast, Reliable)
	void Hurt(const float& remainHealth, const float& totalHealth, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	TSubclassOf<UGameplayEffect> GetDamageGESubclass() override {
		return DamageGESubclass;
	}

	UAttributeSet* GetAttributeSet() override {
		return CharacterAttributeSet;
	}

	float GetDeathDelay() override {
		return 3.f;
	}

	FOnDeath& OnDeath() override {
		return OnDeathDel;
	}

	void ExecuteAfterDeathBehaviour(AController * inInstigator, AActor* DamageCauser) override;
private:
	UPROPERTY()
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimulusSourceComp = nullptr;

	void SetupStimulusSource();
};
