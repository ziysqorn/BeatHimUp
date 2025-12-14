// Fill out your copyright notice in the Description page of Project Settings.


#include "OnScreenMessage.h"

void UOnScreenMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (WidgetAnimDel.IsBound()) {
		WidgetAnimDel.Clear();
	}
	WidgetAnimDel.BindUFunction(this, FName("FadeinFinishedDelay"));
	BindToAnimationFinished(Fadein, WidgetAnimDel);
}

void UOnScreenMessage::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOnScreenMessage::FadeinFinishedDelay()
{
	GetWorld()->GetTimerManager().SetTimer(AfterFinishedFadeinHandle, AfterFinishedFadeinDel, AfterFinishedFadeinDelay, false);
}

void UOnScreenMessage::PlayMessageFadein()
{
	if (IsValid(Fadein)) {
		PlayAnimationForward(Fadein);
	}
}
