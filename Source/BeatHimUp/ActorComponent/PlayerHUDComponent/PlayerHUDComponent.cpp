// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDComponent.h"
#include "../../CustomGameState/MainGameState.h"
#include "../../Interface/HaveHealthAttribute.h"
#include "../../Interface/HaveStaminaAttribute.h"


// Sets default values for this component's properties
UPlayerHUDComponent::UPlayerHUDComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPlayerHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPlayerHUDComponent::Client_SetEnemyNameText_Implementation(const FText& inText)
{
	if (IsValid(MainHUD)) {
		MainHUD->SetBossName(inText);
	}
}

void UPlayerHUDComponent::Client_BindEnemyHealthBar_Implementation(AActor* Actor)
{
	if (IsValid(MainHUD)) {
		if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(Actor)) {
			if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet())) {
				MainHUD->BindBossHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
			}
		}
	}
}

void UPlayerHUDComponent::Client_SetupBossHUDHealthbar_Implementation(AActor* Actor)
{
	if (MainHUDSubclass) {
		if (APlayerController* PlayerController = Cast<APlayerController>(this->GetOwner())) {
			if (!IsValid(MainHUD))
				MainHUD = CreateWidget<UCustomHUD>(PlayerController, MainHUDSubclass);
			if (IsValid(MainHUD)) {
				MainHUD->SetBossName(FText::FromString(TEXT("Warrok")));
				if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(Actor)) {
					if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet())) {
						MainHUD->BindBossHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
					}
				}
				if (!MainHUD->IsInViewport()) {
					MainHUD->AddToViewport(0);
				}
			}
		}
	}
}

void UPlayerHUDComponent::Client_AddHUD_Implementation()
{
	if (MainHUDSubclass) {
		if (APlayerController* PlayerController = Cast<APlayerController>(this->GetOwner())) {
			if (!IsValid(MainHUD))
				MainHUD = CreateWidget<UCustomHUD>(PlayerController, MainHUDSubclass);
			if (MainHUD) {
				if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(PlayerController->GetPawn())) {
					if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet())) {
						MainHUD->BindHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
					}
					if (IHaveStaminaAttribute* HaveStaminaAttr = Cast<IHaveStaminaAttribute>(HaveAS->GetAttributeSet())) {
						MainHUD->BindStaminaProgress(HaveAS->GetAttributeSet(), FName("GetStaminaPercentage"));
					}
				}
				if (AMainGameState* MainGS = GetWorld()->GetGameState<AMainGameState>()) {
					if (AActor* Boss = MainGS->GetBossRef()) {
						MainHUD->SetBossName(FText::FromString(TEXT("Warrok")));
						if (IHaveAttributeSet* HaveAS = Cast<IHaveAttributeSet>(Boss)) {
							if (IHaveHealthAttribute* HaveHealthAttr = Cast<IHaveHealthAttribute>(HaveAS->GetAttributeSet())) {
								MainHUD->BindBossHealthProgress(HaveAS->GetAttributeSet(), FName("GetHealthPercentage"));
							}
						}
					}
				}
				if (!MainHUD->IsInViewport()) {
					MainHUD->AddToViewport(0);
				}
			}
		}
	}
}

