// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "GameServerController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGameServerController : public UObject
{
	GENERATED_BODY()

public:
	UGameServerController();

	void DropGameServer(const FString& Server_ID, const FHttpRequestCompleteDelegate& callback);
};
