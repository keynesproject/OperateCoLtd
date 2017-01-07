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
    bool   m_IsCreated;      //是否建立;//
    string m_strFilePath;    //音樂檔案路徑;//
    bool   m_isLoop;         //是否循環撥放;//
    bool   m_isMusic;        //true代表為音樂,false代表為音效;//
    unsigned int m_nSoundId; //playEffect時回傳的ID;//
};

/////////////////////////////////////////////////////////////////////
//
//oSprite : 整合進音樂撥放功能
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
    
    //-1:持續回播, 0:停止播放, N:播放N次;//
    void AniPlay( int nLoop );                
    void AniPause( bool isResume );

    void SetMusic(MusicData *pData);
    void SetMusic(string strFile, bool isMusic, bool isLoop);
    void MusicPlay();
    void MusicStop();
    void MusicPause( bool isResume);
    
private:
    oAudio    m_Audio;       //音樂播放物件;//
    Animation *m_Animation;  //影格播放物件;//
    Action    *m_pAniAction; //影格撥放動作;//
};

#endif 
