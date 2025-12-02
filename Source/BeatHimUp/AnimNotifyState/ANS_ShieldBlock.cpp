// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ShieldBlock.h"

void UANS_ShieldBlock::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp) {
		if (ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner())) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(character)) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying")));
				}
			}
			if (UWeaponComponent* WeaponComponent = character->GetWeaponComponent()) {
				if (AWeapon* weapon = WeaponComponent->GetLeftWeapon()) {
					if (weapon->GetBoxComp())
						weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponActivePreset"));
				}
			}
		}
	}
}

void UANS_ShieldBlock::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp) {
		if (ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner())) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(character)) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying")));
				}
			}
			if (UWeaponComponent* WeaponComponent = character->GetWeaponComponent()) {
				if (AWeapon* weapon = WeaponComponent->GetLeftWeapon()) {
					if (weapon->GetBoxComp())
						weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponOfflinePreset"));
				}
			}
		}
	}
}
