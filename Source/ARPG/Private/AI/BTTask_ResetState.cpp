// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ResetState.h"

#include "AIController.h"
#include "AI/BaseEnemyAI.h"

EBTNodeResult::Type UBTTask_ResetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ABaseEnemyAI* AICharacter = Cast<ABaseEnemyAI>(AIController->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		AICharacter->ReturnToIdle();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
