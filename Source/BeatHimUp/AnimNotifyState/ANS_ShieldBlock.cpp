// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ShieldBlock.h"

void UANS_ShieldBlock::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp) {
		if (ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner())) {
			if (character->HasAuthority()) {
				if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(character)) {
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
						if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Block")))) {
							if (UWeaponComponent* WeaponComponent = character->GetWeaponComponent()) {
								if (AWeapon* weapon = WeaponComponent->GetLeftWeapon()) {
									if (weapon->GetBoxComp())
										weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponActivePreset"));
								}
							}
							ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying")));
						}
					}
				}
			}
		}
	}
}

void UANS_ShieldBlock::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp) {
		if (ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner())) {
			if (character->HasAuthority()) {
				if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(character)) {
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
						if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Block")))) {
							ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying")));
						}
					}
				}
			}
		}
	}
}
