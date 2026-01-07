// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "PlayerPreviewerInfo.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UPlayerPreviewerInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Username;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_Captain;

public:
	FString GetUsernameTextAsString() {
		if (IsValid(Txt_Username)) {
			return Txt_Username->GetText().ToString();
		}
		return TEXT("");
	}

	void SetUsernameText(const FString& inUsername) {
		if (IsValid(Txt_Username)) {
			Txt_Username->SetText(FText::FromString(inUsername));
		}
	}

	void ShowCaptainIcon() {
		if (IsValid(Img_Captain)) {
			Img_Captain->SetVisibility(ESlateVisibility::Visible);
		}
	}

	void HideCaptainIcon() {
		if (IsValid(Img_Captain)) {
			Img_Captain->SetVisibility(ESlateVisibility::Hidden);
		}
	}
};
