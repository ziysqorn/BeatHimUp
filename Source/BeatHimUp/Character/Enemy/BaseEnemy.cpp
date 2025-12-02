// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
	CharacterAttributeSet = CreateDefaultSubobject<UAS_AICharacter>("GameplayAttributeSet");
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

			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Hurt")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Hurt")), 1, -1, this));

			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead")), 1, -1, this));

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
	}
}

void ABaseEnemy::Hurt_Implementation(const float& remainHealth, const float& totalHealth)
{
	if (AbilitySystemComp) {
		if (FMath::IsNearlyEqual(remainHealth, 0.0f, 1.0E-4)) {
			if (IsValid(GADataAsset) && GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead"))) {
				AbilitySystemComp->TryActivateAbilityByClass(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead")));
			}
		}
		else
		{
			if (IsValid(GADataAsset) && GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Hurt"))) {
				AbilitySystemComp->TryActivateAbilityByClass(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Hurt")));
			}
		}
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
