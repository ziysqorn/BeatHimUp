// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "UserSettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UUserSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_Settings;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> HorBox_SettingsTab;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_GraphicsSettings;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_ExitMenu;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBox_Presets;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBox_MaxFrameLimit;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBox_VSync;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Close;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Apply;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_ExitGame;

	UPROPERTY()
	TWeakObjectPtr<UWidget> ChosenSettingsTab;

	bool bShouldHideCursorAfterRemoved = true;

	TMap<FString, int> QualityMap;

	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void GraphicTitleHighlight();

	UFUNCTION()
	void GraphicTitleUnHighlight();

	UFUNCTION()
	void ExitTitleHighlight();

	UFUNCTION()
	void ExitTitleUnHighlight();

	UFUNCTION()
	void SwitchToGraphicSettings();

	UFUNCTION()
	void SwitchToExitMenu();

	UFUNCTION()
	void ApplySettings();

	UFUNCTION()
	void LogoutAndExit();

	void FetchUserSettings();

	void UpdateSettingsTabHighlight(UWidget* ChosenTab);

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
public:
	void SetHideMouseCursorAfterRemoved(bool inBool) {
		bShouldHideCursorAfterRemoved = inBool;
	}
};
