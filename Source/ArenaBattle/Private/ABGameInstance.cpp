 // Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"

UABGameInstance::UABGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);
	ABCHECK(DT_ABCHARACTER.Succeeded());
	ABCharacterTable = DT_ABCHARACTER.Object;
}

void UABGameInstance::Init()
{
	Super::Init();
	ABLOG(Warning, TEXT("EXP : %d"), GetABCharacterData(15)->DropExp);
}

FABCharacterData* UABGameInstance::GetABCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
	//저우를 문자열로 변환한 후 데이터 테이블에서 해당 행을 찾음
	//FromInt는 FString 클래스의 함수로, 정수를 FString으로 변환하는 기능을 함
}