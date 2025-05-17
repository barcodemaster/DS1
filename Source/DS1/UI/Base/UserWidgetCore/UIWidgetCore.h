#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIWidgetCore.generated.h"

class UWidgetAnimation;
class UUMGSequencePlayer;

enum class EStopAnimType
{
    ToDefault,
    ToStartTime,
    ToTargetPos,
    ToEndTime,
    Max
};

/**
 * Core user widget class to be used as a base for other widgets.
 */
UCLASS()
class DS1_API UIWidgetCore : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
    UWidgetAnimation* Anim_Show;
    UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
    UWidgetAnimation* Anim_Hide;
    UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
    UWidgetAnimation* Anim_Auto_Loop;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSUI|WidgetCore")
	bool bPlayAutoLoopAnimAutomatically = true;
	
protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;    
public:
    virtual void Show(const ESlateVisibility visibility = ESlateVisibility::SelfHitTestInvisible);
	virtual void Hide(ESlateVisibility visibility = ESlateVisibility::Collapsed);
protected: 
    virtual void ShowInternal(ESlateVisibility visibility);
    virtual void HideInternal(ESlateVisibility visibility);

public:
    FName GetBPName() const;
    bool IsActive();

    /** 애니메이션*/    
public: 
    bool IsPlayingAnimation(UWidgetAnimation* InAnimation);
    UUMGSequencePlayer* PlayAnimation(UWidgetAnimation* InAnimation, float StartAtTime = 0.0f, int32 NumLoopsToPlay = 1, EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward, float PlaybackSpeed = 1.0f, bool bRestoreState = false/*, bool bSmoothStart = true*/);	
    UUMGSequencePlayer* StopAnimation(UWidgetAnimation* inAnimation, EStopAnimType type = EStopAnimType::ToDefault, bool bRecvEvent = false, bool bStopAniExecOnlyPlaying = false);
    void StopAllAnimations(/*bool bForceToStart = false*/);

    UUMGSequencePlayer* PlayAnimation(const FName& animationName, float StartAtTime = 0.0f, int32 NumLoopsToPlay = 1, EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward, float PlaybackSpeed = 1.0f, bool bRestoreState = false);
    UUMGSequencePlayer* StopAnimation(const FName& animationName, EStopAnimType type = EStopAnimType::ToDefault, bool bRecvEvent = false, bool bStopAniExecOnlyPlaying = false);
    bool IsPlayingAnimationName(const FName& animationName);

    UUMGSequencePlayer* SetAnimationTime(UWidgetAnimation* InAnimation, float AtTime, EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward);

    UWidgetAnimation* FindAnimation(const FName& animationName);
    FName GetAnimationName(UWidgetAnimation* animation);

    void ClearStopSequencePlayersIfMany();
protected:
    void ProcShowAnimation();
    bool ProcHideAnimation();    
    void PlayAutoLoopAnimation();
    void StopAutoLoopAnimation();    

protected:
    virtual void OnAnimationStarted_Implementation(const UWidgetAnimation* pAnimation)override;
    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* pAnimation)override;
    virtual void UIOnAnimationStarted(const UWidgetAnimation* pAnimation);
    virtual void UIOnAnimationFinished(const UWidgetAnimation* pAnimation);
    
protected:
    
};