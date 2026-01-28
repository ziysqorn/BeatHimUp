// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	HitStopComp = CreateDefaultSubobject<UHitStopComponent>(FName("HitStopComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(FName("WeaponComponent"));
}

void ABaseCharacter::NetMulticast_SetCapsuleCollisionProfile_Implementation(FName profileName)
{
	if (GetCapsuleComponent()) 
		GetCapsuleComponent()->SetCollisionProfileName(profileName);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::EndPlay(EEndPlayReason::Type Reason)
{
	if (HasAuthority()) {
		TArray<AActor*> AttachedActors;
		GetAttachedActors(AttachedActors);
		for (int i = 0; i < AttachedActors.Num(); ++i) {
			if (IsValid(AttachedActors[i])) {
				AttachedActors[i]->Destroy();
			}
		}
	}
	Super::EndPlay(Reason);
}
