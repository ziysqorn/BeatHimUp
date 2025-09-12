// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "HumanoidMontagesDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UHumanoidMontagesDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HumanoidMontagesMap")
	TMap<FName, UAnimMontage*> HumanoidMontagesMap;
};
