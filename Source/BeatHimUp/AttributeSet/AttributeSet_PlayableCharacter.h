// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../Interface/HaveHealthAttribute.h"
#include "../Interface/HaveStaminaAttribute.h"
#include "AttributeSet_PlayableCharacter.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class BEATHIMUP_API UAttributeSet_PlayableCharacter : public UAttributeSet, public IHaveHealthAttribute, public IHaveStaminaAttribute
{
	GENERATED_BODY()
	
public:
	UAttributeSet_PlayableCharacter();

	UFUNCTION()
	float GetHealthPercentage() const override {
		return Health.GetCurrentValue() / MaxHealth.GetCurrentValue();
	}

	UFUNCTION()
	float GetStaminaPercentage() const override {
		return Stamina.GetCurrentValue() / MaxStamina.GetCurrentValue();
	}

	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, Health)
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, MaxHealth)

	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, Stamina)
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, MaxStamina)

	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, Damage)
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, MaxDamage)

	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, StaminaRecoveryAmount)

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes" )
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Attributes")
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Attributes")
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRecoveryAmount, Category = "Attributes")
	FGameplayAttributeData StaminaRecoveryAmount;

	UPROPERTY()
	FGameplayAttributeData Damage;

	UPROPERTY()
	FGameplayAttributeData MaxDamage;

	UPROPERTY()
	FTimerHandle StaminaRecoverHandle;

	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	void OnRep_StaminaRecoveryAmount(const FGameplayAttributeData& OldAmount);
};
