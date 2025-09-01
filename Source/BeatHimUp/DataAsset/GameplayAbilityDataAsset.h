// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "GameplayAbilityDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGameplayAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "GASubclassesMap")
	TMap<FName, TSubclassOf<UGameplayAbility>> GameplayAbilitySubclassMap;
};
