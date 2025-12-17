// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../Item/UsableItem/UsableItem.h"
#include "ItemComponent.generated.h"


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItem> ItemSubclass;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRepUsableItemList, const TArray<UUsableItem*>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemQuantityChanged, UUsableItem* /*Changed Item*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEATHIMUP_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

	FOnRepUsableItemList OnRepUsableItemListDel;

	FOnItemQuantityChanged OnItemQuantityChangedDel;

	void UseItem();

	void SwitchUsableItem();

	const UItem* GetCurrentItem() {
		if (UsableItemList.IsValidIndex(CurrentItemIdx)) {
			return UsableItemList[CurrentItemIdx];
		}
		return nullptr;
	}

	const TArray<UUsableItem*>& GetUsableItemList() {
		return UsableItemList;
	}

	UFUNCTION(Server, Reliable)
	void Server_InitUsableItemList();

	void AddUsableItemToList(UUsableItem* Item);
	 
	void RemoveUsableItemFromList(UUsableItem* Item);

protected:
	int CurrentItemIdx = 0;

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_UsableItemList")
	TArray<UUsableItem*> UsableItemList;

	UPROPERTY(EditDefaultsOnly, Category = "DataTable | Items")
	TObjectPtr<UDataTable> DT_Items;

	// Called when the game starts
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_UsableItemList(const TArray<UUsableItem*>& OldUsableItemList);
};
