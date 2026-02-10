// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

AAICharacter::AAICharacter()
{
	this->PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
}

void AAICharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (UAbilitySystemComponent* ASC = this->GetAbilitySystemComponent()) {
		TagContainer = ASC->GetOwnedGameplayTags();
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && AbilitySystemComp) {
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		AbilitySystemComp->AffectedAnimInstanceTag = NAME_None;
	}
}
