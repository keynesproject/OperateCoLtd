#include "oSceneGameplay.h"
#include "oItemCharacter.h"
#include "oConfig.h"
#include "oUnit.h"
#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#include "PluginAdColony/PluginAdColony.h"
#endif

#define  LOG_TAGG    "oSceneGamePlay.cpp"
#define  LOGGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAGG,__VA_ARGS__)

static std::string kHomeBanner = "home";
static std::string kGameOverAd = "gameover";

static std::function<void(const std::string &)> showText = nullptr;

cocos2d::Label *_status;
cocos2d::Label *_reward;

#ifdef SDKBOX_ENABLED

/////////////////////////////////////////////////////////////////////
//
//oListenerAdMob
//
/////////////////////////////////////////////////////////////////////
class oListenerAdMob : public sdkbox::AdMobListener 
{
public:

    //取得廣告來源;//
    virtual void adViewDidReceiveAd(const std::string &name) 
    {
        if (showText)
        {
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
            LOGGD("!!!!! 3333333333");
        }

        if (name == kHomeBanner)
        {
            LOGGD("!!!!! 18");
            sdkbox::PluginAdMob::show(name);
            LOGGD("!!!!! 44444444444");
            
        }
        LOGGD("!!!!! 555555555555");
    }


    virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) 
    {
        if (showText)
        {
            LOGGD("!!!!! 666666666666");
            showText(StringUtils::format("%s name=%s, msg=%s", __FUNCTION__, name.c_str(), msg.c_str()));
        }
        LOGGD("!!!!! 77777777777");
    }

    //點取廣告後，準備顯示廣告網站畫面;//
    virtual void adViewWillPresentScreen(const std::string &name) 
    {
        if (showText)
        {
            LOGGD("!!!!! 88888888888");
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
        }
        LOGGD("!!!!! 999999999999");
    }

    //關閉廣告,回到遊戲;//
    virtual void adViewDidDismissScreen(const std::string &name) 
    {
        if (showText)
        {
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
            LOGGD("!!!!! 10");
        }
        LOGGD("!!!!! 11");
    }


    virtual void adViewWillDismissScreen(const std::string &name) 
    {
        if (showText)
        {
            LOGGD("!!!!! 12");
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
        }

        if (name == "gameover") 
        {
            LOGGD("!!!!! 13");
            sdkbox::PluginAdMob::cache(kGameOverAd);
            LOGGD("!!!!! 14");
        }
        LOGGD("!!!!! 15");
    }

    //廣告點擊後離開APP;//
    virtual void adViewWillLeaveApplication(const std::string &name) 
    {
        if (showText)
        {
            LOGGD("!!!!! 16");
            showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
        }
        LOGGD("!!!!! 17");
    }
};

/////////////////////////////////////////////////////////////////////
//
//oListenerAdColony
//
/////////////////////////////////////////////////////////////////////
class oListenerAdColony : public sdkbox::AdColonyListener
{
public:
    //called when AdColony is finished loading.
    virtual void onAdColonyChange(const sdkbox::AdColonyAdInfo& info, bool available)
    {
        //    struct AdColonyAdInfo
        //    {
        //        std::string name;
        //        bool shown;
        //        std::string zoneID;
        //        bool iapEnabled;
        //        std::string iapProductID;
        //        int iapQuantity;
        //        int iapEngagementType;
        //    };

        _status->setString(__FUNCTION__);

        cocos2d::log("onAdColonyChange");
        cocos2d::log("info.name: %s", info.name.c_str());
        cocos2d::log("info.shown: %s", info.shown ? "true" : "false");
        cocos2d::log("info.zoneID: %s", info.zoneID.c_str());
        cocos2d::log("info.iapEnabled: %s", info.iapEnabled ? "true" : "false");
        cocos2d::log("info.iapProductID: %s", info.iapProductID.c_str());
        cocos2d::log("info.iapQuantity: %d", info.iapQuantity);
        cocos2d::log("info.iapEngagementType: %d", info.iapEngagementType);
    }
    
    // reward was received.
    virtual void onAdColonyReward(const sdkbox::AdColonyAdInfo& info, const std::string& currencyName, int amount, bool success)
    {
        _status->setString(__FUNCTION__);
        _reward->setString(StringUtils::toString((int)utils::atof(_reward->getString().c_str()) + amount));

        cocos2d::log("onAdColonyReward, currencyName: %s, amount: %d, success: %s", currencyName.c_str(), amount, success ? "true" : "false");
        cocos2d::log("info.name: %s", info.name.c_str());
        cocos2d::log("info.shown: %s", info.shown ? "true" : "false");
        cocos2d::log("info.zoneID: %s", info.zoneID.c_str());
        cocos2d::log("info.iapEnabled: %s", info.iapEnabled ? "true" : "false");
        cocos2d::log("info.iapProductID: %s", info.iapProductID.c_str());
        cocos2d::log("info.iapQuantity: %d", info.iapQuantity);
        cocos2d::log("info.iapEngagementType: %d", info.iapEngagementType);
    }

