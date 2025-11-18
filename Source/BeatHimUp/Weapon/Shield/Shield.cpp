// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"

AShield::AShield()
{
}

void AShield::CancelWeaponAbility()
{
	if (IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(this->GetOwner())) {
		if (UAbilitySystemComponent* ASC = ASInterface->GetAbilitySystemComponent()) {
			FGameplayTagContainer tagContainer;
			tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Block")));
			ASC->CancelAbilities(&tagContainer);
		}
	}
}
