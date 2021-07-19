// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
#include "HexNode.generated.h"

/**
 * 
 */
UCLASS()
class UHexNode : public UGridNode
{
	GENERATED_BODY()
	
public:
	//����-���캯��
	UHexNode();

	//����-�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* RightUpNode;

	//����-�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* RightNode;

	//����-�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* RightDownNode;

	//����-�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* LeftDownNode;

	//����-�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* LeftNode;

	//����-�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHexNode* LeftUpNode;

	//����-��ȡ�������-����
	virtual TArray<UGridNode*> GetNeighbors() override;

	//Ѱ·-��ȡ��������ʵ�ʾ���-����
	virtual float GetRealRadiusSize() const override { return Size * FMath::Sqrt(3) * 0.5; };

	//ģ��-����ģ��-����
	virtual void DrawNode(TArray<FVector>& InVertices,
		TArray<int32>& InIndecies,
		TArray<FVector>& InNormals,
		TArray<FVector2D>& InUV,
		TArray<FColor>& InVertexColors,
		TArray<FVector>& InTangents,
		FVector InOffset) override;

};
