#include "oConfig.h"
#include "oSceneLogo.h"
#include "oSceneGamePlay.h"
#include "oUnit.h"

USING_NS_CC;

Scene* oSceneLogo::createScene()
{
    // 'scene' is an autorelease object
    auto Scene = Scene::create();

    // 'layer' is an autorelease object
    auto Layer = oSceneLogo::create();

    // add layer as a child to scene
    Scene->addChild(Layer);

    // return the scene
    return Scene;
}

bool oSceneLogo::init()
{
    //��l�e�ϼh�A�ó]�w���⬰�զ�;//
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //���oScene�]�w���;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneLogo");
    if (!SceneConfig)
        return false;

    //���o������;//
    MusicData *pMusicData = SceneConfig->GetMusic("LogoStart");  
    m_SoundStart = new oAudio();
    m_SoundStart->Create(pMusicData->strFilePath, pMusicData->isMusic, pMusicData->isLoop);

    //���o����;//
    ImageData *pImgData = SceneConfig->GetImage("Logo");
    m_sLogo = oSprite::create(pImgData->strFilePath);
    m_sLogo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    m_sLogo->setScale(5);
    this->addChild(m_sLogo, 0);

    //�إ߷ƹ����U�ƥ�;//
    auto MouseListener = EventListenerMouse::create();
    MouseListener->onMouseDown = CC_CALLBACK_1(oSceneLogo::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseListener, this);

    scheduleOnce(schedule_selector(oSceneLogo::UpdateOnce), 5.0f);
    
    return true;
}

void oSceneLogo::onExit()
{
    if (m_SoundStart != NULL)
        delete m_SoundStart;
    m_SoundStart = NULL;
    
    LayerColor::onExit();    
}

void oSceneLogo::onMouseDown(Event *event)
{
    m_SoundStart->Play();

    //�ƹ����U������ܹC������;//
    auto SceneGamePlay = oSceneGamePlay::createScene();
    Director::getInstance()->replaceScene(SceneGamePlay);
}

void oSceneLogo::UpdateOnce(float DelayTime)
{    
    m_SoundStart->Play();

    //�g�LDelay�ɶ���A�����ܤU�ӳ���;//
    auto SceneGamePlay = oSceneGamePlay::createScene();
    Director::getInstance()->replaceScene(SceneGamePlay);
}


