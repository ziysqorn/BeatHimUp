// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		if (WeaponComponent) {
			if (WeaponComponent->GetWeaponSubclassByName(FName("MonsterSwiping"))) {
				WeaponComponent->SetupWeaponsOnHands(
					*WeaponComponent->GetWeaponSubclassByName(FName("MonsterSwiping")),
					nullptr,
					FName("LeftHand_Shield"),
					FName("")
				);
			}
		}

		if (AbilitySystemComp && GADataAsset) {
			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_HumanoidMove")), 1, -1, this));

			if (AWeapon* RightWeapon = WeaponComponent->GetRightWeapon()) {
				if (GADataAsset->GameplayAbilitySubclassMap.Find(RightWeapon->GetAbilitySubclass())) {
					AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(RightWeapon->GetAbilitySubclass()), 1, -1, RightWeapon));
				}
			}

			if (AWeapon* LeftWeapon = WeaponComponent->GetLeftWeapon()) {
				if (GADataAsset->GameplayAbilitySubclassMap.Find(LeftWeapon->GetAbilitySubclass())) {
					AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(LeftWeapon->GetAbilitySubclass()), 1, -1, LeftWeapon));
				}
			}
		}
		//GetWorldTimerManager().SetTimer(AttackHandle, FTimerDelegate::CreateUObject(this, &ABaseEnemy::AttackTriggered), 2.0f, true);
	}
}

void ABaseEnemy::AttackTriggered_Implementation()
{
	if (WeaponComponent && AbilitySystemComp) {
		if (AWeapon* LeftWeapon = WeaponComponent->GetLeftWeapon()) {
			if (GADataAsset->GameplayAbilitySubclassMap.Find(LeftWeapon->GetAbilitySubclass())) {
				AbilitySystemComp->TryActivateAbilityByClass(*GADataAsset->GameplayAbilitySubclassMap.Find(LeftWeapon->GetAbilitySubclass()));
			}
		}
	}
}
