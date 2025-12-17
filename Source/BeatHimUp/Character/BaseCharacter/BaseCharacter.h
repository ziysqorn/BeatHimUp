// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../ActorComponent/WeaponComponent/WeaponComponent.h"
#include "../../ActorComponent/HitStopComponent/HitStopComponent.h"
#include "../../DataAsset/MontagesDataAsset.h"
#include "BaseCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLockTarget, AActor* /*Locked Target*/);

UCLASS()
class BEATHIMUP_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> LockedOnTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | WeaponComponent")
	UWeaponComponent* WeaponComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | HitStopComponent")
	UHitStopComponent* HitStopComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties | DataAssets | Montages")
	UMontagesDataAsset* MontagesDataAsset = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FOnLockTarget OnLockTargetDel;

	// Sets default values for this character's properties
	ABaseCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetCapsuleCollisionProfile(FName profileName);

	UWeaponComponent* GetWeaponComponent() {
		return WeaponComponent;
	}

	TWeakObjectPtr<AActor>& GetLockedOnTarget() {
		return LockedOnTarget;
	}

	UMontagesDataAsset* GetMontagesDataAsset() {
		return MontagesDataAsset;
	}
};
