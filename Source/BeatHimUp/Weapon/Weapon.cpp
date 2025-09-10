// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AltRootComp = CreateDefaultSubobject<USceneComponent>(FName("AltenativeRootComp"));
	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComp"));
	AltRootComp->SetupAttachment(RootComponent);
	RootComponent = AltRootComp;
	BoxComp->AttachToComponent(AltRootComp, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMeshComp->AttachToComponent(AltRootComp, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

