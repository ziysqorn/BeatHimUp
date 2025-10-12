// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "OnlyCloseAlert.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UOnlyCloseAlert : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Message = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Confirm = nullptr;

	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(Client, Unreliable)
	void CloseAlert();

public:
	void SetMessage(const FText& inMessage) {
		if (inMessage.IsEmptyOrWhitespace()) return;
		if (Txt_Message) Txt_Message->SetText(inMessage);
	}
};
