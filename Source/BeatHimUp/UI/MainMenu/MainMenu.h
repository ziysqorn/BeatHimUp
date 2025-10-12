// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Txt_Username;

	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	void SetUsernameText(const FText& inText) {
		if (IsValid(Txt_Username)) Txt_Username->SetText(inText);
	}
};
