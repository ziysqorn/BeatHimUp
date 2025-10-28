// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendlistController.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"


UFriendlistController::UFriendlistController()
{
}

void UFriendlistController::SendFriendlistMessage(FName Username)
{
	if (UServiceControllerSubsystem* ServiceControllerSubsystem = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
		TSharedPtr<FJsonObject> PayloadObj = MakeShareable(new FJsonObject());
		PayloadObj->SetStringField("username", Username.ToString());
		JsonObj->SetStringField("resource", "friendlist");
		JsonObj->SetStringField("action", "get");
		JsonObj->SetObjectField("payload", PayloadObj);
		FString contentString;
		TSharedRef<TJsonWriter<>> Write = TJsonWriterFactory<>::Create(&contentString);
		if (FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Write)) {
			ServiceControllerSubsystem->SendWSMessage(contentString);
		}
	}
}