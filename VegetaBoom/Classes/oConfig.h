#ifndef __O_CONFIG_H__
#define __O_CONFIG_H__

#include "cocos2d.h"
#include "json/rapidjson.h" 
#include "json/document.h"

using namespace std;

//字型資料;//
struct FontData
{
    string strJsonTag;
    bool   isTTf;       //是否為ttf檔;//
    string strFilePath; //字型檔路徑;//
    int    nWidth;      //字型寬度;//
    int    nHigh;       //字型高度;//
    int    nAlignment;  //對齊,0左 1中 2右;//
};

//音樂及音效資料;//
struct MusicData
{
    string strJsonTag;
    string strFilePath; //音樂檔案路徑;//    
    bool   isLoop;      //是否循環撥放;//
    bool   isMusic;     //true代表為音樂,false代表為音效;//
};

//圖形資料;//
struct ImageData
{
    string    strJsonTag;
    string    strFilePath; //圖形檔案路徑;//  
    int       nAniNum;     //動畫檔案張數;//
    double    dAniSpeed;   //每單位間隔撥放時間;//
    MusicData Music;       //圖形需要音效資料;//
};

//腳色資料;//
struct CharacterData
{
    string    strJsonTag;
    ImageData Rush;        //Rush圖檔;//
    ImageData Touch;       //Touch圖檔;//
    ImageData Fall;        //掉落圖檔;//
    ImageData Bomb;        //爆炸特效圖檔;//
    ImageData Child;       //子物件;//
    double    dScaleStart; //角色初始大小;//
    double    dScaleEnd;   //角色初始大小;//
    int       nType;       //角色種類;//
    int       nSatellite;  //衛星數量;//
    double    dMoveOutSec; //移動速度;//
    double    dBlowUpSec;  //放大爆炸的時間;//
};

/////////////////////////////////////////////////////////////////////
//
//oSceneConfig
//
/////////////////////////////////////////////////////////////////////
class oSceneConfig
{
public:

    oSceneConfig();
    ~oSceneConfig();

    FontData*      GetFont(string strName);
    MusicData*     GetMusic(string strName);
    ImageData*     GetImage(string strName);
    CharacterData* GetCharacter(string strName);

    vector< FontData >&      GetALLFont();
    vector< MusicData >&     GetALLMusic();
    vector< ImageData >&     GetALLImage();
    vector< CharacterData >& GetALLCharacter();

    void SetFont(FontData Data);
    void SetMusic(MusicData Data);
    void SetImage(ImageData Data);
    void SetCharacter(CharacterData Data);

private:
    vector< FontData >      m_FontData;
    vector< MusicData >     m_MusicData;
    vector< ImageData >     m_ImageData;
    vector< CharacterData > m_CharacterData;
};

/////////////////////////////////////////////////////////////////////
//
//oConfig
//
/////////////////////////////////////////////////////////////////////
class oConfig
{
public:
    static oConfig* getInstance();

    ~oConfig();

    bool LoadJsonConfig();

    oSceneConfig* GetSceneConfig(string strSceneName);

    //取得資料夾路徑;//
    string GetPathImage();
    string GetPathMusic();
    string GetPathCharacter();

    //取得錯誤訊息;//
    string GetErrorInfom();

private:

    oConfig();

    bool ParseBaseDir(rapidjson::Document& JsonDoc);

    oSceneConfig* ParseScene(rapidjson::Value& ObjScene);

    bool ParseObjFont(oSceneConfig* Scene, rapidjson::Value& Obj);
    bool ParseObjMusic(oSceneConfig* Scene, rapidjson::Value& Obj);
    bool ParseObjImage(oSceneConfig* Scene, rapidjson::Value& Obj);
    bool ParseObjCharacter(oSceneConfig* Scene, rapidjson::Value& Obj);

    FontData* ParseArrayFont(rapidjson::Value& Array, bool isUseDefaultDir = true);
    MusicData* ParseArrayMusic(rapidjson::Value& Array, bool isUseDefaultDir = true);
    ImageData* ParseArrayImage(rapidjson::Value& Array, bool isUseDefaultDir = true);

    //設定錯誤訊息;//
    void SetErrorInform(string strError, bool isNewLine = true);
    void SetErrorInform(string strError, string strMember, bool NewLine = true);

private:

    bool m_IsParsed;  //紀錄是否以解析過Config.json檔;//

                      //各檔案類型儲存的資料夾路徑;//
    string m_strDirFont;
    string m_strDirImage;
    string m_strDirMusic;
    string m_strDirCharacter;

    //各Scene的音樂,圖檔,腳色資訊;//
    map< string, oSceneConfig* > m_SceneData;

    //Error Infor;//
    string m_strErrorInfor;
};

#endif