    //showing an ad has started.    
    virtual void onAdColonyStarted(const sdkbox::AdColonyAdInfo& info)
    {
        _status->setString(__FUNCTION__);

        cocos2d::log("onAdColonyStarted");
        cocos2d::log("info.name: %s", info.name.c_str());
        cocos2d::log("info.shown: %s", info.shown ? "true" : "false");
        cocos2d::log("info.zoneID: %s", info.zoneID.c_str());
        cocos2d::log("info.iapEnabled: %s", info.iapEnabled ? "true" : "false");
        cocos2d::log("info.iapProductID: %s", info.iapProductID.c_str());
        cocos2d::log("info.iapQuantity: %d", info.iapQuantity);
        cocos2d::log("info.iapEngagementType: %d", info.iapEngagementType);
    }
    
    //showing an ad has finished.
    virtual void onAdColonyFinished(const sdkbox::AdColonyAdInfo& info)
    {
        _status->setString(__FUNCTION__);

        cocos2d::log("onAdColonyFinished");
        cocos2d::log("info.name: %s", info.name.c_str());
        cocos2d::log("info.shown: %s", info.shown ? "true" : "false");
        cocos2d::log("info.zoneID: %s", info.zoneID.c_str());
        cocos2d::log("info.iapEnabled: %s", info.iapEnabled ? "true" : "false");
        cocos2d::log("info.iapProductID: %s", info.iapProductID.c_str());
        cocos2d::log("info.iapQuantity: %d", info.iapQuantity);
        cocos2d::log("info.iapEngagementType: %d", info.iapEngagementType);
    }
};
#endif

/////////////////////////////////////////////////////////////////////
//
//oLayerUiSetting
//
/////////////////////////////////////////////////////////////////////
Scene* oLayerUiSetting::createScene()
{
    // 'scene' is an autorelease object
    auto Scene = Scene::create();

    // 'layer' is an autorelease object
    auto Layer = oLayerUiSetting::create();

    // add layer as a child to scene
    Scene->addChild(Layer);

    // return the scene
    return Scene;
}

bool oLayerUiSetting::init()
{
    if (!Layer::init())
    {
        return false;
    }


    return true;
}

void oLayerUiSetting::update(float dt)
{
}

/////////////////////////////////////////////////////////////////////
//
//oLayerBackGround
//
/////////////////////////////////////////////////////////////////////
Scene* oLayerBackground::createScene()
{
    // 'scene' is an autorelease object
    auto Scene = Scene::create();

    // 'layer' is an autorelease object
    auto Layer = oLayerBackground::create();

    // add layer as a child to scene
    Scene->addChild(Layer);

    // return the scene
    return Scene;
}

bool oLayerBackground::init()
{
    if (!Layer::init())
    {
        return false;
    }

    //取得Scene設定資料;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneGamePlay");
    if (!SceneConfig)
        return false;

    ImageData *pImgData = SceneConfig->GetImage("Cloud");
    std::size_t Found = pImgData->strFilePath.find_last_of(".");
    if (Found == 0)
        return false;

    string strFilePath = pImgData->strFilePath.substr(0, Found);
    string strFileExtension = pImgData->strFilePath.substr(Found);    

    //建立雲材質;//
    for(int i = 1; i <= pImgData->nAniNum; i++)
    {        
        char szName[128] = { 0 };
        sprintf(szName, "%s0%d%s", strFilePath.c_str(), i, strFileExtension.c_str());
        Texture2D *Texture = Director::getInstance()->getTextureCache()->addImage( szName );
        m_TextureCloud.push_back( Texture );
    }

    //建立雲Sprite;//
    for( int i = 0; i <= 20; i++ )
    {
        //建立一個雲Sprite,;//
        m_sClouds.push_back( Sprite::create() );   
        m_sClouds[i]->setVisible(false);
        this->addChild( m_sClouds[i], 5 );
    }  

    schedule( schedule_selector(oLayerBackground::ShowCloud), 0.8f, kRepeatForever, 0);


    //定時調度更新;//
    scheduleUpdate();    
    
    return true;
}

