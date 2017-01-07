#ifndef __O_CONFIG_H__
#define __O_CONFIG_H__

#include "cocos2d.h"
#include "json/rapidjson.h" 
#include "json/document.h"

using namespace std;

//�r�����;//
struct FontData
{
    string strJsonTag;
    bool   isTTf;       //�O�_��ttf��;//
    string strFilePath; //�r���ɸ��|;//
    int    nWidth;      //�r���e��;//
    int    nHigh;       //�r������;//
    int    nAlignment;  //���,0�� 1�� 2�k;//
};

//���֤έ��ĸ��;//
struct MusicData
{
    string strJsonTag;
    string strFilePath; //�����ɮ׸��|;//    
    bool   isLoop;      //�O�_�`������;//
    bool   isMusic;     //true�N������,false�N������;//
};

//�ϧθ��;//
struct ImageData
{
    string    strJsonTag;
    string    strFilePath; //�ϧ��ɮ׸��|;//  
    int       nAniNum;     //�ʵe�ɮױi��;//
    double    dAniSpeed;   //�C��춡�j����ɶ�;//
    MusicData Music;       //�ϧλݭn���ĸ��;//
};

//�}����;//
struct CharacterData
{
    string    strJsonTag;
    ImageData Rush;        //Rush����;//
    ImageData Touch;       //Touch����;//
    ImageData Fall;        //��������;//
    ImageData Bomb;        //�z���S�Ĺ���;//
    ImageData Child;       //�l����;//
    double    dScaleStart; //�����l�j�p;//
    double    dScaleEnd;   //�����l�j�p;//
    int       nType;       //�������;//
    int       nSatellite;  //�ìP�ƶq;//
    double    dMoveOutSec; //���ʳt��;//
    double    dBlowUpSec;  //��j�z�����ɶ�;//
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

    //���o��Ƨ����|;//
    string GetPathImage();
    string GetPathMusic();
    string GetPathCharacter();

    //���o���~�T��;//
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

    //�]�w���~�T��;//
    void SetErrorInform(string strError, bool isNewLine = true);
    void SetErrorInform(string strError, string strMember, bool NewLine = true);

private:

    bool m_IsParsed;  //�����O�_�H�ѪR�LConfig.json��;//

                      //�U�ɮ������x�s����Ƨ����|;//
    string m_strDirFont;
    string m_strDirImage;
    string m_strDirMusic;
    string m_strDirCharacter;

    //�UScene������,����,�}���T;//
    map< string, oSceneConfig* > m_SceneData;

    //Error Infor;//
    string m_strErrorInfor;
};

#endif

