// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../DataAsset/GameplayAbilityDataAsset.h"
#include "Weapon.generated.h"

UCLASS()
class BEATHIMUP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	FName GetAbilitySubclass() {
		return AbilitySubclassName;
	}

	UBoxComponent* GetBoxComp() {
		return BoxComp;
	}

	void ClearHandledActors(int32 ExpectedNumElements) {
		HandledActors.Empty(ExpectedNumElements);
	}

	virtual void CancelWeaponAbility() {}
protected:
	UPROPERTY()
	TSet<AActor*> HandledActors;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySubclassName")
	FName AbilitySubclassName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Altenative Root Component")
	USceneComponent* AltRootComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Box Collision")
	UBoxComponent* BoxComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComp = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
