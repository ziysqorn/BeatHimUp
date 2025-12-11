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
