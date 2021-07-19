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

	//是否激活
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive = false;

	//旋转目标角色
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;

	//旋转目标人物
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGridNode* TargetNode;

	//旋转目标值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TargetRotation;

	//误差角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopOnDegree = 0;

	FRotateTarget() {};

	//构建函数-传入角度值
	FRotateTarget(AActor* InActor, float InStopOnDegree)
	{
		this->bActive = true;
		this->TargetActor = InActor;
		this->TargetNode = nullptr;
		this->TargetRotation = FRotator::ZeroRotator;
		this->StopOnDegree = InStopOnDegree;
	}

	//构建函数-传入棋格
	FRotateTarget(UGridNode* InNode, float InStopOnDegree)
	{
		this->bActive = true;
		this->TargetActor = nullptr;
		this->TargetNode = InNode;
		this->TargetRotation = FRotator::ZeroRotator;
		this->StopOnDegree = InStopOnDegree;
	}

	//构建函数-传入角色
	FRotateTarget(FRotator InRotator, float InStopOnDegree)
	{
		this->bActive = true;
		this->TargetActor = nullptr;
		this->TargetNode = nullptr;
		this->TargetRotation = InRotator;
		this->StopOnDegree = InStopOnDegree;
	}

	//当前是否激活
	bool IsValid() const
	{
		return bActive;
	}

	//获取目标Rotation
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

	//重置
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
	//Tick函数
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//组件销毁
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	//立刻停止
	virtual void StopMovementImmediately() override;

public:

	//当前棋格
	UPROPERTY(BlueprintReadOnly)
	UGridNode* NowNode;

	//预定棋格
	UPROPERTY(BlueprintReadOnly)
	UGridNode* BookNode;

	//移动棋格栈
	UPROPERTY(BlueprintReadWrite)
	TArray<UGridNode*> MoveStack;

	//旋转目标
	UPROPERTY(BlueprintReadWrite)
	FRotateTarget RotateTarget;

	//移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMoveSpeed = 300.f;

	//旋转速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRotationRate = 360.f;

	//设置当前棋格
	UFUNCTION(BlueprintCallable)
	void SetNowNode(UGridNode* InNode);

	//设置预定棋格
	UFUNCTION(BlueprintCallable)
	void SetBookNode(UGridNode* InNode);

	//计算当前棋格
	UFUNCTION(BlueprintCallable)
	void CalcuNowNode();

	//设置移动路径
	UFUNCTION(BlueprintCallable)
	void SetMovePath(TArray<UGridNode*> InPath, bool bNeedReverse = true);

	//委托-开始移动时触发
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnMoveBegin;

	//委托-到达中间路点时触发
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnMoveCheckPoint;

	//委托-到达终点时触发
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnMoveComplete;

	//委托-完成旋转
	UPROPERTY(BlueprintAssignable)
	FMoveDelegate OnRotateComplete;
	
};
