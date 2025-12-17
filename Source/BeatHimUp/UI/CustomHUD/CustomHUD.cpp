// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomHUD.h"

void UCustomHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCustomHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCustomHUD::AddItemFrameToContainer(UWidget* inWidget)
{
	if (IsValid(HorBox_ItemFrameContainer)) {
		HorBox_ItemFrameContainer->AddChildToHorizontalBox(inWidget);
	}
}

void UCustomHUD::RemoveItemFrameFromContainer(UWidget* inWidget)
{
	if (IsValid(HorBox_ItemFrameContainer)) {
		HorBox_ItemFrameContainer->RemoveChild(inWidget);
	}
}

UWidget* UCustomHUD::GetItemFrame(int Idx)
{
	if (IsValid(HorBox_ItemFrameContainer)) {
		return HorBox_ItemFrameContainer->GetChildAt(Idx);
	}
	return nullptr;
}
