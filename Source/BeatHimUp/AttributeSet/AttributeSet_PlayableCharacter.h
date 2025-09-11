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

	float GetMaxHealthCurrentValue() {
		return MaxHealth.GetCurrentValue();
	}

	float GetHealthCurrentValue() {
		return Health.GetCurrentValue();
	}

	float GetHealthPercentage() {
		return Health.GetCurrentValue() / MaxHealth.GetCurrentValue();
	}
protected:
	UPROPERTY()
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, Health)

	UPROPERTY()
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, MaxHealth)

	UPROPERTY()
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, Damage)

	UPROPERTY()
	FGameplayAttributeData MaxDamage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_PlayableCharacter, MaxDamage)

	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
