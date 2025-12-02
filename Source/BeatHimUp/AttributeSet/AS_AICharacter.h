// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "AS_AICharacter.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class BEATHIMUP_API UAS_AICharacter : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAS_AICharacter();

	float GetHealthPercentage() const {
		return Health.GetCurrentValue() / MaxHealth.GetCurrentValue();
	}

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAS_AICharacter, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAS_AICharacter, MaxHealth)

	UPROPERTY()
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAS_AICharacter, Damage)

	UPROPERTY()
	FGameplayAttributeData MaxDamage;
	ATTRIBUTE_ACCESSORS(UAS_AICharacter, MaxDamage)

	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldHealth);
};
