// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "MontagesDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMontageSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, UAnimMontage*> ActionMontages;
};

UCLASS()
class BEATHIMUP_API UMontagesDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontagesMap")
	TMap<FName, FMontageSet> MontagesMap;
};