// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "../../GameplayAbilities/GA_Dead.h"
#include "../../CustomGameState/MainGameState.h"

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
			if (GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_AIMove")))
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_AIMove")), 1, -1, this));

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

		if (AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>()) {
			MainGameState->SetBossRef(this);
		}
	}


}

void ABaseEnemy::ExecuteAfterDeathBehaviour(AController* inInstigator, AActor* DamageCauser)
{
	if (OnDeathDel.IsBound()) {
		OnDeathDel.Broadcast(this);
	}
	if (AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>()) {
		MainGameState->OnBossKilled(this, inInstigator, DamageCauser);
	}
}

void ABaseEnemy::Hurt_Implementation(const float& remainHealth, const float& totalHealth, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AbilitySystemComp) {
		if (FMath::IsNearlyEqual(remainHealth, 0.0f, 1.0E-4)) {
			if (IsValid(GADataAsset) && GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead"))) {
				if (FGameplayAbilitySpec* GASpec = AbilitySystemComp->FindAbilitySpecFromClass(*GADataAsset->GameplayAbilitySubclassMap.Find(FName("GA_Dead")))) {
					if (UGA_Dead* GA_Dead = Cast<UGA_Dead>(GASpec->GetPrimaryInstance())) {
						GA_Dead->SetupDeadInfo(EventInstigator, DamageCauser);
						AbilitySystemComp->TryActivateAbility(GASpec->Handle);
					}
				}
			}
		}
		else
		{
			FVector VecFromThisToDamageCauser = DamageCauser->GetActorLocation() - this->GetActorLocation();
			float dotProduct = FVector::DotProduct(this->GetActorForwardVector(), VecFromThisToDamageCauser);
			if (dotProduct < 0) {
				FGameplayTagContainer tagContainer;
				tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Block")));
				AbilitySystemComp->CancelAbilities(&tagContainer);
			}
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
