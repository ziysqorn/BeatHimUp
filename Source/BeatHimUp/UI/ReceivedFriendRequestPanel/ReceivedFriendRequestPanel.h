// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "ReceivedFriendRequestPanel.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UReceivedFriendRequestPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Username;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Accept;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Decline;

	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void AcceptRequest();

	UFUNCTION()
	void DeclineRequest();

public:
	void SetSenderUsername(const FName& inUsername) {
		if (IsValid(Txt_Username)) {
			Txt_Username->SetText(FText::FromName(inUsername));
		}
	}
};
