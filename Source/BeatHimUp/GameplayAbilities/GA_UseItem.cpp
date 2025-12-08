// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_UseItem.h"
#include "../Character/BaseCharacter/BaseCharacter.h"

void UGA_UseItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(ActorInfo->OwnerActor))
		{
			if (ICanUseItem* CanUseItemInterface = Cast<ICanUseItem>(OwnCharacter)) {
				if (UItemComponent* ItemComp = CanUseItemInterface->GetItemComponent()) {
					if (const UUsableItem* UsableItem = Cast<UUsableItem>(ItemComp->GetCurrentItem())) {
						if (UMontagesDataAsset* DA_Montages = OwnCharacter->GetMontagesDataAsset()) {
							if (FMontageSet* MontageSet = DA_Montages->MontagesMap.Find("State")) {
								if (UAnimMontage** MontageToPlay = MontageSet->ActionMontages.Find(UsableItem->GetMontageSubclassName())) {
									if (MontageToPlay && *MontageToPlay)
									{
										if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageAndWait"), *MontageToPlay)) {
											Task->OnCompleted.AddDynamic(this, &UGA_UseItem::UseItemMontageEnd);
											Task->ReadyForActivation();
										}

									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void UGA_UseItem::UseItemMontageEnd()
{
	if (CurrentActorInfo) {
		if (!HasAuthority(&CurrentActivationInfo)) {
			if (ICanUseItem* CanUseItemInterface = Cast<ICanUseItem>(CurrentActorInfo->OwnerActor)) {
				if (UItemComponent* ItemComp = CanUseItemInterface->GetItemComponent()) {
					ItemComp->UseItem();
					Server_RequestEndAbility();
				}
			}
		}
	}
}

bool UGA_UseItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool SuperCondition = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	bool CheckValidQuantity = false;
	if (ActorInfo) {
		if (ICanUseItem* CanUseItemInterface = Cast<ICanUseItem>(ActorInfo->OwnerActor)) {
			if (UItemComponent* ItemComp = CanUseItemInterface->GetItemComponent()) {
				if (const UUsableItem* UsableItem = Cast<UUsableItem>(ItemComp->GetCurrentItem())) {
					CheckValidQuantity = UsableItem->CanUseItem();
				}
			}
		}
	}
	return SuperCondition && CheckValidQuantity;
}

void UGA_UseItem::Server_RequestEndAbility_Implementation()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
