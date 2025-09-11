// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

ASword::ASword()
{
	//this->OnActorBeginOverlap.AddDynamic(this, &ASword::ActorBeginOverlapped);
}

void ASword::BeginPlay()
{
	Super::BeginPlay();

	this->OnActorBeginOverlap.AddDynamic(this, &ASword::ActorBeginOverlapped);
}

void ASword::ActorBeginOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor)) {
		FGameplayTag eventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
		FGameplayEventData Payload;
		Payload.EventTag = eventTag;
		Payload.Instigator = this->GetOwner();
		Payload.Target = OtherCharacter;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this->GetOwner(), eventTag, Payload);
	}
}
