// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ShieldBlock.h"

void UANS_ShieldBlock::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
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

void UANS_ShieldBlock::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp) {
		if (ABaseCharacter* character = Cast<ABaseCharacter>(MeshComp->GetOwner())) {
			if (UWeaponComponent* WeaponComponent = character->GetWeaponComponent()) {
				if (AWeapon* LeftWeapon = WeaponComponent->GetLeftWeapon()) {
					TArray<AActor*> OverlappedArr;
					LeftWeapon->GetOverlappingActors(OverlappedArr, AWeapon::StaticClass());
					for (auto actor : OverlappedArr) {
						if (actor != LeftWeapon && actor->GetOwner() != LeftWeapon->GetOwner()) {
							if (AWeapon* weapon = Cast<AWeapon>(actor)) {
								if (weapon->HasWeaponStateTag(FGameplayTag::RequestGameplayTag(FName("WeaponState.Attack")))) {
									FGameplayTag eventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Parried"));
									FGameplayEventData Payload;
									Payload.EventTag = eventTag;
									Payload.Instigator = weapon->GetOwner();
									Payload.Target = character;
									UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(character, eventTag, Payload);
								}
							}
						}
					}
				}
			}
		}
	}
}

void UANS_ShieldBlock::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
