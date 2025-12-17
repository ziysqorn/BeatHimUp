// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "UsableItemFrame.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UUsableItemFrame : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ItemImage", meta = (BindWidget))
	TObjectPtr<UImage> Img_Item;

	UPROPERTY(EditDefaultsOnly, Category = "QuantityText", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Quantity;

public:
	void SetItemImage(UObject* inResourceObj) {
		if (IsValid(Img_Item)) {
			Img_Item->SetBrushResourceObject(inResourceObj);
		}
	}
	void SetQuantityText(FText inText) {
		if (IsValid(Txt_Quantity)) Txt_Quantity->SetText(inText);
	}
};
