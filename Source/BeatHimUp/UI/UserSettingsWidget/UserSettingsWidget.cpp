// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingsWidget.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Interface/HaveSpecialInputMode.h"

void UUserSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_Close)) Btn_Close->OnClicked.AddDynamic(this, &UUserSettingsWidget::RemoveFromParent);
	if (IsValid(Btn_Apply)) Btn_Apply->OnClicked.AddDynamic(this, &UUserSettingsWidget::ApplySettings);
	if (IsValid(Btn_GraphicsSettings)) {
		Btn_GraphicsSettings->OnClicked.AddDynamic(this, &UUserSettingsWidget::SwitchToGraphicSettings);
		Btn_GraphicsSettings->OnHovered.AddDynamic(this, &UUserSettingsWidget::GraphicTitleHighlight);
		Btn_GraphicsSettings->OnUnhovered.AddDynamic(this, &UUserSettingsWidget::GraphicTitleUnHighlight);
	}
	if (IsValid(Btn_ExitMenu)) {
		Btn_ExitMenu->OnClicked.AddDynamic(this, &UUserSettingsWidget::SwitchToExitMenu);
		Btn_ExitMenu->OnHovered.AddDynamic(this, &UUserSettingsWidget::ExitTitleHighlight);
		Btn_ExitMenu->OnUnhovered.AddDynamic(this, &UUserSettingsWidget::ExitTitleUnHighlight);
	}
	if (IsValid(Btn_ExitGame)) {
		Btn_ExitGame->OnClicked.AddDynamic(this, &UUserSettingsWidget::LogoutAndExit);
	}

	QualityMap = { { "Low", 0 }, { "Medium", 2 }, { "High", 4 } };
}

void UUserSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(WidgetSwitcher_Settings)) {
		WidgetSwitcher_Settings->SetActiveWidgetIndex(0);
		UpdateSettingsTabHighlight(Btn_GraphicsSettings);
	}
	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->AddWidget(this);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
		if (APlayerController* PC = GetOwningPlayer()) {
			FInputModeUIOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
		FetchUserSettings();
	}
}

void UUserSettingsWidget::NativeDestruct()
{
	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->PopLastWidget();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		if (IHaveSpecialInputMode* HaveSpecialInputMode = GetOwningPlayer<IHaveSpecialInputMode>()) {
			HaveSpecialInputMode->HandleAfterUIRemove();
		}
	}

	Super::NativeDestruct();
}

void UUserSettingsWidget::GraphicTitleHighlight()
{
	if (IsValid(Btn_GraphicsSettings)) {
		if (UTextBlock* Title = Cast<UTextBlock>(Btn_GraphicsSettings->GetChildAt(0))) {
			Title->SetColorAndOpacity(FSlateColor(FColor::FromHex(TEXT("#ff3636"))));
		}
	}
}

void UUserSettingsWidget::GraphicTitleUnHighlight()
{
	if (IsValid(Btn_GraphicsSettings)) {
		if (!ChosenSettingsTab.IsValid() || (ChosenSettingsTab.IsValid() && ChosenSettingsTab != Btn_GraphicsSettings)) {
			if (UTextBlock* Title = Cast<UTextBlock>(Btn_GraphicsSettings->GetChildAt(0))) {
				Title->SetColorAndOpacity(FSlateColor(FColor::White));
			}
		}
	}
}

void UUserSettingsWidget::ExitTitleHighlight()
{
	if (IsValid(Btn_ExitMenu)) {
		if (UTextBlock* Title = Cast<UTextBlock>(Btn_ExitMenu->GetChildAt(0))) {
			Title->SetColorAndOpacity(FSlateColor(FColor::FromHex(TEXT("#ff3636"))));
		}
	}
}

