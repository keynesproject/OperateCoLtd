#include "oItemCharacter.h"
#include "oUnit.h"
#include "oConfig.h"

/////////////////////////////////////////////////////////////////////
//
//ActorActionState 角色狀態機
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
//ActorStop 角色停止狀態
//
/////////////////////////////////////////////////////////////////////
CharacterState ActorStop::Action(oCharacter *pActor)
{
    return eSTATE_ERROR_STATE;
}

/////////////////////////////////////////////////////////////////////
//
//ActorShowUp 角色出現狀態
//
/////////////////////////////////////////////////////////////////////
CharacterState ActorShowUp::Action(oCharacter *pActor)
{
    if (!m_bActionActive)
    {
        m_bActionActive = true;
        
        //設定Move Out的動作;//
        pActor->SetActionMoveOut();

        //切換狀態;//
        pActor->SetState(eSTATE_MOVE_OUT);

        return eSTATE_MOVE_OUT;
    }

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorMoveOut 角色移動至畫面外狀態
//
/////////////////////////////////////////////////////////////////////
ActorMoveOut::ActorMoveOut()
{
    m_bHit = false;
}

CharacterState ActorMoveOut::Action(oCharacter *pActor)
{   
    //表示有按壓事件,則結束移動狀態切換至下個狀態;//
    if (m_bHit)
    {
        m_bHit = false;
        m_bActionActive = true;

        //停止執行動作;//
        pActor->SetActionStop();

        //設定Blow Up的動作;//
        pActor->SetActionBlowUp();

        //切換狀態;//
        pActor->SetState(eSTATE_BLOW_UP);

        return eSTATE_BLOW_UP;
    }

    //表示角色移動至視窗外,切換至停止狀態,並回傳失敗狀態;//
    if (!m_bActionActive)
    {
        m_bActionActive = true;

        //停止執行動作;//
        pActor->SetActionStop();

        //切換至停止狀態;//
        pActor->SetState(eSTATE_STOP);

        //回傳失敗狀態;//
        return eSTATE_FAILURE;
    }

    return eSTATE_RUNNING;
}

CharacterState ActorMoveOut::SetHitState(bool isDown)
{
    //紀錄是否有按壓事件;//
    if (isDown)
        m_bHit = true;

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorBlowUp 角色震動放大狀態
//
/////////////////////////////////////////////////////////////////////
ActorBlowUp::ActorBlowUp()
{
    m_isFailure = false;
}

CharacterState ActorBlowUp::Action(oCharacter *pActor)
{
    //表示有放開手指按壓動作,切換至失敗狀態;//
    if (m_isFailure)
    {
        m_isFailure = false;

        //停止執行動作;//
        pActor->SetActionStop();

        //設定失敗動作;//
        pActor->SetActionFailure();

        //切換至失敗狀態;//
        pActor->SetState(eSTATE_FAILURE);

        return eSTATE_RUNNING;
    }

    //表示動作執行完畢,切換至爆炸狀態;//
    if (!m_bActionActive)
    {
        m_bActionActive = true;

        //停止執行動作;//
        pActor->SetActionStop();

        //設定爆炸動作;//
        pActor->SetActionBomb();

        //切換至爆炸狀態;//
        pActor->SetState(eSTATE_BOMB);

        return eSTATE_BOMB;
    }

    return eSTATE_RUNNING;
}

CharacterState ActorBlowUp::SetHitState(bool isDown)
{
    //表示有放開的動作,切換至失敗狀態;//
    if (!isDown)
    {
        m_isFailure = true;
    }

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorBomb 角色爆炸狀態
//
/////////////////////////////////////////////////////////////////////
CharacterState ActorBomb::Action(oCharacter *pActor)
{
    //表示動作執行完畢,切換至停止狀態;//
    if (!m_bActionActive)
    {
        m_bActionActive = true;

        //設定停止動作;//
        pActor->SetActionStop();

        //初始化角色動作;//
        pActor->SetActionInit();

        //缺換狀態至停止;//
        pActor->SetState(eSTATE_STOP);

        return eSTATE_STOP;
    }

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//ActorFailure 角色失敗狀態
//
/////////////////////////////////////////////////////////////////////
CharacterState ActorFailure::Action(oCharacter *pActor)
{
    if (!m_bActionActive)
    {
        m_bActionActive = true;

        //設定停止動作;//
        pActor->SetActionStop();

        //切換至停止狀態;//
        pActor->SetState(eSTATE_STOP);

        return eSTATE_FAILURE;
    }

    return eSTATE_RUNNING;
}

/////////////////////////////////////////////////////////////////////
//
//oCharacter 角色基本介面
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
    
    //設定圖形及音樂資訊;//
    m_Actor[eASPRITE_RUSH]->SetAnimation(&pData->Rush);    
    m_Actor[eASPRITE_TOUCH]->SetAnimation(&pData->Touch);
    m_Actor[eASPRITE_FALL]->SetAnimation(&pData->Fall);
    m_Actor[eASPRITE_BOMB]->SetAnimation(&pData->Bomb);

    //設定移動速度;//
    m_dMoveOutSec = pData->dMoveOutSec;

    //設定脹大爆炸時間;//
    m_dBlowUpSec = pData->dBlowUpSec;

    //角色的子角色數量(要按的次數);//
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
    //停止所有動作及隱藏及設定回初始位置;//
    for (size_t i = 0; i < m_Actor.size(); i++)
    {
        //m_Actor[i]->pause();   
        if (m_pActiveAction[i])
        {
            m_Actor[i]->stopAction(m_pActiveAction[i]);
            m_pActiveAction[i] = NULL;
        }
        //m_Actor[i]->stopAllActions();  //這邊呼叫的話會連動畫檔案都被清除掉;//
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
    //設定顯示及撥放動畫還有初始出現位置;//
    m_Actor[eASPRITE_RUSH]->setVisible(true);
    m_Actor[eASPRITE_RUSH]->AniPlay(-1);
    m_Actor[eASPRITE_RUSH]->setPosition(0, 0);  

    //設定動作:快速漸慢移動至指定位置
    Vec2 PosBy(0, 400);
    auto moveBy = MoveBy::create(1.0f, PosBy);
    auto easeOut = EaseOut::create(moveBy, 5.0f);
    auto MoveEaseOut = Sequence::create(easeOut, m_pActionDoneCallFunc, NULL);

    //執行並記錄動作;//
    m_pActiveAction[eASPRITE_RUSH] = m_Actor[eASPRITE_RUSH]->runAction(MoveEaseOut);
}

void oCharacter::SetActionMoveOut()
{
    //繼續撥放動畫;//
    m_Actor[eASPRITE_RUSH]->AniPlay(-1);

    //定速往上移動;//
    auto VisibleSize = Director::getInstance()->getVisibleSize();

    //先取得本身位置;//
    Vec2 Pos = m_Actor[eASPRITE_RUSH]->getPosition();

    //往上移動至螢幕外;//
    auto moveTo = MoveTo::create(m_dMoveOutSec, Vec2(Pos.x, VisibleSize.height + m_Actor[eASPRITE_RUSH]->getContentSize().height));

    //執行並記錄動作;//
    m_pActiveAction[eASPRITE_RUSH] = m_Actor[eASPRITE_RUSH]->runAction(Sequence::create(moveTo, m_pActionDoneCallFunc, NULL));
}

void oCharacter::SetActionBlowUp()
{
    //設定顯示及撥放動畫還有初始出現位置;//
    m_Actor[eASPRITE_RUSH]->setVisible(false);
    m_Actor[eASPRITE_RUSH]->AniPlay(0);
    m_Actor[eASPRITE_TOUCH]->setVisible(true);
    m_Actor[eASPRITE_TOUCH]->AniPlay(-1);
    m_Actor[eASPRITE_TOUCH]->setPosition(m_Actor[eASPRITE_RUSH]->getPosition());

    //建立震動放大動作;//
    float ScaleSize = m_dScaleEnd - m_dScaleStart;
    auto moveBy = MoveBy::create(0.1f, Vec2(0.0f, 3.0f));
    auto rotateRight = RotateTo::create(0.1f, 5.0f);
    auto rotateLeft = RotateTo::create(0.1f, -5.0f);
    auto scaleBy = ScaleBy::create(0.1f, 1.1f);
    auto rotateRightScale = Spawn::createWithTwoActions(rotateRight, scaleBy);
    auto rotateLeftScale = Spawn::createWithTwoActions(rotateLeft, scaleBy);
    auto Shake = Sequence::create(rotateRightScale, rotateLeftScale, NULL);
    auto MoveAndShake = Spawn::createWithTwoActions(moveBy, Shake);

    //建立移動動作，移動至原點;//
    auto moveTo = m_bFirstStrt ? MoveTo::create(0.1f, Vec2(0, Director::getInstance()->getVisibleSize().height/4)) : MoveTo::create(0.1f, Vec2(0, 0));

    //建立序列動作:1.移動至指定位置 2.震動放大;//
    unsigned int ShakeTime = m_dBlowUpSec / 0.3f;
    auto Seq = Sequence::create(moveTo, Repeat::create(MoveAndShake, ShakeTime), m_pActionDoneCallFunc, NULL);

    //執行並記錄動作;//
    m_pActiveAction[eASPRITE_TOUCH] = m_Actor[eASPRITE_TOUCH]->runAction(Seq);
}

void oCharacter::SetActionBomb()
{
    //顯示爆炸及皺縮動畫;//

    //隱藏按下動畫;//
    m_Actor[eASPRITE_TOUCH]->setVisible(false);
    m_Actor[eASPRITE_TOUCH]->AniPlay(0);

    //播放爆炸特效;//
    m_Actor[eASPRITE_BOMB]->setVisible(true);
    m_Actor[eASPRITE_BOMB]->setScale(m_Actor[eASPRITE_TOUCH]->getScale());
    m_Actor[eASPRITE_BOMB]->setPosition(m_Actor[eASPRITE_TOUCH]->getPosition());
    m_Actor[eASPRITE_BOMB]->AniPlay(1);

    //顯示皺縮下移及淡出動作;//
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
    //隱藏按下動畫;//
    m_Actor[eASPRITE_TOUCH]->setVisible(false);
    m_Actor[eASPRITE_TOUCH]->AniPlay(0);

    //顯示皺縮下移及淡出動作;//
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
    //設定動作撥放結束;//    
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
//oCharacterSingle 角色物件，單一角色
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


