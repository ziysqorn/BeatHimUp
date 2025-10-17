// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor_MainMenu.h"
#include "../../Controller/MainMenuController/MainMenuController.h"

void ALevelScriptActor_MainMenu::BeginPlay()
{
	Super::BeginPlay();

	SetupView();
}

void ALevelScriptActor_MainMenu::ResetPlayerPreviewerList_Implementation()
{
	for (auto PlayerPreviewer : PlayerPreviewerList) {
		if (IsValid(PlayerPreviewer)) PlayerPreviewer->ResetModelRotation();
	}
}

void ALevelScriptActor_MainMenu::SetupView_Implementation()
{
	if (CineCamera) {
		if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
			MainMenuController->SetViewTargetWithBlend(CineCamera);
		}
	}
}
