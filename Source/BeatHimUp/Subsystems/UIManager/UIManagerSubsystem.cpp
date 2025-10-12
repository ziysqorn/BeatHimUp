// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubsystem.h"


void UUIManagerSubsystem::AddWidget_Implementation(UUserWidget* inWidget)
{
	if (IsValid(inWidget)) {
		if (stk_Widgets.Num() > 0) {
			TWeakObjectPtr<UUserWidget> topWidget = stk_Widgets.Top();
			if (topWidget.IsValid()) {
				topWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		TWeakObjectPtr<UUserWidget> newWidget(inWidget);
		stk_Widgets.Push(newWidget);
	}
}

void UUIManagerSubsystem::PopLastWidget_Implementation() {
	if (!stk_Widgets.IsEmpty()) {
		stk_Widgets.Pop(EAllowShrinking::No);
		if (!stk_Widgets.IsEmpty()) {
			TWeakObjectPtr<UUserWidget> topWidget = stk_Widgets.Top();
			if (topWidget.IsValid()) {
				topWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

TWeakObjectPtr<UUserWidget>& UUIManagerSubsystem::GetTopWidget() {
	return stk_Widgets.Top();
}

