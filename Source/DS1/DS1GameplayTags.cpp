// Fill out your copyright notice in the Description page of Project Settings.


#include "DS1GameplayTags.h"

namespace DS1GameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Attacking, "Character.State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Rolling, "Character.State.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_GeneralAction, "Character.State.GeneralAction");

	UE_DEFINE_GAMEPLAY_TAG(Character_Action_Equip, "Character.Action.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_Unequip, "Character.Action.Unequip");

	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_LightOrRunning, "Character.Attack.LightOrRunning");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Light, "Character.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Special, "Character.Attack.Special");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Running, "Character.Attack.Running");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Heavy, "Character.Attack.Heavy");

	UE_DEFINE_GAMEPLAY_TAG(Notify_ComboWindow_Begin, "Notify.ComboWindow.Begin");
	UE_DEFINE_GAMEPLAY_TAG(Notify_ComboWindow_End, "Notify.ComboWindow.End");
	UE_DEFINE_GAMEPLAY_TAG(Notify_ComboWindow_Finished, "Notify.ComboWindow.Finished");
}
