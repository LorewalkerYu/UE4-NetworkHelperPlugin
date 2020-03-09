// Fill out your copyright notice in the Description page of Project Settings.



#include "JsonObj.h"
#include "Json.h"
#include "JsonValue.h"
#include "JsonObjectFactory.h"


void UJsonObj::SetJsonObj(TSharedPtr<FJsonObject> NewObj)
{
	this->Obj = NewObj;
}

bool UJsonObj::AsJsonStr(FString& JsonStr)
{
	if (this->Obj.IsValid() && this->Obj->Values.Num() > 0)
	{
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&JsonStr);
		FJsonSerializer::Serialize(this->Obj.ToSharedRef(), t_writer);

		return true;
	}
	return false;
}

bool UJsonObj::IsValid() const
{
	return Obj.IsValid();
}

bool UJsonObj::HasField(const FString& FieldName) const
{
	if (!this->IsValid())
	{
		return false;
	}
	return Obj->HasField(FieldName);
}

bool UJsonObj::RemoveField(const FString& FieldName)
{
	if (!this->IsValid())
	{
		return false;
	}
	this->Obj->RemoveField(FieldName);
	return true;

}


bool UJsonObj::GetNumberField(const FString& FieldName, float& OutNumber) const
{
	if (!this->IsValid())
	{
		return false;
	}
	double Out;
	if (Obj->TryGetNumberField(FieldName, Out))
	{
		OutNumber = Out;
		return true;
	}
	return false;
}

bool UJsonObj::GetIntegerField(const FString& FieldName, int32& OutInteger) const
{
	if (!this->IsValid())
	{
		return false;
	}
	int32 Out;
	if (Obj->TryGetNumberField(FieldName, Out))
	{
		OutInteger = Out;
		return true;
	}
	return false;
}

bool UJsonObj::GetStringField(const FString& FieldName, FString& OuStr) const
{
	if (!this->IsValid())
	{
		return false;
	}
	FString Out;
	if (Obj->TryGetStringField(FieldName, Out))
	{
		OuStr = Out;
		return true;
	}
	return false;
}

bool UJsonObj::GetBoolField(const FString& FieldName, bool& OutBool) const
{
	if (!this->IsValid())
	{
		return false;
	}
	bool Out;
	if (Obj->TryGetBoolField(FieldName, Out))
	{
		OutBool = Out;
		return true;
	}
	return false;
}

UJsonObj* UJsonObj::GetObjectField(const FString& FieldName) const
{
	if (!this->IsValid())
	{
		return nullptr;
	}
	const TSharedPtr<FJsonObject> JsonObj = Obj->GetObjectField(FieldName);
	if (!JsonObj.IsValid())
	{
		return nullptr;
	}
	UJsonObj* NewObj = NewObject<UJsonObj>();
	NewObj->SetJsonObj(JsonObj);
	return NewObj;
}

bool UJsonObj::GetObjectArray(const FString& FieldName, TArray<UJsonObj*>& OutArray)
{
	const TArray<TSharedPtr<FJsonValue>>* ValueArrayPtr;
	bool bRet = GetJsonValueArray(FieldName, ValueArrayPtr);
	if (!bRet)
	{
		return false;
	}
	for (const TSharedPtr<FJsonValue>& elem : (*ValueArrayPtr))
	{
		TSharedPtr<FJsonObject> TempObj = elem->AsObject();
		if (!TempObj.IsValid())
		{
			continue;
		}
		UJsonObj* NewObj = NewObject<UJsonObj>();
		NewObj->SetJsonObj(TempObj);
		OutArray.Add(NewObj);
	}
	return true;

}

bool UJsonObj::GetNumberArray(const FString& FieldName, TArray<bool>& OutArray)
{
	const TArray<TSharedPtr<FJsonValue>>* ValueArrayPtr;
	bool bRet = GetJsonValueArray(FieldName, ValueArrayPtr);
	if (!bRet)
	{
		return false;
	}
	for (const TSharedPtr<FJsonValue>& elem : (*ValueArrayPtr))
	{
		bool TempBool = elem->AsBool();
		OutArray.Add(TempBool);
	}
	return true;
}

bool UJsonObj::GetIntegerArray(const FString& FieldName, TArray<int32>& OutArray)
{
	const TArray<TSharedPtr<FJsonValue>>* ValueArrayPtr;
	bool bRet = GetJsonValueArray(FieldName, ValueArrayPtr);
	if (!bRet)
	{
		return false;
	}
	for (const TSharedPtr<FJsonValue>& elem : (*ValueArrayPtr))
	{
		int32 TempInt = elem->AsNumber();
		OutArray.Add(TempInt);
	}
	return true;
}

