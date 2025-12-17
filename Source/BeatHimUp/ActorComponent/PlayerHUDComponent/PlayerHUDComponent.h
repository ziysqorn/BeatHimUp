// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/CustomHUD/CustomHUD.h"
#include "../../UI/UsableItemFrame/UsableItemFrame.h"
#include "../../UI/OnScreenMessage/OnScreenMessage.h"
#include "../../AttributeSet/AttributeSet_PlayableCharacter.h"
#include "../../Interface/HaveAttributeSet.h"
#include "PlayerHUDComponent.generated.h"

class UUsableItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEATHIMUP_API UPlayerHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerHUDComponent();

	UFUNCTION(Client, Unreliable)
	void Client_SetEnemyNameText(const FText& inText);

	UFUNCTION(Client, Reliable)
	void Client_BindEnemyHealthBar(AActor* Actor);

	UFUNCTION(Client, Unreliable)
	void Client_SetupBossHUDHealthbar(AActor* Actor);

	UFUNCTION(Client, Reliable)
	void Client_AddHUD();

	UFUNCTION()
	void DisplayMatchStatusMessage(EMatchStatus inMatchStatus);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties|HUDSubclass")
	TSubclassOf<UCustomHUD> MainHUDSubclass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties|UsableItemFrameSubclass")
	TSubclassOf<UUsableItemFrame> UsableItemFrameSubclass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties|OnScreenMessageSubclass")
	TSubclassOf<UOnScreenMessage> OnScreenMessageSubclass;

	UPROPERTY()
	TObjectPtr<UCustomHUD> MainHUD = nullptr;

	UPROPERTY()
	TObjectPtr<UOnScreenMessage> OnScreenMessage = nullptr;

	// Called when the game starts
	virtual void BeginPlay() override;

	void SetupItemFrameList(const TArray<UUsableItem*>& inItemList);

	void UpdateItemFrameQuantity(UUsableItem* Item);

	void BindItemDelegates();
};
