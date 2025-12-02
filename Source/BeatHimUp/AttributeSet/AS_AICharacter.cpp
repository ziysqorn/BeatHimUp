// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_AICharacter.h"
#include "../Interface/Damageable.h"
#include "GameplayEffectExtension.h"

UAS_AICharacter::UAS_AICharacter()
{
	MaxHealth = 100.0f;
	MaxDamage = 50.0f;
	Health = MaxHealth;
	Damage = MaxDamage;
}

void UAS_AICharacter::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxHealth.GetCurrentValue());

	}
}

void UAS_AICharacter::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UAS_AICharacter::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) 
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

void UAS_AICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAS_AICharacter, Health);

	DOREPLIFETIME(UAS_AICharacter, MaxHealth);
}

void UAS_AICharacter::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_AICharacter, Health, OldHealth);
}

void UAS_AICharacter::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_AICharacter, MaxHealth, OldHealth);
}
