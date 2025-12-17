// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BEATHIMUP_API UItem : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties | ItemName")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties | ItemImage")
	TObjectPtr<UTexture2D> ItemImage;

	virtual bool IsSupportedForNetworking() const override { return true; }
public:
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack) override;

	const FName& GetItemName() {
		return ItemName;
	}

	UTexture2D* GetItemImage() {
		return ItemImage;
	}
};
