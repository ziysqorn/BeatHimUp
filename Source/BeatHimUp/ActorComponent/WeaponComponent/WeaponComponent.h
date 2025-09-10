// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../Weapon/Weapon.h"
#include "../../DataAsset/WeaponDataAsset.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEATHIMUP_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	AWeapon* LeftWeapon = nullptr;
	AWeapon* RightWeapon = nullptr;

public:
	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Weapon Subclass Data Asset")
	UWeaponDataAsset* WeaponDataAsset = nullptr;

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	void SetLeftWeapon(AWeapon* inWeapon, FName SocketName) {
		LeftWeapon = inWeapon;
		if (IsValid(LeftWeapon)) {
			LeftWeapon->SetOwner(this->GetOwner());
			if (ACharacter* character = Cast<ACharacter>(this->GetOwner())) {
				LeftWeapon->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
				LeftWeapon->SetActorEnableCollision(false);
			}
		}
	}

	void SetRightWeapon(AWeapon* inWeapon, FName SocketName) {
		RightWeapon = inWeapon;
		if (IsValid(RightWeapon)) {
			RightWeapon->SetOwner(this->GetOwner());
			if (ACharacter* character = Cast<ACharacter>(this->GetOwner())) {
				RightWeapon->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
				RightWeapon->SetActorEnableCollision(false);
			}
		}

	}


	void SetupWeaponsOnHands(TSubclassOf<AWeapon> LeftWeaponType, TSubclassOf<AWeapon> RightWeaponType, FName LeftHandSocketName, FName RightHandSocketname) {
		if (ACharacter* character = Cast<ACharacter>(this->GetOwner())) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = character;
			if (IsValid(LeftWeaponType) && !LeftHandSocketName.IsNone()) {
				if (AWeapon* spawnedLeftWeapon = GetWorld()->SpawnActor<AWeapon>(LeftWeaponType, character->GetMesh()->GetSocketLocation(LeftHandSocketName),
					character->GetMesh()->GetSocketRotation(LeftHandSocketName),
					SpawnParams
				))
				{
					spawnedLeftWeapon->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandSocketName);
					LeftWeapon = spawnedLeftWeapon;
					LeftWeapon->SetActorEnableCollision(false);
				}
			}
			if (IsValid(LeftWeaponType) && !RightHandSocketname.IsNone()) {
				if (AWeapon* spawnedRightWeapon = GetWorld()->SpawnActor<AWeapon>(RightWeaponType, character->GetMesh()->GetSocketLocation(RightHandSocketname),
					character->GetMesh()->GetSocketRotation(RightHandSocketname),
					SpawnParams
				))
				{
					spawnedRightWeapon->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandSocketname);
					RightWeapon = spawnedRightWeapon;
					RightWeapon->SetActorEnableCollision(false);
				}
			}
		}
	}

	AWeapon* GetLeftWeapon() {
		return LeftWeapon;
	}

	AWeapon* GetRightWeapon() {
		return RightWeapon;
	}

	TSubclassOf<AWeapon>* GetWeaponSubclassByName(const FName& name) {
		if (WeaponDataAsset) {
			return WeaponDataAsset->WeaponSubclassMap.Find(name);
		}
		return nullptr;
	}
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
