// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthbarWidgetComponent.h"

void UHealthbarWidgetComponent::InitWidget()
{
	Super::InitWidget();
}

void UHealthbarWidgetComponent::SetupHealthbarUI()
{
    if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(CustomOwner)) {
        if (IHaveHealthAttribute* HaveHealthAttri = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet()))
        {
            if (UHeadHealthbar* HeadHealthbar = Cast<UHeadHealthbar>(this->GetUserWidgetObject()))
            {
                HeadHealthbar->BindHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
            }
        }
    }
}

void UHealthbarWidgetComponent::SetHeadInfo(const FPlayerInfo& InInfo)
{
    if (UHeadHealthbar* HeadHealthbar = Cast<UHeadHealthbar>(this->GetUserWidgetObject()))
    {
        HeadHealthbar->SetNameText(InInfo.Username.ToString());
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Format(TEXT("Username On PlayerState: {0}"), { PlayerInfo->Username.ToString() }));
    }
}
