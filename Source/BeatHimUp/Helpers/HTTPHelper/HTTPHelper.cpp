// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTPHelper.h"

void UHTTPHelper::SendRequest(const FString& Url, const FString& Method, const FString& contentString, const FHttpRequestCompleteDelegate& callback) {
	FString realURL = BASE_URL.Append(Url);
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
	httpRequest->SetVerb(Method);
	for (auto& header : headers) {
		httpRequest->SetHeader(header.Key, header.Value);
	}
	if (queryParams.Num() > 0) {
		realURL.Append(TEXT("?"));
		for (int i = 0; i < queryParams.Num(); ++i) {
			realURL.Append(FString::Format(TEXT("{0}={1}"), { queryParams[i].Key, queryParams[i].Value}));
			if(i < queryParams.Num() - 1) realURL.Append(TEXT("&"));
		}
	}
	httpRequest->SetURL(realURL);
	httpRequest->SetContentAsString(contentString);
	httpRequest->OnProcessRequestComplete() = callback;
	httpRequest->ProcessRequest();
}

void UHTTPHelper::AddQueryParam(const FString& key, const FString& value)
{
	queryParams.Add(TPair<FString, FString>(key, value));
}

void UHTTPHelper::AddHeader(const FString& key, const FString& value)
{
	headers.Add(TPair<FString, FString>(key, value));
}
