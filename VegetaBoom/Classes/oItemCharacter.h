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
    eSTATE_SHOW_UP,   //�}��_�X;//
    eSTATE_MOVE_OUT,  //�������~����;//
    eSTATE_BLOW_UP,   //�}�l�_���ܤj;//
    eSTATE_BOMB,      //�z���ĪG,����ܽK�����}��;//
    eSTATE_FAILURE,   //�ʧ@����;//
    eSTATE_RUNNING,   //�ʧ@��t��;//
    eSTATE_ERROR_STATE,
};

/////////////////////////////////////////////////////////////////////
//
//ActorActionState ���⪬�A��
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

//����@���ʧ@�ó]�w�^���l���A;//
class ActorStop : public ActorActionState
{
public:
    ActorStop() {};
    ~ActorStop() {};

    CharacterState Action(oCharacter *pActor);
};

//����i���ʵe;//
class ActorShowUp : public ActorActionState
{
public:
    ActorShowUp() {};
    ~ActorShowUp() {};

    CharacterState Action(oCharacter *pActor);
};

//���⩹�����~���ʰʧ@;//
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

//����_���ܤj;//
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

//�����z��;//
class ActorBomb : public ActorActionState
{
public:
    ActorBomb() {};
    ~ActorBomb() {};

    CharacterState Action(oCharacter *pActor);
};

//�ާ@���~�ʵe;//
class ActorFailure : public ActorActionState
{
public:
    ActorFailure() {};
    ~ActorFailure() {};

    CharacterState Action(oCharacter *pActor);
};

/////////////////////////////////////////////////////////////////////
//
//oCharacter �����¦����
//
/////////////////////////////////////////////////////////////////////
class oCharacter : public Sprite
{
public:

    static oCharacter* create();
    static oCharacter* create(const std::string& filename);

    oCharacter();
    
    ~oCharacter();

    //�]�w������;//
    virtual void Setup(CharacterData *Data);    
    
    //�}�l����}��y�{;//
    void StartAction(bool bFirst = false);

    //������w���A;//
    void SetState(CharacterState State);

    CharacterState Action();

    //����@���ʧ@�ó]�w�^���l���A;//
    virtual void SetActionInit();

    //����i���ʵe;//
    virtual void SetActionShowUp();

    //���⩹�����~���ʰʧ@;//
    virtual void SetActionMoveOut();

    //����_���ܤj;//
    virtual void SetActionBlowUp();

    //�����z���νK�Y�ʵe;//
    virtual void SetActionBomb();

    //�ާ@���~�ʵe;//
    virtual void SetActionFailure();
    
    //�������ثeAction;//
    void SetActionStop();

    //�ˬd�O�_���ʧ@���椤;//
    bool IsActorAction();
    
    //Action���槹���|�I�s��Function;//
    void ActionIsDoneCallFunc();

    //�]�w�ϥΪ̫������A;//
    CharacterState SetHitState(bool isDown);

    //���o�����m;//
    const Vec2& getActorPosition();
protected:

    //�����O�_���Ĥ@������;//
    bool m_bFirstStrt;

    //���⪬�A��;//
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

    //����U���A�ɪ��ʵe;//
    vector<oSprite*> m_Actor;
    CallFunc *m_pActionDoneCallFunc;

    //���⥿�b���檺Action;//
    vector<cocos2d::Action*> m_pActiveAction;

    //�����z�}�ܩw��᩹�ù��~���ʪ��ɶ�;//
    double m_dMoveOutSec;

    //�����j���z�����ɶ�;//
    double m_dBlowUpSec;

    //����n��������;//
    int m_nSatellite;

    double m_dScaleStart;
    double m_dScaleEnd;

};

/////////////////////////////////////////////////////////////////////
//
//oCharacterSingle ���⪫��A��@�}��
//
/////////////////////////////////////////////////////////////////////
class oCharacterSingle : public oCharacter
{
public:
    oCharacterSingle();

    ~oCharacterSingle();

    //�]�w������;//
    virtual void Setup(CharacterData *Data);
    
    void SetActionMoveOut();
    
    //void SetActionInit();
    //void SetActionShowUp(float StartX, float StartY);
    //void SetActionBlowUp();
    //void SetActionBomb();
    //void SetActionFailure();
        
};

#endif
