// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "OnScreenAlert.generated.h"

/**
 * 
 */
UCLASS()

class BEATHIMUP_API UOnScreenAlert : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Txt_Message = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Txt_Confirm = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Txt_Cancel = nullptr;

	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Confirm = nullptr;

	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Cancel = nullptr;

public:
	void SetMessage(const FText& inMessage) {
		if (inMessage.IsEmptyOrWhitespace()) return;
		if (Txt_Message) Txt_Message->SetText(inMessage);
	}

	void SetConfirmText(const FText& inMessage) {
		if (inMessage.IsEmptyOrWhitespace()) return;
		if (Txt_Confirm) Txt_Confirm->SetText(inMessage);
	}

	void SetCancelText(const FText& inMessage) {
		if (inMessage.IsEmptyOrWhitespace()) return;
		if (Txt_Cancel) Txt_Cancel->SetText(inMessage);
	}

	UButton* GetConfirmBtn() {
		return Btn_Confirm;
	}

	UButton* GetCancelBtn() {
		return Btn_Cancel;
	}
};
