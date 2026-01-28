// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "HeadHealthbar.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UHeadHealthbar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ProgressBar_Health", meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Health = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "PawnNameText", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Name = nullptr;

	void NativeOnInitialized() override;

	void NativeConstruct() override;

public:
	void BindHealthProgress(UObject* inObject, const FName& funcName) {
		if (IsValid(ProgressBar_Health)) ProgressBar_Health->PercentDelegate.BindUFunction(inObject, funcName);
	}

	void SetNameText(const FString& inName) {
		if (IsValid(Txt_Name)) Txt_Name->SetText(FText::FromString(inName));
	}
};
