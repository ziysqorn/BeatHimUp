// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_LeftWeaponAttack.h"
#include "../Character/BaseCharacter/BaseCharacter.h"

void UANS_LeftWeaponAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp) {
		if (ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner())) {
			if (UWeaponComponent* WeaponComponent = character->GetWeaponComponent()) {
				if (AWeapon* weapon = WeaponComponent->GetLeftWeapon()) {
					if (weapon->GetBoxComp())
						weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponActivePreset"));
				}
			}
		}
	}
}

void UANS_LeftWeaponAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp) {
		if (ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner())) {
			if (UWeaponComponent* WeaponComponent = character->GetWeaponComponent()) {
				if (AWeapon* weapon = WeaponComponent->GetLeftWeapon()) {
					if (weapon->GetBoxComp())
						weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponOfflinePreset"));
				}
			}
		}
	}
}


