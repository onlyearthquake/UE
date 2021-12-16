// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			const AAIController* AIController = OwnerComp.GetAIOwner();

			APawn* AIPawn = AIController->GetPawn();
			if (AIPawn != nullptr)
			{
				float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
				//UE_LOG(LogTemp,Log,TEXT("%f"),DistanceTo)
				bool bWithinRange = DistanceTo < MaxAttackRange;

				bool bHasLOS = false;
				if (bWithinRange)
				{
					bHasLOS =AIController->LineOfSightTo(TargetActor);
				}

				BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				BlackBoardComp->SetValueAsVector("MoveDir",TargetActor->GetActorLocation());
			}
		}
	}
}

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
}
