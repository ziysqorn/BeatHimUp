// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/OnScreenAlert/OnScreenAlert.h"
#include "LoginScreen.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ULoginScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TxtBox_Username;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UEditableTextBox> TxtBox_Password;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn_ShowPassword;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Txt_UsernameAlert;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Txt_PasswordAlert;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Login;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn_Clear;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn_QuitGame;

	UPROPERTY(EditDefaultsOnly, Category = "EyeTextureArray")
	TArray<UTexture2D*> EyeArr;

	UPROPERTY(EditDefaultsOnly, Category = "ScreenAlertSubclass")
	TSubclassOf<UOnScreenAlert> ScreenAlertSubclass;

	UPROPERTY()
	TObjectPtr<UOnScreenAlert> ScreenAlert;

	void NativeOnInitialized() override;
	void NativeConstruct() override;

	UFUNCTION()
	void TogglePassword();

	UFUNCTION()
	void ClearInputs();

	UFUNCTION(Client, Reliable)
	void ExitGame();

	UFUNCTION()
	void Login();

	UFUNCTION(Client, Reliable)
	void DisplaySignUpAlert();

	void LoginRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	bool CheckUsername();
	
	bool CheckPassword();

	UFUNCTION(Client, Unreliable)
	void CloseScreenAlert();
};
