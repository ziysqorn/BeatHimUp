// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "OnScreenMessage.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UOnScreenMessage : public UUserWidget
{
	GENERATED_BODY()

protected:
	FTimerHandle AfterFinishedFadeinHandle;

	FWidgetAnimationDynamicEvent WidgetAnimDel;

	float AfterFinishedFadeinDelay = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "MessageText", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Message = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Fadein = nullptr;

	void NativeOnInitialized() override;

	void NativeConstruct() override;

	UFUNCTION()
	void FadeinFinishedDelay();

public:
	FTimerDelegate AfterFinishedFadeinDel;

	void SetMessageText(FText inText) {
		if (IsValid(Txt_Message)) Txt_Message->SetText(inText);
	}

	void SetMessageColor(const FString& inColorCode) {
		if (IsValid(Txt_Message)) {
			FSlateColor SlateColor(FColor::FromHex(inColorCode));
			Txt_Message->SetColorAndOpacity(SlateColor);
		}
	}

	void PlayMessageFadein();

	void SetAfterFinishedFadeinDelay(float inDelay) {
		AfterFinishedFadeinDelay = inDelay;
	}
};
