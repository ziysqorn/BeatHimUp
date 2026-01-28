// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "InGameController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UInGameController : public UObject
{
	GENERATED_BODY()

public:
	UInGameController();

	void GetCharacterStats(const FHttpRequestCompleteDelegate& callback);

	void SaveCharacterStats(const FString& contentString, const FHttpRequestCompleteDelegate& callback);
};
