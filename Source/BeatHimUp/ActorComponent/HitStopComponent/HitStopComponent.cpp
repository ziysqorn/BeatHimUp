// Fill out your copyright notice in the Description page of Project Settings.


#include "HitStopComponent.h"


// Sets default values for this component's properties
UHitStopComponent::UHitStopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UHitStopComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHitStopComponent::NetMulticast_HitStop_Implementation(float HitStopDuration, float HitStopDilation)
{
	if (GetOwner()) {
		GetOwner()->CustomTimeDilation = HitStopDilation;
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(HitStopTimerHandle, FTimerDelegate::CreateUObject(this, &UHitStopComponent::EndHitStop), HitStopDuration, false);
	}
}

void UHitStopComponent::EndHitStop()
{
	GetOwner()->CustomTimeDilation = 1.0f;
}

void UHitStopComponent::Client_ShakeCameraOnHit_Implementation()
{
	if (GetOwner()) {
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeSubclass, GetOwner()->GetActorLocation(), 0.0f, 1000.0f);
	}
}


