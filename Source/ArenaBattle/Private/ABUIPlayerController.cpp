// Fill out your copyright notice in the Description page of Project Settings.


#include "ABUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void AABUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass); //UIWidgetClass 유형의 새 위젯 인스턴스를 생성함.
	//UIWidgetClass는 UUserWidget의 서브클래스.

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;  //UI와 상호작용할 때 필수적임.
}
