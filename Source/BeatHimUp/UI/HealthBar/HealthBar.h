// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeOnInitialized() override;
};
