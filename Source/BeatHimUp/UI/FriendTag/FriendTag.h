// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "FriendTag.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UFriendTag : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Username;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_OnlineStatus;

public:
	void SetUsernameText(FText inText) {
		if (IsValid(Txt_Username)) Txt_Username->SetText(inText);
	}
	void SetTextOnline() {
		if (IsValid(Txt_OnlineStatus)) {
			Txt_OnlineStatus->SetText(FText::FromString("Online"));
			FSlateColor textColor(FColor::FromHex(FString("00FF08FF")));
			Txt_OnlineStatus->SetColorAndOpacity(textColor);
		}
	}

	void SetTextOffline() {
		if (IsValid(Txt_OnlineStatus)) {
			Txt_OnlineStatus->SetText(FText::FromString("Offline"));
			FSlateColor textColor(FColor::FromHex(FString("515151FF")));
			Txt_OnlineStatus->SetColorAndOpacity(textColor);
		}
	}
};
