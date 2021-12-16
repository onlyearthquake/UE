
#include "AI/BTService_CheckHealth.h"

#include "AIController.h"
#include "AttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckHealth::UBTService_CheckHealth()
{
}

void UBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp))
		{
			const bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetMaxHealth()) < LowHealthFraction;

			UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
			BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}
