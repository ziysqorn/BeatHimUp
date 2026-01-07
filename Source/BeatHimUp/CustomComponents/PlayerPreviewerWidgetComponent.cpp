// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPreviewerWidgetComponent.h"
#include "../UI/PlayerPreviewerInfo/PlayerPreviewerInfo.h"

FString UPlayerPreviewerWidgetComponent::GetUsernameTextAsString() 
{
	if (UPlayerPreviewerInfo* PlayerPreviewerInfo = Cast<UPlayerPreviewerInfo>(this->GetWidget())) {
		return PlayerPreviewerInfo->GetUsernameTextAsString();
	}
	return TEXT("");
}

void UPlayerPreviewerWidgetComponent::SetUsernameText(const FString& inUsername) 
{
	if (UPlayerPreviewerInfo* PlayerPreviewerInfo = Cast<UPlayerPreviewerInfo>(this->GetWidget())) {
		PlayerPreviewerInfo->SetUsernameText(inUsername);
	}
}

void UPlayerPreviewerWidgetComponent::ShowCaptainIcon() 
{
	if (UPlayerPreviewerInfo* PlayerPreviewerInfo = Cast<UPlayerPreviewerInfo>(this->GetWidget())) {
		PlayerPreviewerInfo->ShowCaptainIcon();
	}
}

void UPlayerPreviewerWidgetComponent::HideCaptainIcon() 
{
	if (UPlayerPreviewerInfo* PlayerPreviewerInfo = Cast<UPlayerPreviewerInfo>(this->GetWidget())) {
		PlayerPreviewerInfo->HideCaptainIcon();
	}
}



