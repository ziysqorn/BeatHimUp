// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyStructs.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMatchStatus : uint8
{
	MATCH_BEGIN UMETA(DisplayName = "Match Begin"),
	MATCH_PAUSED UMETA(DisplayName = "Match Paused"),
	MATCH_END UMETA(DisplayName = "Match Ended"),
	MATCH_WON UMETA(DisplayName = "Match Won"),
	MATCH_LOST UMETA(DisplayName = "Match Lost")
};

USTRUCT()
struct FPlayerInfo {

	GENERATED_BODY()

	UPROPERTY()
	FName Username;

	UPROPERTY()
	bool isOnline;

	FPlayerInfo() : Username(NAME_None), isOnline(false) {}
};
