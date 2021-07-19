// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "AITask_TurnTo.generated.h"

class UGridNode;

/**
 * 
 */
UCLASS()
class EZGAME_AUTOCHESS_API UAITask_TurnTo : public UAITask
{
	GENERATED_BODY()

public:
	//����Task����-��̬
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Controller", BlueprintInternalUseOnly = "TRUE", DisplayName = "Turn To"))
	static UAITask_TurnTo* AITurnTo(AAIController* Controller, AActor* GoalActor, UGridNode* GoalNode, FRotator GoalRotation, float StopOnDegree = 10);

	//��תĿ���ɫ
	UPROPERTY()
	AActor* GoalActor;

	//��תĿ�����
	UPROPERTY()
	UGridNode* GoalNode;

	//��תĿ��ֵ
	UPROPERTY()
	FRotator GoalRotation;

	//��ת�ݲ�ֵ
	UPROPERTY()
	float StopOnDegree;

	//ǿ��ִ��
	void ForceActiveTask();

	//ǿ��ʧ��
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

	//ִ����ת����
	UFUNCTION()
	void PerformRotate();

	//�ɹ����������
	UFUNCTION()
	void RequestSuccess();

	//ʧ�����������
	UFUNCTION()
	void RequestFailed();
	
};
