// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "PlayerPreviewerWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UPlayerPreviewerWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	FString GetUsernameTextAsString();

	void SetUsernameText(const FString& inUsername);

	void ShowCaptainIcon();

	void HideCaptainIcon();
};
