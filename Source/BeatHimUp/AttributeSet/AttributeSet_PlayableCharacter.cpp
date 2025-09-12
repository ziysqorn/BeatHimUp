// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet_PlayableCharacter.h"
#include "../Interface/Damageable.h"
#include "GameplayEffectExtension.h"

UAttributeSet_PlayableCharacter::UAttributeSet_PlayableCharacter()
{
	MaxHealth = 100.0f;
	MaxDamage = 50.0f;
	Health = MaxHealth;
	Damage = MaxDamage;
}

void UAttributeSet_PlayableCharacter::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxHealth.GetCurrentValue());
	}
}

void UAttributeSet_PlayableCharacter::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		
		if (Data.EvaluatedData.ModifierOp == EGameplayModOp::AddBase && Data.EvaluatedData.Magnitude < 0.0f) {
			if (IDamageable* damageable = Cast<IDamageable>(this->GetActorInfo()->OwnerActor)) {
				damageable->Hurt(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
			}
		}
	}
}
