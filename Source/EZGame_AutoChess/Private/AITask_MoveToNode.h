// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "AITask_MoveToNode.generated.h"

class AAIController;
class UGridNode;

/**
 * 
 */
UCLASS()
class UAITask_MoveToNode : public UAITask
{
	GENERATED_BODY()

public:
	//����Task����-��̬
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Controller", BlueprintInternalUseOnly = "TRUE", DisplayName = "Move To Node"))
	static UAITask_MoveToNode* AIMoveTo(AAIController* Controller, UGridNode* GoalNode, AActor* GoalActor, int StopOnStep = 1);

	//Ŀ���ɫ
	UPROPERTY()
	AActor* GoalActor;

	//�������
	UPROPERTY()
	UGridNode* FromNode;

	//�յ����
	UPROPERTY()
	UGridNode* ToNode;

	//ֹͣ����
	UPROPERTY()
	int StopOnStep;

	//ǿ�ƿ�������
	void ForceActiveTask();

	//ǿ�ƽ�������
	void ForceEndTask();

protected:
	//ί��-�ɹ�����
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnRequestSuccess;

	//ί��-ʧ�ܽ���
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnRequestFailed;

	//����������
	virtual void Activate() override;

	//ִ���ƶ�����
	UFUNCTION()
	void PerformMove();

	//�ɹ����������
	UFUNCTION()
	void RequestSuccess();

	//ʧ�����������
	UFUNCTION()
	void RequestFailed();

	//����Ŀ���ɫ��ȡ�����
	UGridNode* GetGridNode(AActor* InActor, bool bIncldeBookNode = false) const;

};
