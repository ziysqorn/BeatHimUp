// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "HTTPHelper.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UHTTPHelper : public UObject
{
	GENERATED_BODY()
	
private:
	FString BASE_URL = "http://127.0.0.1:3000";

	TArray<TPair<FString, FString>> queryParams;

	TArray<TPair<FString, FString>> headers;

public:
	void SendRequest(const FString& Url, const FString& Method, const FString& contentString, const FHttpRequestCompleteDelegate& callback);

	void AddQueryParam(const FString& key, const FString& value);

	void AddHeader(const FString& key, const FString& value);

	bool IsHeaderEmpty() {
		return headers.Num() == 0;
	}
};
