// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include "../../Character/BaseCharacter/BaseCharacter.h"

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

void AShield::ResponseToAttackingWeapon(AActor* Causer)
{
	if (Causer) {
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(this->GetOwner())) {
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
				if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying")))) {
					FGameplayTag eventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Parried"));

					FGameplayEventData ParriedPayload;
					ParriedPayload.EventTag = eventTag;
					ParriedPayload.Instigator = this->GetOwner();
					ParriedPayload.Target = Causer->GetOwner();

					/*UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Causer->GetOwner(), eventTag, CauserPayload);
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this->GetOwner(), eventTag, TargetPayload);*/

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this->GetOwner(), eventTag, ParriedPayload);
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Causer->GetOwner(), eventTag, ParriedPayload);
				}
				else {

				}
			}
		}
	}
}
