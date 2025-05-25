#include "UIWidgetCore.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "MovieScene.h"

void UIWidgetCore::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯 초기화 로직
}

void UIWidgetCore::NativeDestruct()
{
	// 해제 또는 정리 로직
	Super::NativeDestruct();
}
	
FName UIWidgetCore::GetBPName() const
{
	if (UClass* pClass = GetClass())
	{
		return (*pClass->GetName().LeftChop(2));
	}

	return NAME_None;
}

bool UIWidgetCore::IsActive()
{
	return IsInViewport();
}


void UIWidgetCore::Show(const ESlateVisibility visibility)
{	
	ShowInternal(visibility);
}

void UIWidgetCore::Hide(ESlateVisibility visibility /*= ESlateVisibility::Collapsed*/)
{
	HideInternal(visibility);
}

void UIWidgetCore::ShowInternal(ESlateVisibility visibility)
{
	
	SetVisibility(visibility);

	ProcShowAnimation();
	StopAutoLoopAnimation();
}

void UIWidgetCore::HideInternal(ESlateVisibility visibility)
{
	ProcHideAnimation();

	SetVisibility(visibility);

		
}

bool UIWidgetCore::IsPlayingAnimation(UWidgetAnimation* InAnimation)
{
	if (InAnimation == nullptr)
		return false;
	UUMGSequencePlayer* pPlayer = GetOrAddSequencePlayer(InAnimation);
	if (pPlayer == nullptr)
		return false;
	return pPlayer->GetPlaybackStatus() == EMovieScenePlayerStatus::Playing;
}

UUMGSequencePlayer* UIWidgetCore::PlayAnimation(UWidgetAnimation* InAnimation, float StartAtTime /*= 0.0f*/, int32 NumLoopsToPlay /*= 1*/, EUMGSequencePlayMode::Type PlayMode /*= EUMGSequencePlayMode::Forward*/, float PlaybackSpeed /*= 1.0f*/, bool bRestoreState /*= false*/)
{
	if (InAnimation == nullptr || IsGarbageCollecting() || FUObjectThreadContext::Get().IsRoutingPostLoad)
		return nullptr;

	return Super::PlayAnimation( InAnimation, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed, bRestoreState);
}

UUMGSequencePlayer* UIWidgetCore::PlayAnimation(const FName& animationName, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed, bool bRestoreState)
{
	if ( UWidgetAnimation* widgetAnimation = FindAnimation(animationName) )
	{
		return PlayAnimation(widgetAnimation, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed, bRestoreState);
	}
	return nullptr;
}

UUMGSequencePlayer* UIWidgetCore::StopAnimation(UWidgetAnimation* inAnimation, EStopAnimType type, bool bRecvEvent /*= false*/, bool bStopAniExecOnlyPlaying /*= false*/)
{
	if (inAnimation == nullptr)
		return nullptr;

	UUMGSequencePlayer* pPlayer = ((type == EStopAnimType::ToTargetPos) || bStopAniExecOnlyPlaying) ? GetSequencePlayer(inAnimation) : GetOrAddSequencePlayer(inAnimation);
	if (pPlayer == nullptr)
		return nullptr;

	pPlayer->Stop();

	if (type != EStopAnimType::ToDefault)
	{
		EUMGSequencePlayMode::Type playMode = EUMGSequencePlayMode::Forward;
		switch (type)
		{
		case EStopAnimType::ToStartTime:
		{
			playMode = EUMGSequencePlayMode::Reverse;
		}break;
		case EStopAnimType::ToEndTime:
		{
			playMode = EUMGSequencePlayMode::Forward;
		}break;
		case EStopAnimType::ToTargetPos:
		{
			playMode = pPlayer->IsPlayingForward() ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse;
		}break;
		}

		pPlayer->PlayTo(inAnimation->GetEndTime(), inAnimation->GetEndTime(), 1, playMode, 0.f, false);
	}

	return pPlayer;
}

UUMGSequencePlayer* UIWidgetCore::StopAnimation(const FName& animationName, EStopAnimType type, bool bRecvEvent, bool bStopAniExecOnlyPlaying)
{
	if ( UWidgetAnimation* widgetAnimation = FindAnimation(animationName) )
	{
		return StopAnimation(widgetAnimation, type, bRecvEvent, bStopAniExecOnlyPlaying);
	}
	return nullptr;
}

bool UIWidgetCore::IsPlayingAnimationName(const FName& animationName)
{
	if ( UWidgetAnimation* widgetAnimation = FindAnimation(animationName) )
	{
		return IsAnimationPlaying(widgetAnimation);
	}
	return false;
}

