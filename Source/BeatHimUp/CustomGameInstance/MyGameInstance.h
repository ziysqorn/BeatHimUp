// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../MyStructs/MyStructs.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	void Init() override;
	void Shutdown() override;
	void SystemShutdownLogout();
public:
	UPROPERTY()
	FPlayerInfo PlayerInfo;

	TArray<FPlayerInfo> Friendlist;

	void LogoutRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);
};
