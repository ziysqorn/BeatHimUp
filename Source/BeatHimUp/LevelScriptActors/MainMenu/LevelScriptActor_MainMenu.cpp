// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor_MainMenu.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../GameMode/MainMenuGameMode/MainMenuGameMode.h"

void ALevelScriptActor_MainMenu::BeginPlay()
{
	Super::BeginPlay();

	if (AMainMenuGameMode* GameMode = GetWorld()->GetAuthGameMode<AMainMenuGameMode>()) {
		GameMode->PostLoginDel.AddUObject(this, &ALevelScriptActor_MainMenu::NetMulticast_AddPlayerToPreviewer);
	}
	if (!HasAuthority()) {
		SetupView();
	}
}

void ALevelScriptActor_MainMenu::ResetPlayerPreviewerList_Implementation()
{
	for (auto PlayerPreviewer : PlayerPreviewerList) {
		if (IsValid(PlayerPreviewer)) PlayerPreviewer->ResetModelRotation();
	}
}

void ALevelScriptActor_MainMenu::SetupView()
{
	for (int i = 0; i < PlayerPreviewerList.Num(); ++i) {
		if (PlayerPreviewerList[i]) {
			if (USkeletalMeshComponent* ModelComp = PlayerPreviewerList[i]->GetModel()) {
				ModelComp->SetVisibility(false);
			}
		}
	}
	if (CineCamera) {
		if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
			MainMenuController->SetViewTargetWithBlend(CineCamera);
		}
	}
}

void ALevelScriptActor_MainMenu::NetMulticast_AddPlayerToPreviewer_Implementation(APlayerController* NewPlayerController)
{
	if (!HasAuthority()) {
		if (USkeletalMeshComponent* ModelComp = PlayerPreviewerList[CurrentPreviewerPlaceholder]->GetModel()) {
			ModelComp->SetVisibility(true);
			++CurrentPreviewerPlaceholder;
		}
	}
}