void UUserSettingsWidget::ExitTitleUnHighlight()
{
	if (IsValid(Btn_ExitMenu)) {
		if (!ChosenSettingsTab.IsValid() || (ChosenSettingsTab.IsValid() && ChosenSettingsTab != Btn_ExitMenu)) {
			if (UTextBlock* Title = Cast<UTextBlock>(Btn_ExitMenu->GetChildAt(0))) {
				Title->SetColorAndOpacity(FSlateColor(FColor::White));
			}
		}
	}
}

void UUserSettingsWidget::SwitchToGraphicSettings()
{
	if (IsValid(WidgetSwitcher_Settings)) {
		UpdateSettingsTabHighlight(Btn_GraphicsSettings);
		WidgetSwitcher_Settings->SetActiveWidgetIndex(0);
	}
}

void UUserSettingsWidget::SwitchToExitMenu()
{
	if (IsValid(WidgetSwitcher_Settings)) {
		UpdateSettingsTabHighlight(Btn_ExitMenu);
		WidgetSwitcher_Settings->SetActiveWidgetIndex(1);
	}
}

void UUserSettingsWidget::ApplySettings()
{
	if (IsValid(ComboBox_Presets) && IsValid(ComboBox_MaxFrameLimit) && IsValid(ComboBox_VSync)) {
		if (GEngine) {
			if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings()) {
				int SelectedPresets = *QualityMap.Find(ComboBox_Presets->GetSelectedOption());
				GameUserSettings->SetOverallScalabilityLevel(SelectedPresets);

				GameUserSettings->SetFrameRateLimit(FCString::Atoi(*ComboBox_MaxFrameLimit->GetSelectedOption()));
				if (ComboBox_VSync->GetSelectedIndex() == 0) {
					GameUserSettings->SetVSyncEnabled(false);
				}
				else {
					GameUserSettings->SetVSyncEnabled(true);
				}
				GameUserSettings->ApplySettings(false);
			}
		}
	}
}

void UUserSettingsWidget::LogoutAndExit()
{
	UKismetSystemLibrary::QuitGame(this, this->GetOwningPlayer(), EQuitPreference::Quit, false);
}

FReply UUserSettingsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape) {
		RemoveFromParent();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UUserSettingsWidget::FetchUserSettings()
{
	if (GEngine) {
		if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings()) {
			if (IsValid(ComboBox_Presets)) {
				int CurrentQualityPreset = GameUserSettings->GetTextureQuality();
				for (auto& Preset : QualityMap) {
					FString PresetName = Preset.Get<0>();
					int PresetQuality = Preset.Get<1>();
					if (PresetQuality == CurrentQualityPreset) {
						ComboBox_Presets->SetSelectedOption(PresetName);
						break;
					}
				}
			}
			if (IsValid(ComboBox_MaxFrameLimit)) {
				ComboBox_MaxFrameLimit->SetSelectedOption(FString::FromInt(GameUserSettings->GetFrameRateLimit()));
			}
			if (IsValid(ComboBox_VSync)) {
				ComboBox_VSync->SetSelectedIndex(GameUserSettings->IsVSyncEnabled());
			}
		}
	}
}

void UUserSettingsWidget::UpdateSettingsTabHighlight(UWidget* ChosenTab)
{
	if (IsValid(HorBox_SettingsTab)) {
		ChosenSettingsTab = ChosenTab;
		FColor TabTitleColor;
		for (int i = 0; i < HorBox_SettingsTab->GetChildrenCount(); ++i) {
			if (HorBox_SettingsTab->GetChildAt(i) == ChosenTab) {
				TabTitleColor = FColor::FromHex(TEXT("#ff3636"));
			}
			else {
				TabTitleColor = FColor::White;
			}
			if (UButton* TabButton = Cast<UButton>(HorBox_SettingsTab->GetChildAt(i))) {
				if (UTextBlock* Title = Cast<UTextBlock>(TabButton->GetChildAt(0))) {
					Title->SetColorAndOpacity(FSlateColor(TabTitleColor));
				}
			}
		}
	}
}
