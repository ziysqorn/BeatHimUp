// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
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
class BEATHIMUP_API UAttributeSet_PlayableCharacter : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAttributeSet_PlayableCharacter();

	float GetHealthPercentage() const {
		return Health.GetCurrentValue() / MaxHealth.GetCurrentValue();
	}

	float GetStaminaPercentage() const {
		return Stamina.GetCurrentValue() / MaxStamina.GetCurrentValue();
	}
protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes" )
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, Stamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, MaxStamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRecoveryAmount, Category = "Attributes")
	FGameplayAttributeData StaminaRecoveryAmount;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, StaminaRecoveryAmount)

	UPROPERTY()
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, Damage)

	UPROPERTY()
	FGameplayAttributeData MaxDamage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, MaxDamage)

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
