// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "DodgeMontagesDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UDodgeMontagesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "DodgeMontages")
	TMap<FName, TObjectPtr<UAnimMontage>> DodgeMontagesMap;
};