bool UJsonObj::GetStringArray(const FString& FieldName, TArray<FString>& OutArray)
{
	const TArray<TSharedPtr<FJsonValue>>* ValueArrayPtr;
	bool bRet = GetJsonValueArray(FieldName, ValueArrayPtr);
	if (!bRet)
	{
		return false;
	}
	for (const TSharedPtr<FJsonValue>& elem : (*ValueArrayPtr))
	{
		FString TempStr = elem->AsString();
		OutArray.Add(TempStr);
	}
	return true;
}

bool UJsonObj::GetJsonValueArray(const FString& FieldName, const TArray<TSharedPtr<FJsonValue>>*& OutArray)
{
	if (!this->IsValid())
	{
		return false;
	}
	if (Obj->TryGetArrayField(FieldName, OutArray))
	{
		return true;
	}
	return false;

}

bool UJsonObj::SetBoolField(const FString& FieldName, bool InValue)
{
	if (!this->IsValid())
	{
		return false;
	}
	Obj->SetBoolField(FieldName, InValue);
	return true;
}

bool UJsonObj::SetIntegerField(const FString& FieldName, int32 Number)
{
	if (!this->IsValid())
	{
		return false;
	}
	Obj->SetNumberField(FieldName, Number);
	return true;
}

bool UJsonObj::SetNumberField(const FString& FieldName, float Number)
{
	if (!this->IsValid())
	{
		return false;
	}

	Obj->SetNumberField(FieldName, Number);
	return true;
}

bool UJsonObj::SetStringField(const FString& FieldName, const FString& StringValue)
{
	if (!this->IsValid())
	{
		return false;
	}
	Obj->SetStringField(FieldName, StringValue);
	return true;
}

bool UJsonObj::SetObjectField(const FString& FieldName, const UJsonObj* JsonObject)
{
	if (!JsonObject || !this->IsValid() || !JsonObject->IsValid())
	{
		return false;
	}
	Obj->SetObjectField(FieldName, JsonObject->GetJsonObj());
	return true;
}

TSharedPtr<FJsonObject> UJsonObj::GetJsonObj() const
{
	return this->Obj;
}

bool UJsonObj::SetArrayField(const FString& FieldName, const TArray<TSharedPtr<FJsonValue>>& Array)
{
	if (!this->IsValid())
	{
		return false;
	}
	Obj->SetArrayField(FieldName, Array);
	return true;
}

bool UJsonObj::SetObjectArrayField(const FString& FieldName, const TArray<UJsonObj*>& InArray)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;
	for (const UJsonObj* elem : InArray)
	{
		TSharedPtr<FJsonValueObject> NewObj = MakeShareable(new FJsonValueObject(elem->GetJsonObj()));
		if (!NewObj.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("UJsonObj::SetObjectArrayField------Failed:Object is not VALID!!"));
			return false;
		}
		ValueArray.Add(NewObj);
	}
	SetArrayField(FieldName, ValueArray);
	return true;

}

bool UJsonObj::SetNumberArrayField(const FString& FieldName, const TArray<float>& InArray)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;
	for (const float& elem : InArray)
	{
		TSharedPtr<FJsonValueNumber> NewObj = MakeShareable(new FJsonValueNumber(elem));
		if (!NewObj.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("UJsonObj::SetObjectArrayField------Failed:Object is not VALID!!"));
			return false;
		}
		ValueArray.Add(NewObj);
	}
	SetArrayField(FieldName, ValueArray);
	return true;
}

bool UJsonObj::SetIntegerArrayField(const FString& FieldName, const TArray<int32>& InArray)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;
	for (const int32& elem : InArray)
	{
		TSharedPtr<FJsonValueNumber> NewObj = MakeShareable(new FJsonValueNumber(elem));
		if (!NewObj.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("UJsonObj::SetObjectArrayField------Failed:Object is not VALID!!"));
			return false;
		}
		ValueArray.Add(NewObj);
	}
	SetArrayField(FieldName, ValueArray);
	return true;
}

bool UJsonObj::SetStringArrayField(const FString& FieldName, const TArray<FString>& InArray)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;
	for (const FString& elem : InArray)
	{
		TSharedPtr<FJsonValueString> NewObj = MakeShareable(new FJsonValueString(elem));
		if (!NewObj.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("UJsonObj::SetObjectArrayField------Failed:Object is not VALID!!"));
			return false;
		}
		ValueArray.Add(NewObj);
	}
	SetArrayField(FieldName, ValueArray);
	return true;
}

bool UJsonObj::SetBoolArrayField(const FString& FieldName, const TArray<bool>& InArray)
{
	TArray<TSharedPtr<FJsonValue>> ValueArray;
	for (const bool& elem : InArray)
	{
		TSharedPtr<FJsonValueBoolean> NewObj = MakeShareable(new FJsonValueBoolean(elem));
		if (!NewObj.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("UJsonObj::SetObjectArrayField------Failed:Object is not VALID!!"));
			return false;
		}
		ValueArray.Add(NewObj);
	}
	SetArrayField(FieldName, ValueArray);
	return true;
}


