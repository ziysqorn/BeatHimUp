// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/ContextMenu/ContextMenu.h"
#include "../../UI/LoadingScreen/LoadingScreen.h"
#include "../../UI/UserSettingsWidget/UserSettingsWidget.h"
#include "../../DataAsset/UIDataAsset.h"
#include "UIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	TArray<TWeakObjectPtr<UUserWidget>> stk_Widgets;

protected:
	UPROPERTY()
	TObjectPtr<UContextMenu> CtxMenu_FriendTag;

	UPROPERTY()
	TObjectPtr<ULoadingScreen> LoadingScreen;

	UPROPERTY()
	TObjectPtr<UUserSettingsWidget> UserSettingWidget;

public:
	UFUNCTION(Client, Reliable)
	void AddWidget(UUserWidget* inWidget);

	UFUNCTION(Client, Reliable)
	void PopLastWidget();

	TWeakObjectPtr<UUserWidget>& GetTopWidget();

	void HideCtxMenu() {
		if (IsValid(CtxMenu_FriendTag)) {
			CtxMenu_FriendTag->RemoveFromParent();
		}
	}

	void EmptyWidgetStk() {
		stk_Widgets.Empty();
	}

	void InitFriendTagCxtMenu(const TArray<TPair<FText, TSharedPtr<FOnButtonClickedEvent>>>& Options, TSubclassOf<UUserWidget> CxtMenuSubclass, FVector2D MenuPosition);

	void ShowNonDestroyedWidget(UUserWidget* Widget, int32 ZOrder);

	void RemoveNonDestroyedWidget(UUserWidget* Widget);

	void ShowLoadingScreen(TSubclassOf<UUserWidget> LoadingScreenSubclass, int32 ZOrder);

	void RemoveLoadingScreen();

	void PostLoadMapPreparation(UWorld* InLoadedWorld);

	void ShowUserSettingsWidget(TSubclassOf<UUserWidget> WidgetSubclass, int32 ZOrder, bool bShouldHideCursorAfterRemoved);

	void RemoveUserSettingsWidget();
};
