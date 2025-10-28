// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "ContextMenu.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UContextMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerBox_Menu;

	UPROPERTY(EditDefaultsOnly, Category = "ButtonStyle")
	FButtonStyle ButtonStyle;

	UPROPERTY(EditDefaultsOnly, Category = "FontStyle")
	FSlateFontInfo FontStyle;

	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;
public:
	void AddMenuOption(UButton* inButton, UTextBlock* inTextBlock);
};
