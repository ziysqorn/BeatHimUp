// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "../Shield/Shield.h"
#include "../../Interface/Damageable.h"
#include "../../Character/BaseCharacter/BaseCharacter.h"

ASword::ASword()
{
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	
	this->BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASword::BoxCompBeginOverlapped);
}

void ASword::BoxCompBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		if (OtherActor && OtherActor != this->GetOwner() && OtherActor->GetOwner() != this->GetOwner()) {
			FGameplayTag TargetAttackedEventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
			FGameplayEventData Payload;
			TArray<AActor*> OverlappingActor;
			GetOverlappingActors(OverlappingActor);
			TObjectPtr<AActor> overlappedWeapon = nullptr, overlappedDamageable = nullptr;
			for (AActor* actor : OverlappingActor) {
				if (AWeapon* Weapon = Cast<AWeapon>(actor)) {
					overlappedWeapon = actor;
				}
				else if (IDamageable* Damageable = Cast<IDamageable>(actor)) {
					overlappedDamageable = actor;
				}
			}
			if (overlappedWeapon) {
				if (IInteractableWithWeapon* InteractableWithWeapon = Cast<IInteractableWithWeapon>(overlappedWeapon)) {
					InteractableWithWeapon->ResponseToAttackingWeapon(this);
					return;
				}
			}
			if (overlappedDamageable) {
				Payload.EventTag = TargetAttackedEventTag;
				Payload.Instigator = this->GetOwner();
				Payload.Target = overlappedDamageable;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this->GetOwner(), TargetAttackedEventTag, Payload);
				return;
			}
		}
	}
}
