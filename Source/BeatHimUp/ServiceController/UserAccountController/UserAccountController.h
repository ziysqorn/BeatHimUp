// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "UserAccountController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UUserAccountController : public UObject
{
	GENERATED_BODY()
	
public:
	void CreateUser(const FString& contentString, const FHttpRequestCompleteDelegate& callback);
	void LoginUser(const FString& contentString, const FHttpRequestCompleteDelegate& callback);
};
