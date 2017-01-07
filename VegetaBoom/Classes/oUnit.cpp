#include "oUnit.h"
#include "oConfig.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define AUDIO_INS CocosDenshion::SimpleAudioEngine::getInstance()

/////////////////////////////////////////////////////////////////////
//
//oAudio
// 播放檔案完整路徑不可以有中文
//
/////////////////////////////////////////////////////////////////////
oAudio::oAudio()
{
    m_IsCreated = false;
}

oAudio::~oAudio()
{
    if (!m_strFilePath.empty())
    {
        if(!m_isMusic)
            AUDIO_INS->unloadEffect(m_strFilePath.c_str());
    }
}

bool oAudio::Create(string strFile, bool isMusic, bool isLoop)
{
    if (!cocos2d::FileUtils::getInstance()->isFileExist(strFile))
        return false;

    //預先載入音樂或音效;//
    if (isMusic)
        AUDIO_INS->preloadBackgroundMusic(strFile.c_str());
    else
        AUDIO_INS->preloadEffect(strFile.c_str());
    
    m_strFilePath = strFile;
    m_isLoop = isLoop;
    m_isMusic = isMusic;
    m_IsCreated = true;

    return true;
}

void oAudio::Play()
{
    if (!m_IsCreated)
        return;

    if (m_isMusic)
    {
        if( AUDIO_INS->isBackgroundMusicPlaying() )
            AUDIO_INS->stopBackgroundMusic();
        AUDIO_INS->playBackgroundMusic(m_strFilePath.c_str(), m_isLoop);        
    }
    else
        m_nSoundId = AUDIO_INS->playEffect(m_strFilePath.c_str(), m_isLoop);
}

void oAudio::Stop()
{
    if (!m_IsCreated)
        return;

    if (m_isMusic)
        AUDIO_INS->stopBackgroundMusic();
    else
        AUDIO_INS->stopEffect(m_nSoundId);
}

void oAudio::Pause(bool isPause)
{
    if (!m_IsCreated)
        return;

    if (isPause)
    {
        if (m_isMusic)
            AUDIO_INS->pauseBackgroundMusic();
        else
            AUDIO_INS->pauseEffect(m_nSoundId);
    }
    else
    {
        if (m_isMusic)
            AUDIO_INS->resumeBackgroundMusic();
        else
            AUDIO_INS->resumeEffect(m_nSoundId);
    }
}

/////////////////////////////////////////////////////////////////////
//
//oSprite
//
/////////////////////////////////////////////////////////////////////
oSprite::oSprite()
{
    m_Animation = NULL;
}

oSprite::~oSprite()
{
    if(m_Animation)
        m_Animation->release();
}

oSprite* oSprite::create()
{
    oSprite *sprite = new (std::nothrow) oSprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

oSprite* oSprite::create(const std::string& filename)
{
    oSprite *sprite = new (std::nothrow) oSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void oSprite::SetAnimation(ImageData *pData)
{
    this->SetAnimation(pData->strFilePath, pData->nAniNum, pData->dAniSpeed);
    this->SetMusic(pData->Music.strFilePath, pData->Music.isMusic, pData->Music.isLoop);
}

void oSprite::SetAnimation(string strAniFile, int nAniNum, float fDelayPerUnit)
{
    std::size_t Found = strAniFile.find_last_of(".");
    if (Found == 0)
        return;

    m_Animation = Animation::create();

    string strFilePath = strAniFile.substr(0, Found);
    string strFileExtension = strAniFile.substr(Found);

    if (nAniNum > 1)
    {
        for (int i = 1; i <= nAniNum; i++)
        {
            char szName[128] = { 0 };
            sprintf(szName, "%s%02d%s", strFilePath.c_str(), i, strFileExtension.c_str());
            m_Animation->addSpriteFrameWithFile(szName);
        }
    }
    else
    {
        m_Animation->addSpriteFrameWithFile(strAniFile);
    }
    m_Animation->retain();

    //設定動畫撥放速度;//    
    float FPS = 1/Director::getInstance()->getAnimationInterval();
    m_Animation->setDelayPerUnit(fDelayPerUnit / FPS);
    m_Animation->setRestoreOriginalFrame(true);
}

void oSprite::AniPlay(int nLoop)
{
    if (!m_Animation)
        return;

    m_Animation->setLoops(nLoop);    
    Animate* Ani = Animate::create(m_Animation);    
    m_pAniAction = this->runAction(Ani);
}

void oSprite::AniPause(bool isResume)
{
    if (!m_Animation)
        return;    
    
    this->stopAction(m_pAniAction);
}

void oSprite::SetMusic(MusicData *pData)
{
    this->SetMusic(pData->strFilePath, pData->isMusic, pData->isLoop);
}

void oSprite::SetMusic(string strFile, bool isMusic, bool isLoop)
{
    m_Audio.Create(strFile, isMusic, isLoop);
}

void oSprite::MusicPlay()
{
    m_Audio.Play();
}

void oSprite::MusicStop()
{
    m_Audio.Stop();
}

void oSprite::MusicPause(bool isPause)
{
    m_Audio.Pause(isPause);
}

