// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyStructs.generated.h"

/**
 * 
 */
USTRUCT()
struct FPlayerInfo {

	GENERATED_BODY()

	UPROPERTY()
	FName Username;

	UPROPERTY()
	bool isOnline;

	FPlayerInfo() : Username(NAME_None), isOnline(false) {}
};
