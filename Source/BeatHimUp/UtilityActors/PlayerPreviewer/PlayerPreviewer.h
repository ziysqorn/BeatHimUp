// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "PlayerPreviewer.generated.h"

UCLASS()
class BEATHIMUP_API APlayerPreviewer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerPreviewer();

	UFUNCTION(Client, Unreliable)
	void ResetModelRotation();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AltRootComponent")
	TObjectPtr<USceneComponent> AltRootComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Model")
	TObjectPtr<USkeletalMeshComponent> ModelComponent = nullptr;

	float MouseLocX = 0.0f;

	FRotator CurRotation = FRotator(0.0f, 0.0f, 0.0f);

	bool bIsRotating = false;

	// Called when the game starts or when spawned
	void BeginPlay() override;

	UFUNCTION(Client,Unreliable)
	void OnModelMeshClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION(Client, Unreliable)
	void OnModelMeshReleased();

	UFUNCTION(Client, Unreliable)
	void RotateModel();
};