void oLayerBackground::ShowCloud( float dt )
{
    //Random 一個數值 MOD 10,若大於3則產生新的雲;//
    int Random = cocos2d::RandomHelper::random_int( 0, 10 );
    
    if ( Random < 5 )
        return;

    //取得未顯示的雲Sprite;//    
    for ( size_t i = 0; i < m_sClouds.size(); i++ )
    {
        if ( m_sClouds[i]->isVisible() == false )
        {
            //顯示雲;//
            m_sClouds[i]->setVisible( true );

            //隨機指定雲材質;//
            Random = cocos2d::RandomHelper::random_int( 0, 8 );
            m_sClouds[i]->setTexture( m_TextureCloud[Random] );
            Rect TextureRect = Rect::ZERO;
            TextureRect.size = m_TextureCloud[Random]->getContentSize();
            m_sClouds[i]->setTextureRect( TextureRect, false, TextureRect.size );
            
            //設定位置至畫面頂端;//
            auto ScreenSize = Director::getInstance()->getVisibleSize();
            Random = cocos2d::RandomHelper::random_int( 0, (int)(ScreenSize.width - m_sClouds[i]->getContentSize().width) );
            m_sClouds[i]->setPosition( Vec2( Random, ScreenSize.height + m_sClouds[i]->getContentSize().height ) );
            
            //設定移動路徑動作;//
            Random = cocos2d::RandomHelper::random_int(4, 7);
            auto MoveTo = MoveTo::create( Random, Vec2(m_sClouds[i]->getPositionX(), -10) );
            auto HideAction = Hide::create();
            auto CloudAction = Sequence::create(MoveTo, HideAction, NULL);
            m_sClouds[i]->runAction(CloudAction);
            
            //設定透明度;//
            Random = cocos2d::RandomHelper::random_int( 2, 6 );
            m_sClouds[i]->setCascadeOpacityEnabled(true);
            m_sClouds[i]->setOpacity( 255 * Random * 0.1f );            

            return;
        }
    }
}

void oLayerBackground::update(float dt)
{
}

/////////////////////////////////////////////////////////////////////
//
//oSceneGamePlay
//
/////////////////////////////////////////////////////////////////////
Scene* oSceneGamePlay::createScene()
{
    // 'scene' is an autorelease object
    auto Scene = Scene::create();

    //建立主遊戲場景
    auto LayerGamePlay = oSceneGamePlay::create();
    LayerGamePlay->setName("SceneGamePlay");
    
    // add layer as a child to scene
    Scene->addChild( LayerGamePlay, 0 );
    
    //兩種方式抓取 background Layer;//
    //cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(1)->pause();
    //cocos2d::Director::getInstance()->getRunningScene()->getChildByName("LayerBackground")->resume();

    // return the scene
    return Scene;
}