bool UJsonObj::GetVector3DField(const FString& FieldName, FVector& OutVector) const
{
	UJsonObj* JsonObject = this->GetObjectField(FieldName);

	if (!JsonObject)
	{
		return false;
	}

	float X = 0; 
	float Y = 0;
	float Z = 0;

	if (!JsonObject->GetNumberField("X", X))
	{
		return false;
	}

	if (!JsonObject->GetNumberField("Y", X))
	{
		return false;
	}

	if (!JsonObject->GetNumberField("Z", X))
	{
		return false;
	}

	OutVector = FVector(X, Y, Z);
	return true;
}

bool UJsonObj::SetVector3DField(const FString& FieldName, const FVector& InVector)
{
	UJsonObj* JsonObj = UJsonObjectFactory::ConstructJsonObject();

	if (!JsonObj)
	{
		return false;
	}

	JsonObj->SetNumberField("X", InVector.X);
	JsonObj->SetNumberField("Y", InVector.Y);
	JsonObj->SetNumberField("Z", InVector.Z);

	this->SetObjectField(FieldName, JsonObj);
	return true;
}

bool UJsonObj::GetVector2DField(const FString& FieldName, FVector2D& OutVector2D) const
{
	UJsonObj* JsonObject = this->GetObjectField(FieldName);

	if (!JsonObject)
	{
		return false;
	}

	float X = 0;
	float Y = 0;

	if (!JsonObject->GetNumberField("X", X))
	{
		return false;
	}

	if (!JsonObject->GetNumberField("Y", X))
	{
		return false;
	}

	OutVector2D = FVector2D(X, Y);
	return true;
}

bool UJsonObj::SetVector2DField(const FString& FieldName, const FVector2D& InVector2D)
{
	UJsonObj* JsonObj = UJsonObjectFactory::ConstructJsonObject();

	if (!JsonObj)
	{
		return false;
	}

	JsonObj->SetNumberField("X", InVector2D.X);
	JsonObj->SetNumberField("Y", InVector2D.Y);

	this->SetObjectField(FieldName, JsonObj);

	return true;
}

bool UJsonObj::GetRotatorField(const FString& FieldName, FRotator& OutRotator) const
{
	UJsonObj* JsonObject = this->GetObjectField(FieldName);

	if (!JsonObject)
	{
		return false;
	}

	float Pitch = 0;
	float Yaw = 0;
	float Roll = 0;

	if (!JsonObject->GetNumberField("Pitch", Pitch))
	{
		return false;
	}

	if (!JsonObject->GetNumberField("Yaw", Yaw))
	{
		return false;
	}

	if (!JsonObject->GetNumberField("Roll", Roll))
	{
		return false;
	}
	OutRotator = FRotator(Pitch, Yaw, Roll);
	return true;
}

bool UJsonObj::SetRotatorField(const FString& FieldName, const FRotator& InRotator)
{
	UJsonObj* JsonObj = UJsonObjectFactory::ConstructJsonObject();

	if (!JsonObj)
	{
		return false;
	}

	JsonObj->SetNumberField("Pitch", InRotator.Pitch);
	JsonObj->SetNumberField("Yaw", InRotator.Yaw);
	JsonObj->SetNumberField("Roll", InRotator.Roll);

	this->SetObjectField(FieldName, JsonObj);

	return true;
}

bool UJsonObj::GetTransformField(const FString& FieldName, FTransform& OutTransform) const
{
	UJsonObj* JsonObject = this->GetObjectField(FieldName);

	if (!JsonObject)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	FVector Scale3D;

	if (!JsonObject->GetVector3DField("Location", Location))
	{
		return false;
	}
	if (!JsonObject->GetRotatorField("Rotation", Rotation))
	{
		return false;
	}
	if (!JsonObject->GetVector3DField("Scale", Scale3D))
	{
		return false;
	}

	OutTransform.SetLocation(Location);
	OutTransform.SetRotation(Rotation.Quaternion());
	OutTransform.SetScale3D(Scale3D);
	return true;
}

bool UJsonObj::SetTransformField(const FString& FieldName, const FTransform& InTransform)
{
	UJsonObj* JsonObj = UJsonObjectFactory::ConstructJsonObject();

	if (!JsonObj->SetVector3DField("Location", InTransform.GetLocation()))
	{
		return false;
	}

	if (!JsonObj->SetRotatorField("Location", InTransform.GetRotation().Rotator()))
	{
		return false;
	}

	if (!JsonObj->SetVector3DField("Scale", InTransform.GetScale3D()))
	{
		return false;
	}
	
	this->SetObjectField(FieldName, JsonObj);
	return true;
}

FString UJsonObj::ToString()
{
	FString JsonStr;
	if (!Obj.IsValid())
	{
		return JsonStr;
	}
	if (Obj->Values.Num() > 0)
	{
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&JsonStr);
		FJsonSerializer::Serialize(Obj.ToSharedRef(), t_writer);

		return JsonStr;
	}

	return JsonStr;
}


