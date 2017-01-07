#ifndef __O_ITEM_CHARACTER_H__
#define __O_ITEM_CHARACTER_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class oSprite;
class oCharacter;

struct CharacterData;

enum CharacterState
{
    eSTATE_STOP = 0,
    eSTATE_SHOW_UP,   //腳色冒出;//
    eSTATE_MOVE_OUT,  //往視窗外移動;//
    eSTATE_BLOW_UP,   //開始震動變大;//
    eSTATE_BOMB,      //爆炸效果,並顯示皺掉的腳色;//
    eSTATE_FAILURE,   //動作失敗;//
    eSTATE_RUNNING,   //動作表演中;//
    eSTATE_ERROR_STATE,
};

/////////////////////////////////////////////////////////////////////
//
//ActorActionState 角色狀態機
//
/////////////////////////////////////////////////////////////////////
class ActorActionState
{
public:
    ActorActionState();
    ~ActorActionState();

    void SetActionState(bool isActive);
    
    virtual CharacterState SetHitState(bool isDown);

    virtual CharacterState Action(oCharacter *pActor) = 0;

protected:
    bool m_bActionActive;
};

//停止一切動作並設定回到初始狀態;//
class ActorStop : public ActorActionState
{
public:
    ActorStop() {};
    ~ActorStop() {};

    CharacterState Action(oCharacter *pActor);
};

//角色進場動畫;//
class ActorShowUp : public ActorActionState
{
public:
    ActorShowUp() {};
    ~ActorShowUp() {};

    CharacterState Action(oCharacter *pActor);
};

//角色往視窗外移動動作;//
class ActorMoveOut : public ActorActionState
{
public:
    ActorMoveOut();
    ~ActorMoveOut() {};

    CharacterState SetHitState(bool isDown);

    CharacterState Action(oCharacter *pActor);

private:
    bool m_bHit;
};

//角色震動變大;//
class ActorBlowUp : public ActorActionState
{
public:
    ActorBlowUp();
    ~ActorBlowUp() {};

    CharacterState SetHitState(bool isDown);

    CharacterState Action(oCharacter *pActor);

private:
    bool m_isFailure;
};

//角色爆炸;//
class ActorBomb : public ActorActionState
{
public:
    ActorBomb() {};
    ~ActorBomb() {};

    CharacterState Action(oCharacter *pActor);
};

//操作錯誤動畫;//
class ActorFailure : public ActorActionState
{
public:
    ActorFailure() {};
    ~ActorFailure() {};

    CharacterState Action(oCharacter *pActor);
};

/////////////////////////////////////////////////////////////////////
//
//oCharacter 角色基礎物件
//
/////////////////////////////////////////////////////////////////////
class oCharacter : public Sprite
{
public:

    static oCharacter* create();
    static oCharacter* create(const std::string& filename);

    oCharacter();
    
    ~oCharacter();

    //設定角色資料;//
    virtual void Setup(CharacterData *Data);    
    
    //開始執行腳色流程;//
    void StartAction(bool bFirst = false);

    //執行指定狀態;//
    void SetState(CharacterState State);

    CharacterState Action();

    //停止一切動作並設定回到初始狀態;//
    virtual void SetActionInit();

    //角色進場動畫;//
    virtual void SetActionShowUp();

    //角色往視窗外移動動作;//
    virtual void SetActionMoveOut();

    //角色震動變大;//
    virtual void SetActionBlowUp();

    //角色爆炸及皺縮動畫;//
    virtual void SetActionBomb();

    //操作錯誤動畫;//
    virtual void SetActionFailure();
    
    //停止執行目前Action;//
    void SetActionStop();

    //檢查是否有動作執行中;//
    bool IsActorAction();
    
    //Action執行完畢會呼叫的Function;//
    void ActionIsDoneCallFunc();

    //設定使用者按壓狀態;//
    CharacterState SetHitState(bool isDown);

    //取得角色位置;//
    const Vec2& getActorPosition();
protected:

    //紀錄是否為第一次執行;//
    bool m_bFirstStrt;

    //角色狀態機;//
    ActorActionState *m_pActorState;
    map<int, ActorActionState*> m_ActorState;
    bool m_bActiveNextState;
    ActorActionState *m_pNextActorState;
    
    enum ActorSprite
    {
        eASPRITE_RUSH=0,
        eASPRITE_TOUCH,
        eASPRITE_FALL,
        eASPRITE_BOMB,
        eASPRITE_MAX_NUM,
    };

    //角色各狀態時的動畫;//
    vector<oSprite*> m_Actor;
    CallFunc *m_pActionDoneCallFunc;

    //角色正在執行的Action;//
    vector<cocos2d::Action*> m_pActiveAction;

    //角色爆開至定位後往螢幕外移動的時間;//
    double m_dMoveOutSec;

    //角色放大至爆炸的時間;//
    double m_dBlowUpSec;

    //角色要按的次數;//
    int m_nSatellite;

    double m_dScaleStart;
    double m_dScaleEnd;

};

/////////////////////////////////////////////////////////////////////
//
//oCharacterSingle 角色物件，單一腳色
//
/////////////////////////////////////////////////////////////////////
class oCharacterSingle : public oCharacter
{
public:
    oCharacterSingle();

    ~oCharacterSingle();

    //設定角色資料;//
    virtual void Setup(CharacterData *Data);
    
    void SetActionMoveOut();
    
    //void SetActionInit();
    //void SetActionShowUp(float StartX, float StartY);
    //void SetActionBlowUp();
    //void SetActionBomb();
    //void SetActionFailure();
        
};

#endif
