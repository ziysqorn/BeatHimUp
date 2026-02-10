// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPreviewer.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../CustomGameInstance/MyGameInstance.h"

// Sets default values
APlayerPreviewer::APlayerPreviewer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AltRootComponent = CreateDefaultSubobject<USceneComponent>(FName("AltenativeRootComponent"));
	ModelComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	PlayerPreviewerWidgetComp = CreateDefaultSubobject<UPlayerPreviewerWidgetComponent>(FName("PlayerPreviewerWidgetComp"));
	AltRootComponent->SetupAttachment(RootComponent);
	RootComponent = AltRootComponent;
	ModelComponent->AttachToComponent(AltRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PlayerPreviewerWidgetComp->AttachToComponent(AltRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void APlayerPreviewer::ResetModelRotation_Implementation()
{
	if (IsValid(ModelComponent)) ModelComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void APlayerPreviewer::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(ModelComponent)) {
		ModelComponent->OnClicked.AddDynamic(this, &APlayerPreviewer::OnModelMeshClicked);
	}
	if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
		MainMenuController->BindLeftMouseRelease(this, FName("OnModelMeshReleased"));
		MainMenuController->BindLeftMouseClicked(this, FName("RotateModel"));
	}
}

void APlayerPreviewer::RotateModel_Implementation()
{
	if (bIsRotating) {
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
			float CurMouseLocX, MouseLocY;
			int32 ViewportX, ViewportY;
			PlayerController->GetMousePosition(CurMouseLocX, MouseLocY);
			PlayerController->GetViewportSize(ViewportX, ViewportY);
			float lengthRatio = FMath::Abs(CurMouseLocX - MouseLocX) / (float)ViewportX;
			float degreeToRotate = CurMouseLocX >= MouseLocX ? 360.0f * lengthRatio * -1 : 360.0f * lengthRatio;
			FRotator TargetRotation = CurRotation + FRotator(0.0f, degreeToRotate, 0.0f);
			ModelComponent->SetRelativeRotation(TargetRotation);
		}

	}
}

void APlayerPreviewer::OnModelMeshReleased_Implementation()
{
	bIsRotating = false;
}

void APlayerPreviewer::OnModelMeshClicked_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::LeftMouseButton) {
		bIsRotating = true;
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
			float MouseLocY;
			PlayerController->GetMousePosition(MouseLocX, MouseLocY);
			if (IsValid(ModelComponent)) {
				CurRotation = ModelComponent->GetRelativeRotation();
			}
		}
	}
	else if (ButtonPressed == EKeys::RightMouseButton) {
		OptionContextHandle();
	}
}

void APlayerPreviewer::TriggerLeaveLobby()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
			if (UUIManagerSubsystem* UIManager = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
				UIManager->HideCtxMenu();
				MainMenuController->LeaveLobby();
			}
		}
	}
}

void APlayerPreviewer::TriggerMakeLeader()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
			if (UUIManagerSubsystem* UIManager = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
				UIManager->HideCtxMenu();
				FString ClickedUsername = PlayerPreviewerWidgetComp->GetUsernameTextAsString();
				MainMenuController->MakeLeader(ClickedUsername);
			}
		}
	}
}

void APlayerPreviewer::TriggerKickMember()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
			if (UUIManagerSubsystem* UIManager = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
				UIManager->HideCtxMenu();
				FString ClickedUsername = PlayerPreviewerWidgetComp->GetUsernameTextAsString();
				MainMenuController->KickMemberFromLobby(ClickedUsername);
			}
		}
	}
}

void APlayerPreviewer::TriggerAddFriend()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
			if (UUIManagerSubsystem* UIManager = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
				UIManager->HideCtxMenu();
				FString ClickedUsername = PlayerPreviewerWidgetComp->GetUsernameTextAsString();
				MainMenuController->SendFriendRequest(ClickedUsername);
			}
		}
	}
}

void APlayerPreviewer::TriggerRemoveFriend()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
			if (UUIManagerSubsystem* UIManager = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
				UIManager->HideCtxMenu();
				FString ClickedUsername = PlayerPreviewerWidgetComp->GetUsernameTextAsString();
				MainMenuController->RemoveFriend(ClickedUsername);
			}
		}
	}
}

void APlayerPreviewer::OptionContextHandle()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (UUIManagerSubsystem* UIManager = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
			if (AMainMenuController* MainMenuController = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(this, 0))) {
				FName ClickedUsername = FName(PlayerPreviewerWidgetComp->GetUsernameTextAsString());
				TArray<TPair<FText, TSharedPtr<FOnButtonClickedEvent>>> Options;
				if (ClickedUsername.IsEqual(MyGameInstance->GetPlayerInfo().Username)) {
					TSharedPtr<FOnButtonClickedEvent> SharedPtr_LeaveLobby = MakeShared<FOnButtonClickedEvent>();
					SharedPtr_LeaveLobby->AddDynamic(this, &APlayerPreviewer::TriggerLeaveLobby);
					Options.Add(TPair<FText, TSharedPtr<FOnButtonClickedEvent>>(FText::FromString("Leave lobby"), SharedPtr_LeaveLobby));
				}
				else {
					if (MyGameInstance->GetLobbyInfo().Leader_Username.IsEqual(MyGameInstance->GetPlayerInfo().Username)) {
						TSharedPtr<FOnButtonClickedEvent> SharedPtr_MakeLeader = MakeShared<FOnButtonClickedEvent>();
						TSharedPtr<FOnButtonClickedEvent> SharedPtr_KickMember = MakeShared<FOnButtonClickedEvent>();
						SharedPtr_MakeLeader->AddDynamic(this, &APlayerPreviewer::TriggerMakeLeader);
						SharedPtr_KickMember->AddDynamic(this, &APlayerPreviewer::TriggerKickMember);
						Options.Add(TPair<FText, TSharedPtr<FOnButtonClickedEvent>>(FText::FromString("Make leader"), SharedPtr_MakeLeader));
						Options.Add(TPair<FText, TSharedPtr<FOnButtonClickedEvent>>(FText::FromString("Kick member"), SharedPtr_KickMember));
					}
					if (MyGameInstance->CheckIsFriend(ClickedUsername) == -1) {
						TSharedPtr<FOnButtonClickedEvent> SharedPtr_SendFriendRequest = MakeShared<FOnButtonClickedEvent>();
						SharedPtr_SendFriendRequest->AddDynamic(this, &APlayerPreviewer::TriggerAddFriend);
						Options.Add(TPair<FText, TSharedPtr<FOnButtonClickedEvent>>(FText::FromString("Add friend"), SharedPtr_SendFriendRequest));
					}
					else {
						TSharedPtr<FOnButtonClickedEvent> SharedPtr_RemoveFriend = MakeShared<FOnButtonClickedEvent>();
						SharedPtr_RemoveFriend->AddDynamic(this, &APlayerPreviewer::TriggerRemoveFriend);
						Options.Add(TPair<FText, TSharedPtr<FOnButtonClickedEvent>>(FText::FromString("Remove friend"), SharedPtr_RemoveFriend));
					}
				}
				FVector2D MouseViewportPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
				UIManager->InitFriendTagCxtMenu(Options, MainMenuController->GetMenuWidgetSubclass("ContextMenu"), MouseViewportPos);
			}
		}
	}
}

