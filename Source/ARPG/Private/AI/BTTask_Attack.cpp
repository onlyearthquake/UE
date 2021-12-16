// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "AI/BaseEnemyAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ABaseEnemyAI* AICharacter = Cast<ABaseEnemyAI>(AIController->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		AICharacter->Attack(TargetActor);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

UBTTask_Attack::UBTTask_Attack()
{
}
