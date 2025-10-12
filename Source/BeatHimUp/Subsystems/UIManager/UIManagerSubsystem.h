// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
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

public:
	UFUNCTION(Client, Reliable)
	void AddWidget(UUserWidget* inWidget);

	UFUNCTION(Client, Reliable)
	void PopLastWidget();

	TWeakObjectPtr<UUserWidget>& GetTopWidget();
};
