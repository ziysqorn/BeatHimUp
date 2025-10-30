// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPreviewer.h"
#include "../../Controller/MainMenuController/MainMenuController.h"

// Sets default values
APlayerPreviewer::APlayerPreviewer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AltRootComponent = CreateDefaultSubobject<USceneComponent>(FName("AltenativeRootComponent"));
	ModelComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	AltRootComponent->SetupAttachment(RootComponent);
	RootComponent = AltRootComponent;
	ModelComponent->AttachToComponent(AltRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("Haha"));
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
}

