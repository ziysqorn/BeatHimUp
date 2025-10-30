// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/ContextMenu/ContextMenu.h"
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

	void InitFriendTagCxtMenu(const TArray<TPair<FText, FOnButtonClickedEvent&>>& Options, TSubclassOf<UUserWidget> CxtMenuSubclass, FVector2D MenuPosition);
};
