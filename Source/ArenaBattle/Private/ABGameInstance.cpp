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
	//���츦 ���ڿ��� ��ȯ�� �� ������ ���̺��� �ش� ���� ã��
	//FromInt�� FString Ŭ������ �Լ���, ������ FString���� ��ȯ�ϴ� ����� ��
}