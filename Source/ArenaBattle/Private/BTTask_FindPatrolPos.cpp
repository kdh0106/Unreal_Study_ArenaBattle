// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "ArenaBattle.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // AI��Ʈ�ѷ��� �����ϴ� ���� �����´�
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); //�׺���̼� �ý����� �����´�
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey); //HomePosKey�� ����� ��ġ�� �����´�
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol)) //�־��� �ݰ� ������ Ž�������� ��ġ�� ã���� NextPatrol�� ��ġ�� ����
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatrolPosKey, NextPatrol.Location); //PatrolPosKey�� �ش� ��ġ�� ����
		ABLOG(Warning, TEXT("PatrolPos : %s"), *NextPatrol.Location.ToString())
		return EBTNodeResult::Succeeded; //������ ��ȯ
	}
	return EBTNodeResult::Failed;
}