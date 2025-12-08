// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BEATHIMUP_API UItem : public UObject
{
	GENERATED_BODY()
	
protected:
	virtual bool IsSupportedForNetworking() const override { return true; }

public:
	FName ItemName;
};
