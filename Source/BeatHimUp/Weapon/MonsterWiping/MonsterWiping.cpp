// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWiping.h"
#include "../Shield/Shield.h"
#include "../../Interface/Damageable.h"
#include "../../Character/BaseCharacter/BaseCharacter.h"

AMonsterWiping::AMonsterWiping()
{
}

void AMonsterWiping::BeginPlay()
{
	Super::BeginPlay();
	
	this->BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AMonsterWiping::BoxCompBeginOverlapped);
	this->BoxComp->OnComponentEndOverlap.AddDynamic(this, &AMonsterWiping::BoxCompEndOverlapped);
}

void AMonsterWiping::BoxCompBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (HasAuthority()) {
		if (OtherActor && OtherActor != this && OtherActor != this->GetOwner() && OtherActor->GetOwner() != this->GetOwner()) {
			FGameplayTag TargetAttackedEventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
			FGameplayEventData Payload;
			if (IInteractableWithWeapon* InteractableWithWeapon = Cast<IInteractableWithWeapon>(OtherActor)) {
				if (HandledActors.Contains(OtherActor->GetOwner())) {
					return;
				}
				HandledActors.Add(OtherActor->GetOwner());
				InteractableWithWeapon->ResponseToAttackingWeapon(this);
			}
			else if (IDamageable* Damageable = Cast<IDamageable>(OtherActor)) {
				if (HandledActors.Contains(OtherActor)) {
					return;
				}
				HandledActors.Add(OtherActor);
				Payload.EventTag = TargetAttackedEventTag;
				Payload.Instigator = this->GetOwner();
				Payload.Target = OtherActor;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this->GetOwner(), TargetAttackedEventTag, Payload);
			}
		}
	}
}

void AMonsterWiping::BoxCompEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*if (HasAuthority()) {
		if (OtherActor && OtherActor != this && OtherActor != this->GetOwner() && OtherActor->GetOwner() != this->GetOwner()) {
			if (IInteractableWithWeapon* InteractableWithWeapon = Cast<IInteractableWithWeapon>(OtherActor)) {
				if (HandledActors.Contains(OtherActor->GetOwner())) {
					HandledActors.Remove(OtherActor->GetOwner());
				}
			}
			else if (IDamageable* Damageable = Cast<IDamageable>(OtherActor)) {
				if (HandledActors.Contains(OtherActor)) {
					HandledActors.Remove(OtherActor);
				}
			}
			else if (HandledActors.Contains(OtherActor)) {
				HandledActors.Remove(OtherActor);
			}
		}
	}*/
}

