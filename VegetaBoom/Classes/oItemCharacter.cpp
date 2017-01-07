#include "oItemCharacter.h"
#include "oUnit.h"
#include "oConfig.h"

/////////////////////////////////////////////////////////////////////
//
//ActorActionState ���⪬�A��
//
/////////////////////////////////////////////////////////////////////
ActorActionState::ActorActionState()
{
    m_bActionActive = true;
}

ActorActionState::~ActorActionState()
{

}

void ActorActionState::SetActionState(bool isActive)
{
    m_bActionActive = isActive;
}

CharacterState ActorActionState::SetHitState(bool isDown)
{
    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorStop ���ⰱ��A
//
/////////////////////////////////////////////////////////////////////
CharacterState ActorStop::Action(oCharacter *pActor)
{
    return eSTATE_ERROR_STATE;
}

/////////////////////////////////////////////////////////////////////
//
//ActorShowUp ����X�{���A
//
/////////////////////////////////////////////////////////////////////
CharacterState ActorShowUp::Action(oCharacter *pActor)
{
    if (!m_bActionActive)
    {
        m_bActionActive = true;
        
        //�]�wMove Out���ʧ@;//
        pActor->SetActionMoveOut();

        //�������A;//
        pActor->SetState(eSTATE_MOVE_OUT);

        return eSTATE_MOVE_OUT;
    }

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorMoveOut ���Ⲿ�ʦܵe���~���A
//
/////////////////////////////////////////////////////////////////////
ActorMoveOut::ActorMoveOut()
{
    m_bHit = false;
}

CharacterState ActorMoveOut::Action(oCharacter *pActor)
{   
    //��ܦ������ƥ�,�h�������ʪ��A�����ܤU�Ӫ��A;//
    if (m_bHit)
    {
        m_bHit = false;
        m_bActionActive = true;

        //�������ʧ@;//
        pActor->SetActionStop();

        //�]�wBlow Up���ʧ@;//
        pActor->SetActionBlowUp();

        //�������A;//
        pActor->SetState(eSTATE_BLOW_UP);

        return eSTATE_BLOW_UP;
    }

    //��ܨ��Ⲿ�ʦܵ����~,�����ܰ���A,�æ^�ǥ��Ѫ��A;//
    if (!m_bActionActive)
    {
        m_bActionActive = true;

        //�������ʧ@;//
        pActor->SetActionStop();

        //�����ܰ���A;//
        pActor->SetState(eSTATE_STOP);

        //�^�ǥ��Ѫ��A;//
        return eSTATE_FAILURE;
    }

    return eSTATE_RUNNING;
}

CharacterState ActorMoveOut::SetHitState(bool isDown)
{
    //�����O�_�������ƥ�;//
    if (isDown)
        m_bHit = true;

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorBlowUp ����_�ʩ�j���A
//
/////////////////////////////////////////////////////////////////////
ActorBlowUp::ActorBlowUp()
{
    m_isFailure = false;
}

CharacterState ActorBlowUp::Action(oCharacter *pActor)
{
    //��ܦ���}��������ʧ@,�����ܥ��Ѫ��A;//
    if (m_isFailure)
    {
        m_isFailure = false;

        //�������ʧ@;//
        pActor->SetActionStop();

        //�]�w���Ѱʧ@;//
        pActor->SetActionFailure();

        //�����ܥ��Ѫ��A;//
        pActor->SetState(eSTATE_FAILURE);

        return eSTATE_RUNNING;
    }

    //��ܰʧ@���槹��,�������z�����A;//
    if (!m_bActionActive)
    {
        m_bActionActive = true;

        //�������ʧ@;//
        pActor->SetActionStop();

        //�]�w�z���ʧ@;//
        pActor->SetActionBomb();

        //�������z�����A;//
        pActor->SetState(eSTATE_BOMB);

        return eSTATE_BOMB;
    }

    return eSTATE_RUNNING;
}

CharacterState ActorBlowUp::SetHitState(bool isDown)
{
    //��ܦ���}���ʧ@,�����ܥ��Ѫ��A;//
    if (!isDown)
    {
        m_isFailure = true;
    }

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorBomb �����z�����A
//
/////////////////////////////////////////////////////////////////////
CharacterState ActorBomb::Action(oCharacter *pActor)
{
    //��ܰʧ@���槹��,�����ܰ���A;//
    if (!m_bActionActive)
    {
        m_bActionActive = true;

        //�]�w����ʧ@;//
        pActor->SetActionStop();

        //��l�ƨ���ʧ@;//
        pActor->SetActionInit();

        //�ʴ����A�ܰ���;//
        pActor->SetState(eSTATE_STOP);

        return eSTATE_STOP;
    }

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorFailure ���⥢�Ѫ��A
//
/////////////////////////////////////////////////////////////////////
CharacterState ActorFailure::Action(oCharacter *pActor)
{
    if (!m_bActionActive)
    {
        m_bActionActive = true;

        //�]�w����ʧ@;//
        pActor->SetActionStop();

        //�����ܰ���A;//
        pActor->SetState(eSTATE_STOP);

        return eSTATE_FAILURE;
    }

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//oCharacter ����򥻤���
//
/////////////////////////////////////////////////////////////////////
oCharacter* oCharacter::create()
{
    oCharacter *sprite = new (std::nothrow) oCharacter();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

oCharacter* oCharacter::create(const std::string& filename)
{
    oCharacter *sprite = new (std::nothrow) oCharacter();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

oCharacter::oCharacter()
{   
    m_bActiveNextState = false;

    m_pActorState = NULL;
    m_ActorState.insert(pair<int, ActorActionState*>(eSTATE_STOP, new ActorStop()));
    m_ActorState.insert(pair<int, ActorActionState*>(eSTATE_SHOW_UP, new ActorShowUp()));
    m_ActorState.insert(pair<int, ActorActionState*>(eSTATE_MOVE_OUT, new ActorMoveOut()));
    m_ActorState.insert(pair<int, ActorActionState*>(eSTATE_BLOW_UP, new ActorBlowUp()));
    m_ActorState.insert(pair<int, ActorActionState*>(eSTATE_BOMB, new ActorBomb()));
    m_ActorState.insert(pair<int, ActorActionState*>(eSTATE_FAILURE, new ActorFailure()));

    m_pActionDoneCallFunc = CallFunc::create(this, callfunc_selector(oCharacter::ActionIsDoneCallFunc));
    m_pActionDoneCallFunc->retain();
}

oCharacter::~oCharacter()
{
    map<int, ActorActionState*>::iterator it = m_ActorState.begin();
    for (; it != m_ActorState.end(); it++)
    {
        delete it->second;
        it->second = NULL;
    }
    m_ActorState.clear();

    m_pActionDoneCallFunc->release();
}

void oCharacter::Setup(CharacterData *pData)
{
    for (size_t i = 0; i < eASPRITE_MAX_NUM; i++)
    {
        m_Actor.push_back(oSprite::create());
        this->addChild(m_Actor[i], i+1);

        m_pActiveAction.push_back(NULL);
    }
    
    //�]�w�ϧΤέ��ָ�T;//
    m_Actor[eASPRITE_RUSH]->SetAnimation(&pData->Rush);    
    m_Actor[eASPRITE_TOUCH]->SetAnimation(&pData->Touch);
    m_Actor[eASPRITE_FALL]->SetAnimation(&pData->Fall);
    m_Actor[eASPRITE_BOMB]->SetAnimation(&pData->Bomb);

    //�]�w���ʳt��;//
    m_dMoveOutSec = pData->dMoveOutSec;

    //�]�w�Ȥj�z���ɶ�;//
    m_dBlowUpSec = pData->dBlowUpSec;

    //���⪺�l����ƶq(�n��������);//
    m_nSatellite = pData->nSatellite;
    
    m_dScaleStart = pData->dScaleStart;
    m_dScaleEnd = pData->dScaleEnd;    

    this->SetActionInit();

    this->SetState(eSTATE_STOP);
}

void oCharacter::StartAction(bool bFirst)
{
    m_bFirstStrt = bFirst;
    if (m_bFirstStrt)
        this->setPositionY(0.0f);

    this->setVisible(true);
    this->SetActionInit();
    this->SetActionShowUp();
    this->SetState(eSTATE_SHOW_UP);
}

void oCharacter::SetState(CharacterState State)
{
    map<int, ActorActionState*>::iterator it = m_ActorState.find(State);
    if (it == m_ActorState.end())
        return;

    m_pNextActorState = it->second;
    m_bActiveNextState = true;
}

CharacterState oCharacter::Action()
{
    if (m_bActiveNextState)
    {
        m_pActorState = m_pNextActorState;
        m_bActiveNextState = false;
    }

    return m_pActorState->Action(this);
}

void oCharacter::SetActionInit()
{
    //����Ҧ��ʧ@�����äγ]�w�^��l��m;//
    for (size_t i = 0; i < m_Actor.size(); i++)
    {
        //m_Actor[i]->pause();   
        if (m_pActiveAction[i])
        {
            m_Actor[i]->stopAction(m_pActiveAction[i]);
            m_pActiveAction[i] = NULL;
        }
        //m_Actor[i]->stopAllActions();  //�o��I�s���ܷ|�s�ʵe�ɮ׳��Q�M����;//
        m_Actor[i]->setVisible(false);
        m_Actor[i]->setPosition(Vec2(0, 0));
        m_Actor[i]->setCascadeOpacityEnabled(true);
        m_Actor[i]->setOpacity(255);
        m_Actor[i]->setScale(m_dScaleStart);
        m_Actor[i]->setRotation(0.0f);
    }
}

void oCharacter::SetActionShowUp()
{
    //�]�w��ܤμ���ʵe�٦���l�X�{��m;//
    m_Actor[eASPRITE_RUSH]->setVisible(true);
    m_Actor[eASPRITE_RUSH]->AniPlay(-1);
    m_Actor[eASPRITE_RUSH]->setPosition(0, 0);  

    //�]�w�ʧ@:�ֳt���C���ʦܫ��w��m
    Vec2 PosBy(0, 400);
    auto moveBy = MoveBy::create(1.0f, PosBy);
    auto easeOut = EaseOut::create(moveBy, 5.0f);
    auto MoveEaseOut = Sequence::create(easeOut, m_pActionDoneCallFunc, NULL);

    //����ðO���ʧ@;//
    m_pActiveAction[eASPRITE_RUSH] = m_Actor[eASPRITE_RUSH]->runAction(MoveEaseOut);
}

void oCharacter::SetActionMoveOut()
{
    //�~�򼷩�ʵe;//
    m_Actor[eASPRITE_RUSH]->AniPlay(-1);

    //�w�t���W����;//
    auto VisibleSize = Director::getInstance()->getVisibleSize();

    //�����o������m;//
    Vec2 Pos = m_Actor[eASPRITE_RUSH]->getPosition();

    //���W���ʦܿù��~;//
    auto moveTo = MoveTo::create(m_dMoveOutSec, Vec2(Pos.x, VisibleSize.height + m_Actor[eASPRITE_RUSH]->getContentSize().height));

    //����ðO���ʧ@;//
    m_pActiveAction[eASPRITE_RUSH] = m_Actor[eASPRITE_RUSH]->runAction(Sequence::create(moveTo, m_pActionDoneCallFunc, NULL));
}

void oCharacter::SetActionBlowUp()
{
    //�]�w��ܤμ���ʵe�٦���l�X�{��m;//
    m_Actor[eASPRITE_RUSH]->setVisible(false);
    m_Actor[eASPRITE_RUSH]->AniPlay(0);
    m_Actor[eASPRITE_TOUCH]->setVisible(true);
    m_Actor[eASPRITE_TOUCH]->AniPlay(-1);
    m_Actor[eASPRITE_TOUCH]->setPosition(m_Actor[eASPRITE_RUSH]->getPosition());

    //�إ߾_�ʩ�j�ʧ@;//
    float ScaleSize = m_dScaleEnd - m_dScaleStart;
    auto moveBy = MoveBy::create(0.1f, Vec2(0.0f, 3.0f));
    auto rotateRight = RotateTo::create(0.1f, 5.0f);
    auto rotateLeft = RotateTo::create(0.1f, -5.0f);
    auto scaleBy = ScaleBy::create(0.1f, 1.1f);
    auto rotateRightScale = Spawn::createWithTwoActions(rotateRight, scaleBy);
    auto rotateLeftScale = Spawn::createWithTwoActions(rotateLeft, scaleBy);
    auto Shake = Sequence::create(rotateRightScale, rotateLeftScale, NULL);
    auto MoveAndShake = Spawn::createWithTwoActions(moveBy, Shake);

    //�إ߲��ʰʧ@�A���ʦܭ��I;//
    auto moveTo = m_bFirstStrt ? MoveTo::create(0.1f, Vec2(0, Director::getInstance()->getVisibleSize().height/4)) : MoveTo::create(0.1f, Vec2(0, 0));

    //�إߧǦC�ʧ@:1.���ʦܫ��w��m 2.�_�ʩ�j;//
    unsigned int ShakeTime = m_dBlowUpSec / 0.3f;
    auto Seq = Sequence::create(moveTo, Repeat::create(MoveAndShake, ShakeTime), m_pActionDoneCallFunc, NULL);

    //����ðO���ʧ@;//
    m_pActiveAction[eASPRITE_TOUCH] = m_Actor[eASPRITE_TOUCH]->runAction(Seq);
}

void oCharacter::SetActionBomb()
{
    //����z���νK�Y�ʵe;//

    //���ë��U�ʵe;//
    m_Actor[eASPRITE_TOUCH]->setVisible(false);
    m_Actor[eASPRITE_TOUCH]->AniPlay(0);

    //�����z���S��;//
    m_Actor[eASPRITE_BOMB]->setVisible(true);
    m_Actor[eASPRITE_BOMB]->setScale(m_Actor[eASPRITE_TOUCH]->getScale());
    m_Actor[eASPRITE_BOMB]->setPosition(m_Actor[eASPRITE_TOUCH]->getPosition());
    m_Actor[eASPRITE_BOMB]->AniPlay(1);

    //��ܽK�Y�U���βH�X�ʧ@;//
    m_Actor[eASPRITE_FALL]->setVisible(true);
    m_Actor[eASPRITE_FALL]->setScale(m_Actor[eASPRITE_TOUCH]->getScale());
    m_Actor[eASPRITE_FALL]->setPosition(m_Actor[eASPRITE_TOUCH]->getPosition());
    m_Actor[eASPRITE_FALL]->AniPlay(-1);
    
    auto moveBy = MoveBy::create(1.0f, Vec2(0,-200));
    auto fadeOut = FadeOut::create(1.0f);
    auto mySpawn = Spawn::createWithTwoActions(moveBy, fadeOut);
    auto Seq = Sequence::create(mySpawn, m_pActionDoneCallFunc, NULL);
    m_pActiveAction[eASPRITE_FALL] = m_Actor[eASPRITE_FALL]->runAction(Seq);
}

void oCharacter::SetActionFailure()
{
    //���ë��U�ʵe;//
    m_Actor[eASPRITE_TOUCH]->setVisible(false);
    m_Actor[eASPRITE_TOUCH]->AniPlay(0);

    //��ܽK�Y�U���βH�X�ʧ@;//
    m_Actor[eASPRITE_FALL]->setVisible(true);
    m_Actor[eASPRITE_FALL]->setScale(m_Actor[eASPRITE_TOUCH]->getScale());
    m_Actor[eASPRITE_FALL]->setPosition(m_Actor[eASPRITE_TOUCH]->getPosition());
    m_Actor[eASPRITE_FALL]->AniPlay(-1);

    auto moveBy = MoveBy::create(1.0f, Vec2(0, -200));
    auto fadeOut = FadeOut::create(1.0f);
    auto mySpawn = Spawn::createWithTwoActions(moveBy, fadeOut);
    auto Seq = Sequence::create(mySpawn, m_pActionDoneCallFunc, NULL);
    m_pActiveAction[eASPRITE_FALL] = m_Actor[eASPRITE_FALL]->runAction(Seq);
}

void oCharacter::SetActionStop()
{
    for (size_t i = 0; i < eASPRITE_MAX_NUM; i++)
    {
        if (m_pActiveAction[i])
        {
            m_Actor[i]->stopAction(m_pActiveAction[i]);
            m_pActiveAction[i] = NULL;
        }

    }
}

bool oCharacter::IsActorAction()
{
    for (size_t i = 0; i < m_Actor.size(); i++)
    {
        if (m_Actor[i]->getNumberOfRunningActions() != 0)
            return true;
    }

    return false;
}

void oCharacter::ActionIsDoneCallFunc()
{    
    //�]�w�ʧ@���񵲧�;//    
    m_pActorState->SetActionState(false);
}

CharacterState oCharacter::SetHitState(bool isDown)
{
    return m_pActorState->SetHitState(isDown);
}

const Vec2& oCharacter::getActorPosition()
{
    return m_Actor[eASPRITE_TOUCH]->getPosition() + this->getPosition();
}

/////////////////////////////////////////////////////////////////////
//
//oCharacterSingle ���⪫��A��@����
//
/////////////////////////////////////////////////////////////////////
oCharacterSingle::oCharacterSingle()
{

}

oCharacterSingle::~oCharacterSingle()
{

}

void oCharacterSingle::Setup(CharacterData *Data)
{
    oCharacter::Setup(Data);
}

void oCharacterSingle::SetActionMoveOut()
{

}


