// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

AAICharacter::AAICharacter()
{
	this->PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && AbilitySystemComp) {
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		AbilitySystemComp->AffectedAnimInstanceTag = NAME_None;
	}
}
