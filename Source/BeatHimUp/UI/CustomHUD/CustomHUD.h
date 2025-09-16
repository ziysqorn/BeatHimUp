// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "CustomHUD.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UCustomHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (BindWidget))
	UProgressBar* ProgressBar_Health = nullptr;

	void NativeOnInitialized() override;

public:
	void BindHealthProgress(UObject* inObject, const FName& funcName) {
		if (IsValid(ProgressBar_Health)) ProgressBar_Health->PercentDelegate.BindUFunction(inObject, funcName);
	}
};