bool oSceneGamePlay::init()
{
    //初始化底圖為淡藍色;//
    if (!LayerColor::initWithColor(Color4B(150, 222, 221, 255)))
    {
        return false;
    }
    
    //取得Scene設定資料;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneGamePlay");
    if (!SceneConfig)
        return false;
    
    //建立背景圖層-雲的移動;//
    m_LayerBg = oLayerBackground::create();
    this->addChild(m_LayerBg, 0);  
    
    MusicData *pMusicData = SceneConfig->GetMusic("BgGamePlay");
    m_pMusicBg = new oAudio();
    m_pMusicBg->Create(pMusicData->strFilePath, pMusicData->isMusic, pMusicData->isLoop);
    m_pMusicBg->Play();

    //角色物件;//    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    m_pNextActor = NULL;
    vector<CharacterData> AllActors = SceneConfig->GetALLCharacter();
    for (size_t i = 0; i < AllActors.size(); i++)
    {
        oCharacter* pActor = oCharacter::create();
        pActor->Setup(&AllActors[i]);
        pActor->setPosition(visibleSize.width / 2, visibleSize.height / 4);
        pActor->setVisible(false);
        this->addChild(pActor, 1);
        m_Actors.push_back(pActor);
    }
    m_nActionActorNo = 2;
    m_pActionActor = m_Actors[2];
    m_pActionActor->StartAction(true);

    //數字;//
    FontData *FontNum = SceneConfig->GetFont("FontNumDistance");    
    m_Distance = Label::createWithCharMap(FontNum->strFilePath, FontNum->nWidth, FontNum->nHigh, '0');
    m_Distance->setAnchorPoint(Vec2(1, 1));  
    m_Distance->setScale(3);
    m_Distance->setString("123456");
    m_Distance->setPosition(visibleSize.width-10, visibleSize.height-10);
    m_Distance->setHorizontalAlignment(TextHAlignment::RIGHT);
    this->addChild(m_Distance, 998);

    //建立滑鼠按鍵事件;//
    auto MouseDownListener = EventListenerMouse::create();
    MouseDownListener->onMouseDown = CC_CALLBACK_1(oSceneGamePlay::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseDownListener, this);

    auto MouseUpListener = EventListenerMouse::create();
    MouseUpListener->onMouseUp = CC_CALLBACK_1(oSceneGamePlay::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseUpListener, this);
    
    //建立暫停按鈕;//    
    string strBtnN = SceneConfig->GetImage("BtnPauseN")->strFilePath;
    string strBtnS = SceneConfig->GetImage("BtnPauseS")->strFilePath;
    auto PauseItem = MenuItemImage::create(strBtnN.c_str(), strBtnS.c_str(), CC_CALLBACK_1(oSceneGamePlay::MenuPauseCallback, this));
    //PauseItem->setScale(2);
    m_BtnPause = Menu::create(PauseItem, NULL);
    m_BtnPause->setPosition(30, 30);
    this->addChild(m_BtnPause, 999);
    
    //建立繼續按鈕;//
    strBtnN = SceneConfig->GetImage("BtnResumeN")->strFilePath;
    strBtnS = SceneConfig->GetImage("BtnResumeS")->strFilePath;
    auto ResumeItem = MenuItemImage::create(strBtnN.c_str(), strBtnS.c_str(), CC_CALLBACK_1(oSceneGamePlay::MenuResumeCallback, this));
    m_BtnResume = Menu::create(ResumeItem, NULL);
    m_BtnResume->setPosition(30, 30);
    m_BtnResume->setVisible(false);
    this->addChild(m_BtnResume, 999);

    //建立暫定介面;//
    m_LayerUiSetting = oLayerUiSetting::create();
    m_LayerUiSetting->setVisible(false);
    m_LayerUiSetting->pause();
    this->addChild(m_LayerUiSetting, 999);

#ifdef SDKBOX_ENABLED
    cocos2d::log("[ oSceneGamePlay ] 1111111");
    //createTestMenuAdMob();
    createTestMenuAdColony();
    cocos2d::log("[ oSceneGamePlay ] 2222222");
#endif

    //開始定時調度;//
    scheduleUpdate();    
    
    return true;
}

void oSceneGamePlay::createTestMenuAdMob()
{
    auto size = Director::getInstance()->getWinSize();

    auto label = Label::createWithSystemFont("Hello cpp", "Arial", 32);
    label->setPosition(size.width / 2, 100);
    label->setColor(Color3B(255, 0, 0));
    addChild(label);

    showText = [=](const std::string &text)
    {
        cocos2d::log("%s", text.c_str());
        label->setString(text);
    };

    MenuItemFont::setFontName("Arial");
#ifdef SDKBOX_ENABLED
    // ui
    {
        Menu* menu = Menu::create(
            MenuItemFont::create("load banner", [](Ref*) { sdkbox::PluginAdMob::cache(kHomeBanner); }),
            MenuItemFont::create("show banner", [](Ref*) { sdkbox::PluginAdMob::show(kHomeBanner);  }),
            MenuItemFont::create("hide banner", [](Ref*) { sdkbox::PluginAdMob::hide(kHomeBanner);  }),
            MenuItemFont::create("is banner available", [=](Ref*) {
            showText(StringUtils::format("is %s available %d", kHomeBanner.c_str(), sdkbox::PluginAdMob::isAvailable(kHomeBanner)));
        }),

            MenuItemFont::create("load interstitial", [](Ref*) { sdkbox::PluginAdMob::cache(kGameOverAd); }),
            MenuItemFont::create("show interstitial", [](Ref*) { sdkbox::PluginAdMob::show(kGameOverAd);  }),
            MenuItemFont::create("is interstital available", [=](Ref*) {
            showText(StringUtils::format("is %s available %d", kGameOverAd.c_str(), sdkbox::PluginAdMob::isAvailable(kGameOverAd)));
        }),
            NULL);
        menu->alignItemsVerticallyWithPadding(20);
        menu->setPosition(size.width / 2, size.height / 2);
        addChild(menu);
    }

    sdkbox::PluginAdMob::setListener(new oListenerAdMob());
#endif
}

