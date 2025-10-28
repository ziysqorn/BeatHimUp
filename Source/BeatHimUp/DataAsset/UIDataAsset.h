// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../UI/OnScreenAlert/OnScreenAlert.h"
#include "../UI/OnlyCloseAlert/OnlyCloseAlert.h"
#include "../UI/ContextMenu/ContextMenu.h"
#include "UIDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UUIDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UISubclassesMap")
	TMap<FName, TSubclassOf<UUserWidget>> UISubclassMap;
};
