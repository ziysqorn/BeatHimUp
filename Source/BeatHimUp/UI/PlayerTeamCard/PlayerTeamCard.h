// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "PlayerTeamCard.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UPlayerTeamCard : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Txt_CardName = nullptr;

public:
	void SetCardName(const FText& newName) {
		if (Txt_CardName) Txt_CardName->SetText(newName);
	}
};
