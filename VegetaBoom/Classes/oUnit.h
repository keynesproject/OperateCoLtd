#ifndef __O_UNIT_H__
#define __O_UNIT_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

struct MusicData;
struct ImageData;

/////////////////////////////////////////////////////////////////////
//
//oAudio
//
/////////////////////////////////////////////////////////////////////
class oAudio
{
public:
    oAudio();
    ~oAudio();

    bool Create(string strFile, bool isMusic, bool isLoop);

    void Play();
    void Stop();
    void Pause( bool isPause );
    
private:
    bool   m_IsCreated;      //�O�_�إ�;//
    string m_strFilePath;    //�����ɮ׸��|;//
    bool   m_isLoop;         //�O�_�`������;//
    bool   m_isMusic;        //true�N������,false�N������;//
    unsigned int m_nSoundId; //playEffect�ɦ^�Ǫ�ID;//
};

/////////////////////////////////////////////////////////////////////
//
//oSprite : ��X�i���ּ���\��
//
/////////////////////////////////////////////////////////////////////
class oSprite : public Sprite
{
public:
    oSprite();
    ~oSprite();
    
    static oSprite* create();
    static oSprite* create(const std::string& filename);

    void SetAnimation(ImageData *pData);
    void SetAnimation(string strAniFile, int nAniNum, float fDelayPerUnit=0);
    
    //-1:����^��, 0:�����, N:����N��;//
    void AniPlay( int nLoop );                
    void AniPause( bool isResume );

    void SetMusic(MusicData *pData);
    void SetMusic(string strFile, bool isMusic, bool isLoop);
    void MusicPlay();
    void MusicStop();
    void MusicPause( bool isResume);
    
private:
    oAudio    m_Audio;       //���ּ��񪫥�;//
    Animation *m_Animation;  //�v�漽�񪫥�;//
    Action    *m_pAniAction; //�v�漷��ʧ@;//
};

#endif 
