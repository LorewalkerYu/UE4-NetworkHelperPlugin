// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/JsonObjectFactory.h"
#include "Json.h"
UJsonObj* UJsonObjectFactory::ConstructJsonObject()
{
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	UJsonObj* Obj = NewObject<UJsonObj>();
	if (Obj)
	{
		Obj->SetJsonObj(JsonObj);
		return Obj;
	}
	return nullptr;
}

UJsonObj* UJsonObjectFactory::ConstructJsonObjectFromJsonString(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	if (FJsonSerializer::Deserialize(JsonReader, JsonObj))
	{
		UJsonObj* Obj = NewObject<UJsonObj>();
		if (Obj)
		{
			Obj->SetJsonObj(JsonObj);
		}
		return Obj;
	}
	return nullptr;
}

bool UJsonObjectFactory::ConstructJsonObjectArrayFromJsonString(const FString& JsonString, TArray<UJsonObj*> OutObjArray)
{
	TArray<UJsonObj*> OutObjs;
	TArray<TSharedPtr<FJsonValue>> OutArray;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	bool bFlag = FJsonSerializer::Deserialize(JsonReader, OutArray);
	if (!bFlag)
	{
		UE_LOG(LogTemp, Error, TEXT("UJsonHelperLibrary::DeserializeJsonArray---Deserialize Failed!"));
		return false;
	}
	for (const TSharedPtr<FJsonValue>& elem : OutArray)
	{
		TSharedPtr<FJsonObject> TempObj;
		TempObj = elem->AsObject();
		if (!TempObj)
		{
			UE_LOG(LogTemp, Error, TEXT("UJsonHelperLibrary::DeserializeJsonArray---This is not a json obj!"));
			return false;
		}
		UJsonObj* Obj = NewObject<UJsonObj>();
		if (Obj)
		{
			Obj->SetJsonObj(TempObj);
		}
		OutObjs.Add(Obj);
	}
	OutObjArray = OutObjs;
	return true;
}