UUMGSequencePlayer* UIWidgetCore::SetAnimationTime(UWidgetAnimation* InAnimation, float AtTime, EUMGSequencePlayMode::Type PlayMode /*= EUMGSequencePlayMode::Forward*/)
{
	if (nullptr == InAnimation)
		return nullptr;

	UUMGSequencePlayer* pPlayer = GetOrAddSequencePlayer(InAnimation);
	if (pPlayer == nullptr)
		return nullptr;

	float fPlayTime = 0.f;

	switch (PlayMode)
	{
	case EUMGSequencePlayMode::Forward:
		fPlayTime = AtTime;
		break;

	case EUMGSequencePlayMode::Reverse:
		fPlayTime = InAnimation->GetEndTime() - AtTime;
		break;

	case EUMGSequencePlayMode::PingPong:
		return pPlayer;

	default:
		break;
	}

	pPlayer->PlayTo(fPlayTime, fPlayTime, 1, PlayMode, 0.f, false);

	return pPlayer;
}


void UIWidgetCore::StopAllAnimations()
{
	for (UUMGSequencePlayer* FoundPlayer : ActiveSequencePlayers)
	{
		if (FoundPlayer->GetPlaybackStatus() != EMovieScenePlayerStatus::Playing)
			continue;
	}

	Super::StopAllAnimations();
}


UWidgetAnimation* UIWidgetCore::FindAnimation(const FName& animationName)
{
	if ( UWidgetBlueprintGeneratedClass* widgetClass = GetWidgetTreeOwningClass() )
	{
		for ( auto&& animation : widgetClass->Animations )
		{
			if ( animation != nullptr )
			{
				FName animName = GetAnimationName(animation);
				if ( animName.IsEqual(animationName) )
				{
					return animation;
				}
			}
		}
	}
	return nullptr;
}

FName UIWidgetCore::GetAnimationName(UWidgetAnimation* animation)
{
	if ( animation != nullptr && animation->GetMovieScene() != nullptr )
		return animation->GetMovieScene()->GetFName();
	return NAME_None;
}

void UIWidgetCore::ClearStopSequencePlayersIfMany()
{
	if (StoppedSequencePlayers.Num() > 500)
	{
		PostTickActionsAndAnimation(0);
	}
}

void UIWidgetCore::OnAnimationStarted_Implementation(const UWidgetAnimation* pAnimation)
{
		UIOnAnimationStarted(pAnimation);
}

void UIWidgetCore::OnAnimationFinished_Implementation(const UWidgetAnimation* pAnimation)
{
	// Hide() 보다 먼저 실행해야 한다.
	// Hide() 에서 NativeDestruct() 까지 호출되서, TryGetUseAudioEventInAnimation() 가 동작 안 하게 되는 현상 발생.
	UIOnAnimationFinished(pAnimation);
	
}

void UIWidgetCore::UIOnAnimationStarted(const UWidgetAnimation* pAnimation)
{

}
void UIWidgetCore::UIOnAnimationFinished(const UWidgetAnimation* pAnimation)
{
	
}

void UIWidgetCore::ProcShowAnimation()
{
	if (nullptr == Anim_Show)
		return;

	if (IsAnimationPlaying(Anim_Hide))
	{
		//Hide 도중에 Show 되는 상황이면 끈다.
		StopAnimation(Anim_Hide);
	}
	PlayAnimation(Anim_Show);
}

bool UIWidgetCore::ProcHideAnimation()
{
	if (IsAnimationPlaying(Anim_Show))
	{
		//Show 도중에 Hide 되는 상황이면 그냥 끈다.
		StopAnimation(Anim_Show);
		return false;
	}

	if (nullptr == Anim_Hide)
	{
		return false;
	}

	//처음 꺼질때만 동작된다. + Hide 한 이후에 바로 Hide 하면 ProcHideAnimation를 true로 해줘야 Hide 애니가 끝까지 동작된다.
	if (IsActive() == false)
	{
		return IsAnimationPlaying(Anim_Hide);
	}

	if (IsAnimationPlaying(Anim_Hide))
	{
		//Hide 애니메이션 플레이중이면 다시 플레이하지 않는다.
		return true;
	}

	return (PlayAnimation(Anim_Hide) != nullptr);
}

void UIWidgetCore::PlayAutoLoopAnimation()
{
	if (	Anim_Auto_Loop && 
			bPlayAutoLoopAnimAutomatically && 
			IsAnimationPlaying(Anim_Auto_Loop) == false)
	{
		PlayAnimation(Anim_Auto_Loop, 0, 0);
	}
}

void UIWidgetCore::StopAutoLoopAnimation()
{
	if (Anim_Auto_Loop)
	{
		StopAnimation(Anim_Auto_Loop, EStopAnimType::ToDefault, false, true);
	}
}