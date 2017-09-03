// Fill out your copyright notice in the Description page of Project Settings.

#include "GitGroupAgentAIController.h"
#include "GitGroupAgent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/Waypoint.h"
#include "Utilities/GitStatics.h"
#include "Utilities/GitCore.h"


void AGitGroupAgentAIController::Patrol(EPatrolType PatrolType /*= EPatrolType::PT_PingPong*/, float AcceptanceRadius /*= 100.0f*/)
{
	AGitGroupAgent* GitGroupAgent = Cast<AGitGroupAgent>(GetPawn());

	if (GitGroupAgent == nullptr)
	{
		PRINTC("Pawn is not GitGroupAgent. (GitGroupAgentAIController, Patrol)", FColor::Red);
		return;
	}	

	// Removes all waypoints from the list that are not valid.		
	TArray<AWaypoint*> WaypointsToDelete;
	auto Node = GitGroupAgent->Waypoints.GetHead();	
	while (Node != nullptr)
	{
		if (Node->GetValue()->IsValidLowLevel())
		{
			if (IsValid(Node->GetValue()) == false)
			{
				WaypointsToDelete.Add(Node->GetValue());
			}
		}
		else
		{
			WaypointsToDelete.Add(Node->GetValue());
		}
				
		Node = Node->GetNextNode();
	}

	for (const auto& Waypoint : WaypointsToDelete)
	{
		// Before deleting current waypoint, redirect to another node.
		if (CurrentWaypoint == Waypoint)
		{
			if (CurrentTraverseDirection == ETraverseDirection::TD_Forward)
			{	
				// Let's hope this doesn't crash.
				// If current waypoint is invalid, try to redirect to the next waypoint.
				if (AWaypoint* NextWaypoint = GitGroupAgent->Waypoints.FindNode(Waypoint) ? GitGroupAgent->Waypoints.FindNode(Waypoint)->GetNextNode() ? GitGroupAgent->Waypoints.FindNode(Waypoint)->GetNextNode()->GetValue() : nullptr : nullptr)
				{
					CurrentWaypoint = NextWaypoint;
				}
				else // If there is no next waypoint, try to redirect to the previous.
				{
					// If this fails, it means there are no nodes in the linked list, so CurrentWaypoint = nullptr
					CurrentWaypoint = GitGroupAgent->Waypoints.FindNode(Waypoint) ? GitGroupAgent->Waypoints.FindNode(Waypoint)->GetPrevNode() ? GitGroupAgent->Waypoints.FindNode(Waypoint)->GetPrevNode()->GetValue() : nullptr : nullptr;					
				}
			}
			else if(CurrentTraverseDirection == ETraverseDirection::TD_Backward)
			{			
				// same as the above, but reversed.
				if (AWaypoint* NextWaypoint = GitGroupAgent->Waypoints.FindNode(Waypoint) ? GitGroupAgent->Waypoints.FindNode(Waypoint)->GetPrevNode() ? GitGroupAgent->Waypoints.FindNode(Waypoint)->GetPrevNode()->GetValue() : nullptr : nullptr)
				{
					CurrentWaypoint = NextWaypoint;
				}
				else
				{
					CurrentWaypoint = GitGroupAgent->Waypoints.FindNode(Waypoint) ? GitGroupAgent->Waypoints.FindNode(Waypoint)->GetNextNode() ? GitGroupAgent->Waypoints.FindNode(Waypoint)->GetNextNode()->GetValue() : nullptr : nullptr;					
				}
			}			
		}
		// Remove invalid waypoint from the linked list.
		GitGroupAgent->Waypoints.RemoveNode(Waypoint);
	}		

	if (IsValid(CurrentWaypoint) == false)
	{
		if (GitGroupAgent->Waypoints.Num() > 0)
		{
			CurrentWaypoint = GitGroupAgent->Waypoints.GetHead()->GetValue();
		}
		else
		{	
			StopMovement();
			return;
		}
	}
	
	const FVector AgentLocation = GitGroupAgent->GetActorLocation();
	const FVector TargetLocation = CurrentWaypoint->GetActorLocation();
	const float DistanceToTarget = UKismetMathLibrary::VSize(TargetLocation - AgentLocation);		
	if (DistanceToTarget <= AcceptanceRadius + 50.0f)
	{
		if (GitGroupAgent->Waypoints.Num() == 1)
		{
			return;
		}

		switch (PatrolType)
		{
			case EPatrolType::PT_PingPong:
			{
				if (CurrentTraverseDirection == ETraverseDirection::TD_Forward)
				{
					if (AWaypoint* NextWaypoint = GitGroupAgent->Waypoints.FindNode(CurrentWaypoint) ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetNextNode() ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetNextNode()->GetValue() : nullptr : nullptr)
					{
						CurrentWaypoint = NextWaypoint;
					}
					else
					{
						CurrentWaypoint = GitGroupAgent->Waypoints.FindNode(CurrentWaypoint) ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetPrevNode() ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetPrevNode()->GetValue() : nullptr : nullptr;
						CurrentTraverseDirection = ETraverseDirection::TD_Backward;
					}
				}
				else if (CurrentTraverseDirection == ETraverseDirection::TD_Backward)
				{
					if (AWaypoint* NextWaypoint = GitGroupAgent->Waypoints.FindNode(CurrentWaypoint) ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetPrevNode() ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetPrevNode()->GetValue() : nullptr : nullptr)
					{
						CurrentWaypoint = NextWaypoint;
					}
					else
					{
						CurrentWaypoint = GitGroupAgent->Waypoints.FindNode(CurrentWaypoint) ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetNextNode() ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetNextNode()->GetValue() : nullptr : nullptr;
						CurrentTraverseDirection = ETraverseDirection::TD_Forward;
					}
				}
			}
			case EPatrolType::PT_Circular:
			{
				if (AWaypoint* NextWaypoint = GitGroupAgent->Waypoints.FindNode(CurrentWaypoint) ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetNextNode() ? GitGroupAgent->Waypoints.FindNode(CurrentWaypoint)->GetNextNode()->GetValue() : nullptr : nullptr)
				{
					CurrentWaypoint = NextWaypoint;
				}
				else
				{
					CurrentWaypoint = GitGroupAgent->Waypoints.GetHead()->GetValue();
				}
			}
		}

		
	}	
	
	MoveToLocation(CurrentWaypoint->GetActorLocation(), AcceptanceRadius);
}

