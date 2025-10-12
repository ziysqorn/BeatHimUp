// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../ServiceController/UserAccountController/UserAccountController.h"
#include "ServiceControllerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UServiceControllerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	FString BASE_URL = "http://127.0.0.1:3000";

public:
	UServiceControllerSubsystem();

	UPROPERTY()
	TObjectPtr<UUserAccountController> UserAccountController;

	const FString& GetBaseURL() {
		return BASE_URL;
	}
};
