// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "CustomHUD.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UCustomHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Health = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina", meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Stamina = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "ItemFrameContainer", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorBox_ItemFrameContainer = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "BossHealthbar", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerBox_BossHealthbar = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "BossHealth", meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_BossHealth = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "BossName", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_BossName = nullptr;

	void NativeOnInitialized() override;

	void NativeConstruct() override;

public:
	void BindHealthProgress(UObject* inObject, const FName& funcName) {
		if (IsValid(ProgressBar_Health)) ProgressBar_Health->PercentDelegate.BindUFunction(inObject, funcName);
	}

	void BindStaminaProgress(UObject* inObject, const FName& funcName) {
		if (IsValid(ProgressBar_Stamina)) ProgressBar_Stamina->PercentDelegate.BindUFunction(inObject, funcName);
	}

	void BindBossHealthProgress(UObject* inObject, const FName& funcName) {
		if (IsValid(ProgressBar_BossHealth)) ProgressBar_BossHealth->PercentDelegate.BindUFunction(inObject, funcName);
	}

	void AddItemFrameToContainer(UWidget* inWidget);

	void RemoveItemFrameFromContainer(UWidget* inWidget);

	UWidget* GetItemFrame(int Idx);

	void SetBossName(FText inText) {
		if (IsValid(Txt_BossName)) Txt_BossName->SetText(inText);
	}

	void ShowBossHealthbar() {
		if (IsValid(VerBox_BossHealthbar)) {
			VerBox_BossHealthbar->SetVisibility(ESlateVisibility::Visible);
		}
	}

	void HideBossHealthbar() {
		if (IsValid(VerBox_BossHealthbar)) {
			VerBox_BossHealthbar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
};
