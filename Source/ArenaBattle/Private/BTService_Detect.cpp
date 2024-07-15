// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
    NodeName = TEXT("Detect");
    Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
    // AI가 제어하는 폰을 가져온다
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn) return;

    // 월드 컨텍스트를 가져온다
    UWorld* World = ControllingPawn->GetWorld();
    if (nullptr == World) return;

    // 감지할 반경, 중심점을 정의한다
    FVector Center = ControllingPawn->GetActorLocation();
    float DetectRadius = 600.0f;

    // 지정된 반경 내 오버랩 쿼리를 사용하여 타겟을 찾는다
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
    bool bResult = World->OverlapMultiByChannel(
        OverlapResults,
        Center,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(DetectRadius),
        CollisionQueryParam
    );
    if (bResult)  // 감지했으면 이를 실행
    {
        for (auto const& OverlapResult : OverlapResults)
        {
            // 캐릭터 타입인데 오버랩된 액터가 플레이어의 것인지
            AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
            if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
            {
                //발견된 캐릭터를 비헤이비어 트리의 블랙보드에 타겟으로 설정
                OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);
                //디버깅 위해 그림 그리기
                DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
                DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
                DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
                return;
            }
        }
    }
    DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}

