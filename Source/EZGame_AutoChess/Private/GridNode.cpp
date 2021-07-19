// Fill out your copyright notice in the Description page of Project Settings.


#include "GridNode.h"
#include "GridMap.h"

//��ʼ������
void UGridNode::InitNode(AGridMap* InGridMap, FVector InLocation, FGridVector InCoordinate, float InSize)
{
	GridMap = InGridMap;
	Location = InLocation;
	Coordinate = InCoordinate;
	Size = InSize;
}

//��ȡ�������
TArray<UGridNode*> UGridNode::GetNeighbors()
{
	TArray<UGridNode*> Nodes;
	return Nodes;
}

//�ж�����Ƿ���ͨ��
bool UGridNode::CanPass(AActor* InActor) const
{
	if (PassFlag >= ENodePassFlag::Block)
		return false;
	//�ж��Ƿ�����������վ�ڵ�ǰ�����
	if (NodeActors.Num() > 0 && InActor)
	{
		if (NodeActors.Contains(InActor))
			return true;
		else
			return false;
	}
	return true;
}

//Ԥ�����
void UGridNode::BookNode(AActor* InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);
		GridMap->ResetNodeMaterial(this);
	}
}

//ȡ��Ԥ�����
void UGridNode::CancelBookNode(AActor* InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);
		GridMap->ResetNodeMaterial(this);
	}
}

//�������
void UGridNode::EnterNode(AActor* InActor)
{
	if (InActor)
	{
		NodeActors.AddUnique(InActor);
		GridMap->ResetNodeMaterial(this);
	}
}

//�뿪���
void UGridNode::LeaveNode(AActor* InActor)
{
	if (InActor && NodeActors.Contains(InActor))
	{
		NodeActors.Remove(InActor);
		GridMap->ResetNodeMaterial(this);
	}
}
