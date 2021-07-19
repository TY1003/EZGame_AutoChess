// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNode.h"
#include "GameFramework/MovementComponent.h"
#include "ChessMovementComponent.generated.h"


USTRUCT(BlueprintType)
struct  FRotateTarget
{
public:
	GENERATED_USTRUCT_BODY()

	//�Ƿ񼤻�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive = false;

	//��תĿ���ɫ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;

	//��תĿ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGridNode* TargetNode;

	//��תĿ��ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TargetRotation;

	//���Ƕ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopOnDegree = 0;

	FRotateTarget() {};

	//��������-����Ƕ�ֵ
	FRotateTarget(AActor* InActor, float InStopOnDegree)
	{
		this->bActive = true;
		this->TargetActor = InActor;
		this->TargetNode = nullptr;
		this->TargetRotation = FRotator::ZeroRotator;
		this->StopOnDegree = InStopOnDegree;
	}

	//��������-�������
	FRotateTarget(UGridNode* InNode, float InStopOnDegree)
	{
		this->bActive = true;
		this->TargetActor = nullptr;
		this->TargetNode = InNode;
		this->TargetRotation = FRotator::ZeroRotator;
		this->StopOnDegree = InStopOnDegree;
	}

	//��������-�����ɫ
	FRotateTarget(FRotator InRotator, float InStopOnDegree)
	{
		this->bActive = true;
		this->TargetActor = nullptr;
		this->TargetNode = nullptr;
		this->TargetRotation = InRotator;
		this->StopOnDegree = InStopOnDegree;
	}

	//��ǰ�Ƿ񼤻�
	bool IsValid() const
	{
		return bActive;
	}

	//��ȡĿ��Rotation
	FRotator GetRotation(FVector InLocation) const
	{
		if (TargetActor)
		{
			return FRotationMatrix::MakeFromX(TargetActor->GetActorLocation() - InLocation).Rotator();
		}
		else if (TargetNode)
		{
			return FRotationMatrix::MakeFromX(TargetNode->Location - InLocation).Rotator();
		}
		else
		{
			return TargetRotation;
		}
	}

	//����
	void Reset()
	{
		bActive = false;
		TargetActor = nullptr;
		TargetNode = nullptr;
		TargetRotation = FRotator::ZeroRotator;
		StopOnDegree = 0;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveDelegate);

UCLASS(meta = (BlueprintSpawnableComponent), HideCategories = Velocity)
class UChessMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	//Tick����
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//�������
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	//����ֹͣ
	virtual void StopMovementImmediately() override;

public:

	//��ǰ���
	UPROPERTY(BlueprintReadOnly)
	UGridNode* NowNode;

	//Ԥ�����
	UPROPERTY(BlueprintReadOnly)
	UGridNode* BookNode;

	//�ƶ����ջ
	UPROPERTY(BlueprintReadWrite)
	TArray<UGridNode*> MoveStack;

	//��תĿ��
	UPROPERTY(BlueprintReadWrite)
	FRotateTarget RotateTarget;

	//�ƶ��ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMoveSpeed = 300.f;

	//��ת�ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRotationRate = 360.f;

	//���õ�ǰ���
	UFUNCTION(BlueprintCallable)
	void SetNowNode(UGridNode* InNode);

	//����Ԥ�����
	UFUNCTION(BlueprintCallable)
	void SetBookNode(UGridNode* InNode);

	//���㵱ǰ���
	UFUNCTION(BlueprintCallable)
	void CalcuNowNode();

	//�����ƶ�·��
	UFUNCTION(BlueprintCallable)
	void SetMovePath(TArray<UGridNode*> InPath, bool bNeedReverse = true);

	//ί��-��ʼ�ƶ�ʱ����
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnMoveBegin;

	//ί��-�����м�·��ʱ����
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnMoveCheckPoint;

	//ί��-�����յ�ʱ����
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnMoveComplete;

	//ί��-�����ת
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnRotateComplete;
	
};