void oSceneGamePlay::createTestMenuAdColony()
{
    cocos2d::log("[ oSceneGamePlay ] 3");
    Size size = Director::getInstance()->getWinSize();
    cocos2d::log("[ oSceneGamePlay ] 4");
    // add status label
    _status = Label::createWithSystemFont("No event.", "Arial", 32);
    _status->setTextColor(Color4B(0, 255, 0, 255));
    _status->setAnchorPoint(Vec2(0, 0));
    _status->setPosition(5, 5);
    addChild(_status);
    cocos2d::log("[ oSceneGamePlay ] 5");
    // reward label
    _reward = Label::createWithSystemFont("0", "Arial", 32);
    _reward->setPosition(size.width / 2, 30);
    addChild(_reward);
    cocos2d::log("[ oSceneGamePlay ] 6");
    MenuItemFont::setFontName("Arial");
    cocos2d::log("[ oSceneGamePlay ] 7");
    auto menu = Menu::create(
        MenuItemFont::create("show video", CC_CALLBACK_1(oSceneGamePlay::onShowVideo, this)),
        MenuItemFont::create("show v4vc", CC_CALLBACK_1(oSceneGamePlay::onShowV4vc, this)),
        NULL);
    cocos2d::log("[ oSceneGamePlay ] 8");
    menu->alignItemsVerticallyWithPadding(5);
    menu->setPosition(Vec2(size.width / 2, size.height / 2));
    addChild(menu);
    cocos2d::log("[ oSceneGamePlay ] 9");
#ifdef SDKBOX_ENABLED
    sdkbox::PluginAdColony::setListener(new oListenerAdColony());
#endif
    cocos2d::log("[ oSceneGamePlay ] 10");
}

void oSceneGamePlay::onShowVideo(cocos2d::Ref* sender)
{
#ifdef SDKBOX_ENABLED
    sdkbox::PluginAdColony::show("video");
    cocos2d::log("onShowVideo");
#endif
}

void oSceneGamePlay::onShowV4vc(cocos2d::Ref* sender)
{
#ifdef SDKBOX_ENABLED
    sdkbox::PluginAdColony::show("v4vc");
    cocos2d::log("onShowVideo");
#endif
}

void oSceneGamePlay::update(float dt)
{
    CharacterState State = m_pActionActor->Action();
    if (m_pNextActor)
        m_pNextActor->Action();
    switch (State)
    {
        case eSTATE_BOMB:
        {
            static int AA = 0;
            AA++;
            if (AA == 3)
            {
                int a = 5;
            }
            //表示執行到爆炸動畫,要顯示出新的角色;//
            int Random; 
            do
            {
                Random = cocos2d::RandomHelper::random_int(0, (int)m_Actors.size()-1);
            } while (m_nActionActorNo == Random);
            m_nActionActorNo = Random;
            m_pNextActor = m_Actors[m_nActionActorNo];
            m_pNextActor->setVisible(true);
            Vec2 Pos(m_pActionActor->getPositionX(), Director::getInstance()->getVisibleSize().height/4);
            m_pNextActor->setPosition(Pos);
            m_pNextActor->StartAction();
        }
        break;

        case eSTATE_STOP:
        {
            //表示一輪執行完畢,停止角色運作;//
            m_pActionActor = m_pNextActor;
            m_pNextActor = NULL;
        }
        break;

        case eSTATE_FAILURE:
        {
            //顯示Game Over;//
            int a = 5;
        }
        break;

        default:
            break;
    }
}

void oSceneGamePlay::onMouseDown(Event *event)
{
    m_pActionActor->SetHitState(true);
    if(m_pNextActor)
        m_pNextActor->SetHitState(true);
    //Director::getInstance()->pause();
}

void oSceneGamePlay::onMouseUp(Event *event)
{
    m_pActionActor->SetHitState(false);
    if (m_pNextActor)
        m_pNextActor->SetHitState(false);
    //Director::getInstance()->resume();
}

void oSceneGamePlay::MenuPauseCallback(cocos2d::Ref* pSender)
{        
    //關閉暫停按鍵，顯示Play按鍵;//
    m_BtnPause->setVisible(false);
    m_BtnResume->setVisible(true);

    //場景及背景場景暫停;//
    this->pause();
    m_LayerBg->pauseSchedulerAndActions();
    m_ActionNode = m_LayerBg->getActionManager()->pauseAllRunningActions();

    //顯示設定介面;//
    m_LayerUiSetting->setVisible(true);
    m_LayerUiSetting->resume();    
}

void oSceneGamePlay::MenuResumeCallback(cocos2d::Ref* pSender)
{
    //關閉Play按鍵，顯示暫停按鍵;//
    m_BtnPause->setVisible(true);
    m_BtnResume->setVisible(false);

    //場景及背景場景回復執行;//
    this->resume();
    m_LayerBg->resumeSchedulerAndActions();
    m_LayerBg->getActionManager()->resumeTargets(m_ActionNode);

    //關閉設定介面;//
    m_LayerUiSetting->setVisible(false);
    m_LayerUiSetting->pause();
}
