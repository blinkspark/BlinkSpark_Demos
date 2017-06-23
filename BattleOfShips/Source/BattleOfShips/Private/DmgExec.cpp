// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleOfShips.h"
#include "BOS_AttributeSet.h"
#include "BOS_ShipBlock.h"
#include "AbilitySystemComponent.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DmgExec.h"

struct DMGAttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(HP); //The DECLARE_ATTRIBUTE_CAPTUREDEF macro actually only declares two variables. The variable names are dependent on the input, however. Here they will be HealthProperty(which is a UPROPERTY pointer)
										  //and HealthDef(which is a FGameplayEffectAttributeCaptureDefinition).
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackMultiplier); //Here AttackMultiplierProperty and AttackMultiplierDef. I hope you get the drill.
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefenseMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseAttackPower);

	DMGAttStruct()
	{
		// We define the values of the variables we declared now. In this example, HealthProperty will point to the Health attribute in the UMyAttributeSet on the receiving target of this execution. The last parameter is a bool, and determines if we snapshot the attribute's value at the time of definition.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBOS_AttributeSet, HP, Target, false);

		//This here is a different example: We still take the attribute from UMyAttributeSet, but this time it is BaseAttackPower, and we look at the effect's source for it. We also want to snapshot is because the effect's strength should be determined during its initial creation. A projectile wouldn't change
		//damage values depending on the source's stat changes halfway through flight, after all.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBOS_AttributeSet, BaseAttackPower, Source, true);

		//The same rules apply for the multiplier attributes too.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBOS_AttributeSet, AttackMultiplier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBOS_AttributeSet, DefenseMultiplier, Target, false);
	}
};


UDmgExec::UDmgExec(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DMGAttStruct Attributes;

	RelevantAttributesToCapture.Add(Attributes.HPDef); //RelevantAttributesToCapture is the array that contains all attributes you wish to capture, without exceptions. 
	//InvalidScopedModifierAttributes.Add(Attributes.HPDef); //However, an attribute added here on top of being added in RelevantAttributesToCapture will still be captured, but will not be shown for potential in-function modifiers in the GameplayEffect blueprint, more on that later.

	RelevantAttributesToCapture.Add(Attributes.BaseAttackPowerDef);
	RelevantAttributesToCapture.Add(Attributes.DefenseMultiplierDef);
	RelevantAttributesToCapture.Add(Attributes.AttackMultiplierDef);
}


void UDmgExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	DMGAttStruct Attributes;

	auto targetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	auto sourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	auto targetActor = targetASC ? targetASC->AvatarActor : nullptr;
	auto targetSB = targetActor ? Cast<ABOS_ShipBlock>(targetActor): nullptr;
	auto sourceActor = sourceASC ? sourceASC->AvatarActor : nullptr;
	auto sourceSB = sourceActor ? Cast<ABOS_ShipBlock>(sourceActor) : nullptr;


	const auto &spec = ExecutionParams.GetOwningSpec();

	auto sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	auto targetTags = spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = sourceTags;
	EvaluationParameters.TargetTags = targetTags;

	float HP = 0.f;
	/*ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.HealthDef, EvaluationParameters, Health);*/
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.HPDef, EvaluationParameters, HP);

	float BaseAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.BaseAttackPowerDef, EvaluationParameters, BaseAttackPower); // We do this for all other attributes, as well.

	float AttackMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.AttackMultiplierDef, EvaluationParameters, AttackMultiplier);

	float DefenseMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.DefenseMultiplierDef, EvaluationParameters, DefenseMultiplier);

	float dmg = BaseAttackPower * AttackMultiplier * DefenseMultiplier;

	dmg = dmg > HP ? HP : dmg;


	if (dmg > 0.f && targetSB && sourceSB
		&& targetSB->CanAttack()
		&& targetSB->TeamID != sourceSB->TeamID)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Attributes.HPProperty, EGameplayModOp::Additive, -dmg));
		UE_LOG(LogTemp, Warning, TEXT("dmg: %f"), dmg);

	}
	else if (dmg < 0.f&& targetSB && sourceSB
		&& targetSB->CanAttack()
		&& targetSB->TeamID == sourceSB->TeamID)
	{
		if (targetSB->AttributeSet->HP - dmg > targetSB->MaxHP)
		{
			dmg = targetSB->AttributeSet->HP - targetSB->MaxHP;
		}
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Attributes.HPProperty, EGameplayModOp::Additive, -dmg));
		UE_LOG(LogTemp, Warning, TEXT("dmg: %f"), dmg);
	}

}
