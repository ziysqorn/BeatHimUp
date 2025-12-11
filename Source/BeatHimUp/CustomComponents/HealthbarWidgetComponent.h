// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../Interface/HaveAttributeSet.h"
#include "../Interface/HaveHealthAttribute.h"
#include "../UI/HeadHealthbar/HeadHealthbar.h"
#include "HealthbarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UHealthbarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<AActor> CustomOwner = nullptr;

	void InitWidget() override;

public:
	void SetCustomOwner(AActor* inOwner) {
		CustomOwner = inOwner;
	}

	void SetupHealthbarUI();
};
