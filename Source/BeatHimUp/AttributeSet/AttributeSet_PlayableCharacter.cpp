// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet_PlayableCharacter.h"
#include "../Interface/Damageable.h"
#include "GameplayEffectExtension.h"

UAttributeSet_PlayableCharacter::UAttributeSet_PlayableCharacter()
{
	MaxHealth = 100.0f;
	MaxStamina = 100.0f;
	MaxDamage = 50.0f;
	Health = MaxHealth;
	Stamina = MaxStamina;
	Damage = MaxDamage;
	StaminaRecoveryAmount = 1.0f;
}

void UAttributeSet_PlayableCharacter::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxHealth.GetCurrentValue());

	}
	if (Attribute == GetStaminaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxStamina.GetCurrentValue());
	}
}

void UAttributeSet_PlayableCharacter::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetStaminaAttribute()) {
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.0f, MaxStamina.GetCurrentValue()));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Format(TEXT("CurrentValue: {0}, BaseValue: {1}"), { Stamina.GetCurrentValue(), Stamina.GetBaseValue()}));
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

void UAttributeSet_PlayableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UAttributeSet_PlayableCharacter::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_PlayableCharacter, Health, OldHealth);
}

void UAttributeSet_PlayableCharacter::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_PlayableCharacter, MaxHealth, OldHealth);
}

void UAttributeSet_PlayableCharacter::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_PlayableCharacter, Stamina, OldStamina);
}

void UAttributeSet_PlayableCharacter::OnRep_MaxStamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_PlayableCharacter, MaxStamina, OldStamina);
}

void UAttributeSet_PlayableCharacter::OnRep_StaminaRecoveryAmount(const FGameplayAttributeData& OldAmount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_PlayableCharacter, StaminaRecoveryAmount, OldAmount);
}
