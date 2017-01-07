#include "oConfig.h"

const char *ccDirFont = "DirFont";
const char *ccDirImage = "DirImage";
const char *ccDirMusic = "DirMusic";
const char *ccDirCharacter = "DirCharacter";
const char *ccHelp = "__HELP";

/////////////////////////////////////////////////////////////////////
//
//oSceneConfig
//
/////////////////////////////////////////////////////////////////////
oSceneConfig::oSceneConfig()
{

}

oSceneConfig::~oSceneConfig()
{
    
}

FontData* oSceneConfig::GetFont(string strName)
{
    vector< FontData >::iterator it = m_FontData.begin();
    for (; it != m_FontData.end(); it++)
    {
        if (strName.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

MusicData* oSceneConfig::GetMusic(string strName)
{
    vector< MusicData >::iterator it = m_MusicData.begin();
    for (; it != m_MusicData.end(); it++)
    {
        if (strName.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

ImageData* oSceneConfig::GetImage(string strName)
{
    vector< ImageData >::iterator it = m_ImageData.begin();
    for (; it != m_ImageData.end(); it++)
    {
        if (strName.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

CharacterData* oSceneConfig::GetCharacter(string strName)
{
    vector< CharacterData >::iterator it = m_CharacterData.begin();
    for (; it != m_CharacterData.end(); it++)
    {
        if (strName.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

vector< FontData >& oSceneConfig::GetALLFont()
{
    return m_FontData;
}

vector< MusicData >& oSceneConfig::GetALLMusic()
{
    return m_MusicData;
}

vector< ImageData >& oSceneConfig::GetALLImage()
{
    return m_ImageData;
}

vector< CharacterData >& oSceneConfig::GetALLCharacter()
{
    return m_CharacterData;
}

void oSceneConfig::SetFont(FontData Data)
{
    m_FontData.push_back(Data);
}

void oSceneConfig::SetMusic(MusicData Data)
{
    m_MusicData.push_back(Data);
}

void oSceneConfig::SetImage(ImageData Data)
{
    m_ImageData.push_back(Data);
}

void oSceneConfig::SetCharacter(CharacterData Data)
{
    m_CharacterData.push_back(Data);
}

/////////////////////////////////////////////////////////////////////
//
//oConfig
//
/////////////////////////////////////////////////////////////////////
oConfig *oConfig::getInstance()
{
    static oConfig m_Instance;

    return &m_Instance;
}

oConfig::oConfig()
{
    m_IsParsed = false;
}

oConfig::~oConfig()
{
    map< string, oSceneConfig*>::iterator it = m_SceneData.begin();
    for (; it != m_SceneData.end(); it++)
    {
        if (it->second != NULL)
        {
            delete it->second;
            it->second = NULL;
        }
    }
    m_SceneData.clear();
}

oSceneConfig* oConfig::GetSceneConfig(string strSceneName)
{
    //���P�_�O�_�ѪRConfig����;//
    if (!LoadJsonConfig())
        return NULL;

    map< string, oSceneConfig* >::iterator it = m_SceneData.find(strSceneName);
    if (it != m_SceneData.end())
        return it->second;

    return NULL;
}

string oConfig::GetPathImage()
{
    return m_strDirImage;
}

string oConfig::GetPathMusic()
{
    return m_strDirMusic;
}

string oConfig::GetPathCharacter()
{
    return m_strDirCharacter;
}

bool oConfig::LoadJsonConfig()
{
    if (m_IsParsed)
        return true;

    //Ū��Json�ɮ�;//
    std::string strJson = cocos2d::FileUtils::getInstance()->getStringFromFile("Config.json");
    if (strJson.empty())
    {
        SetErrorInform("Config.json file not found!");
        return false;
    }

    //�ѪRJSON�ɮ�;//
    rapidjson::Document JsonDoc;
    JsonDoc.Parse<rapidjson::kParseDefaultFlags>(strJson.c_str());

    //�P�_�O�_�ѪR���~;//
    if (JsonDoc.HasParseError())
    {
        SetErrorInform("Config.json format error!");
        return false;
    }

    //�P�_�O�_��Object;//
    if (!JsonDoc.IsObject())
        return false;

    //���o��Ƨ��������|;//
    ParseBaseDir(JsonDoc);

    //���o�r�����|;//
    //ParseFont(JsonDoc);

    //Ū���æUScene���;//
    rapidjson::Value::ConstMemberIterator it = JsonDoc.MemberBegin();
    for (; it != JsonDoc.MemberEnd(); it++ )
    {
        //�P�_�O�_��Object�榡;//
        string Name = it->name.GetString();
        if (JsonDoc[Name.c_str()].IsObject())
        {
            //�ѪRScene���;//
            oSceneConfig *Scene = ParseScene(JsonDoc[Name.c_str()]);
            if (Scene == NULL)
            {
                SetErrorInform("Scene :", Name);
                return false;
            }

            m_SceneData.insert(pair< string, oSceneConfig*>(Name, Scene));
        }
    }
    
    m_IsParsed = true;

    return true;
}

bool oConfig::ParseBaseDir(rapidjson::Document& JsonDoc)
{    
    if (!JsonDoc.HasMember(ccDirFont))
    {
        SetErrorInform("Base dir not found : ", (string)ccDirFont);
        return false;
    }
    m_strDirFont = JsonDoc[ccDirFont].GetString();

    if (!JsonDoc.HasMember(ccDirImage))
    {
        SetErrorInform("Base dir not found : ", (string)ccDirImage);
        return false;
    }
    m_strDirImage = JsonDoc[ccDirImage].GetString();

    if (!JsonDoc.HasMember(ccDirMusic))
    {
        SetErrorInform("Base dir not found : ", (string)ccDirMusic);
        return false;
    }
    m_strDirMusic = JsonDoc[ccDirMusic].GetString();

    if (!JsonDoc.HasMember(ccDirCharacter))
    {
        SetErrorInform("Base dir not found : ", (string)ccDirCharacter);
        return false;
    }
    m_strDirCharacter = JsonDoc[ccDirCharacter].GetString();
    
    return true;
}

oSceneConfig* oConfig::ParseScene(rapidjson::Value& ObjScene)
{
    oSceneConfig *Scene = new oSceneConfig;    

    //�ѪR�r�����;//
    if (ObjScene.HasMember("Font"))
    {
        if (!ParseObjFont(Scene, ObjScene["Font"]))
        {
            delete Scene;
            return NULL;
        }
    }

    //�ѪR���ָ��;//
    if (ObjScene.HasMember("Music"))
    {
        if (!ParseObjMusic(Scene, ObjScene["Music"]))
        {
            delete Scene;
            return NULL;
        }
    }

    //�ѪR�ϧθ��;//
    if (ObjScene.HasMember("Image"))
    {
        if (!ParseObjImage(Scene, ObjScene["Image"]))
        {
            delete Scene;
            return NULL;
        }
    }

    //�ѪR������;//
    if (ObjScene.HasMember("Character"))
    {
        if (!ParseObjCharacter(Scene, ObjScene["Character"]))
        {
            delete Scene;
            return NULL;
        }
    }

    return Scene;
}

bool oConfig::ParseObjFont(oSceneConfig* Scene, rapidjson::Value& Obj)
{
    rapidjson::Value::ConstMemberIterator it = Obj.MemberBegin();
    for (; it != Obj.MemberEnd(); it++)
    {
        //���o����W��;//
        string Name = it->name.GetString();

        //�Y����W�٬�__HELP��ܬO�����C,�����ѪR���;//
        if (Name.compare(ccHelp) == 0)
            continue;

        //�ˬd�榡�O�_��Array;//
        if (!Obj[Name.c_str()].IsArray())
        {
            SetErrorInform("Parse Obj Font not array: ", Name);
            return false;
        }

        //�ѪRFont���;//
        FontData* Data = ParseArrayFont(Obj[Name.c_str()]);
        if (Data == NULL)
        {
            SetErrorInform("Parse Obj Font : ", Name);
            return false;
        }

        //�x�s�r�����;//
        Data->strJsonTag = Name;
        Scene->SetFont(*Data);
        delete Data;
        Data = NULL;
    }
    return true;
}

bool oConfig::ParseObjMusic(oSceneConfig* Scene, rapidjson::Value& Obj)
{
    rapidjson::Value::ConstMemberIterator it = Obj.MemberBegin();
    for (; it != Obj.MemberEnd(); it++ )
    {
        //���o����W��;//
        string Name = it->name.GetString();

        //�Y����W�٬�__HELP��ܬO�����C,�����ѪR���;//
        if (Name.compare(ccHelp) == 0 )
            continue;

        //�ˬd�榡�O�_��Array;//
        if (!Obj[Name.c_str()].IsArray())
        {
            SetErrorInform("Parse Obj Music not array: ", Name);
            return false;
        }

        //�ѪRMusic���;//
        MusicData* Data = ParseArrayMusic(Obj[Name.c_str()]);
        if (Data == NULL)
        {
            SetErrorInform("Parse Obj Music : ", Name);
            return false;
        }

        //�x�s���ָ��;//
        Data->strJsonTag = Name;
        Scene->SetMusic(*Data);
        delete Data;
        Data = NULL;
    }
    return true;
}

bool oConfig::ParseObjImage(oSceneConfig* Scene, rapidjson::Value& Obj)
{    
    rapidjson::Value::ConstMemberIterator it = Obj.MemberBegin();
    for (; it != Obj.MemberEnd(); it++)
    {
        //���o����W��;//
        string Name = it->name.GetString();

        //�Y����W�٬�__HELP��ܬO�����C,�����ѪR���;//
        if (Name.compare(ccHelp) == 0)
            continue;

        //�ˬd�榡�O�_��Array;//
        if (!Obj[Name.c_str()].IsArray())
        {
            SetErrorInform("Parse Obj Image not array: ", Name);
            return false;
        }
           
        //�ѪRImage���;//
        ImageData* Data = ParseArrayImage(Obj[Name.c_str()]);
        if (Data == NULL)
        {
            SetErrorInform("Parse Obj Image : ", Name);
            return false;
        }

        //�x�s�ϧθ��;//
        Data->strJsonTag = Name;
        Scene->SetImage(*Data);
        delete Data;
        Data = NULL;
    }

    return true;
}

bool oConfig::ParseObjCharacter(oSceneConfig* Scene, rapidjson::Value& Obj)
{
    //���o�U������;//
    rapidjson::Value::ConstMemberIterator itCharacter = Obj.MemberBegin();
    for (; itCharacter != Obj.MemberEnd(); itCharacter++)
    {
        //�榡Sample;//
        //"Vege" :
        //{
        //    "ImgRush"   : ["Rush_Vege.png", 2, 3.0, ["VegeRush.mp3", true, false]],
        //    "ImgTouch" : ["Touch_Vege.png", 2, 3.0, ["VegeTouch.mp3", false, false]],
        //    "ImgFall" : ["Fall_Vege.png", 1, 1.0, ["VegeFall.mp3", false, false]],
        //    "ImgBomb" : ["Bomb_Vege.png", 5, 3.0, ["VegeBomb.mp3", false, false]],
        //    "ScaleStart" : 0.5,
        //    "ScaleEnd" : 1.0,
        //    "Type" : 1,
        //    "Satellite" : 0,
        //    "Speed" : 10.0,
        //    "BlowUpSec" : 3.0
        //}

        //���o��@������;//
        rapidjson::Value& Item = Obj[itCharacter->name.GetString()];
        if (!Item.IsObject())
        {
            SetErrorInform("Parse Obj Character not object: ", (string)itCharacter->name.GetString());
            return false;
        }

        if (Item.MemberCount() != 10)
        {
            char Member[8];
            sprintf(Member, "%d", Item.MemberCount());
            SetErrorInform("Parse Obj Character Member not 10 : ", (string)Member);
            return false;
        }

        CharacterData Data;
        ImageData *ptrImgData;

        //�ѪRRush�ϸ�;//
        if (!Item.HasMember("ImgRush"))
        {
            SetErrorInform("Parse Obj Character not has member : ImgRush");
            return false;
        }
        ptrImgData = ParseArrayImage( Item["ImgRush"], false );
        if (ptrImgData == NULL)
            return false;
        Data.Rush = *ptrImgData;
        delete ptrImgData;

        //�ѪRTouch�ϸ�;//
        if (!Item.HasMember("ImgTouch"))
        {
            SetErrorInform("Parse Obj Character not has member : ImgTouch");
            return false;
        }
        ptrImgData = ParseArrayImage(Item["ImgTouch"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Touch = *ptrImgData;
        delete ptrImgData;

        //�ѪRFall�ϸ�;//
        if (!Item.HasMember("ImgFall"))
        {
            SetErrorInform("Parse Obj Character not has member : ImgFall");
            return false;
        }
        ptrImgData = ParseArrayImage(Item["ImgFall"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Fall = *ptrImgData;
        delete ptrImgData;

        //�ѪRBomb�ϸ�;//
        if (!Item.HasMember("ImgBomb"))
        {
            SetErrorInform("Parse Obj Character not has member : ImgBomb");
            return false;
        }
        ptrImgData = ParseArrayImage(Item["ImgBomb"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Bomb = *ptrImgData;
        delete ptrImgData;

        //�ѪR�����l�j�p;//        
        if (!Item.HasMember("ScaleStart"))
        {
            SetErrorInform("Parse Obj Character not has member : ScaleStart");
            return false;
        }
        Data.dScaleStart = Item["ScaleStart"].GetDouble();

        //�ѪR�����l�j�p;//        
        if (!Item.HasMember("ScaleEnd"))
        {
            SetErrorInform("Parse Obj Character not has member : ScaleEnd");
            return false;
        }
        Data.dScaleEnd = Item["ScaleEnd"].GetDouble();

        //�ѪR�������;//        
        if (!Item.HasMember("Type"))
        {
            SetErrorInform("Parse Obj Character not has member : Type");
            return false;
        }
        Data.nType = Item["Type"].GetInt();

        //�ѪR�ìP�ƶq;//
        if (!Item.HasMember("Satellite"))
        {
            SetErrorInform("Parse Obj Character not has member : Satellite");
            return false;
        }
        Data.nSatellite = Item["Satellite"].GetInt();

        //�ѪR���ʳt��;//
        if (!Item.HasMember("MoveOutSec"))
        {
            SetErrorInform("Parse Obj Character not has member : MoveOutSec");
            return false;
        }
        Data.dMoveOutSec = Item["MoveOutSec"].GetDouble();

        //�ѪR�z���ɶ�;//
        if (!Item.HasMember("BlowUpSec"))
        {
            SetErrorInform("Parse Obj Character not has member : BlowUpSec");
            return false;
        }
        Data.dBlowUpSec = Item["BlowUpSec"].GetDouble();

        //�ק��ɮ׸��|;//
        string CurrentDir = m_strDirCharacter;
        CurrentDir.append(itCharacter->name.GetString());
        CurrentDir.append("/");
        Data.Rush.strFilePath.insert(0, CurrentDir);
        Data.Touch.strFilePath.insert(0, CurrentDir);
        Data.Fall.strFilePath.insert(0, CurrentDir);
        Data.Bomb.strFilePath.insert(0, CurrentDir);
        Data.Rush.Music.strFilePath.insert(0, CurrentDir);
        Data.Touch.Music.strFilePath.insert(0, CurrentDir);
        Data.Fall.Music.strFilePath.insert(0, CurrentDir);
        Data.Bomb.Music.strFilePath.insert(0, CurrentDir);

        //�x�s�����T;//
        Data.strJsonTag.append(itCharacter->name.GetString());
        Scene->SetCharacter(Data);
    }

    return true;
}

FontData* oConfig::ParseArrayFont(rapidjson::Value& Array, bool isUseDefaultDir )
{
    //�榡Sample: "FontTTF1" : [ "arial.ttf", true, 0, 0, 2],
    if (Array.Size() != 5)
    {
        SetErrorInform("Parse Array Font size not 5");
        return NULL;
    }

    if (!(Array[0].IsString()
        && Array[1].IsBool()
        && Array[2].IsInt()
        && Array[3].IsInt()
        && Array[4].IsInt()))
    {
        //��ܸ�Ʈ榡���~;//
        SetErrorInform("Parse Array Font format wrong");
        return NULL;
    }

    FontData* Data = new FontData;

    if (isUseDefaultDir)
        Data->strFilePath.append(m_strDirFont);
    Data->strFilePath.append(Array[0].GetString());
    Data->isTTf  = Array[1].GetBool();
    Data->nWidth = Array[2].GetInt();
    Data->nHigh  = Array[3].GetInt();
    Data->nAlignment = Array[4].GetInt();

    return Data;
}

MusicData* oConfig::ParseArrayMusic( rapidjson::Value& Array, bool isUseDefaultDir)
{
    //�榡Sample: "BgLogo" : [ "", false, true ],
    if (Array.Size() != 3)
    {
        SetErrorInform("Parse Array Music size not 4");
        return NULL;
    }

    if (!( Array[0].IsString()
        && Array[1].IsBool()
        && Array[2].IsBool()))
    {
        //��ܸ�Ʈ榡���~;//
        SetErrorInform("Parse Array Music format wrong");
        return NULL;
    }

    MusicData* Data = new MusicData;

    if (isUseDefaultDir)
        Data->strFilePath.append(m_strDirMusic);
    Data->strFilePath.append(Array[0].GetString());
    Data->isLoop = Array[1].GetBool();
    Data->isMusic = Array[2].GetBool();
    
    return Data;
}

ImageData* oConfig::ParseArrayImage( rapidjson::Value& Array, bool isUseDefaultDir)
{
    //�榡Sample: "Logo":[ "LOGO.png", 1, 1, ["", 0, false, false ] ]
    if (Array.Size() != 4)
    {
        SetErrorInform("Parse Array Image size not 4");
        return NULL;
    }

    if (!(Array[0].IsString()
        && Array[1].IsInt()
        && Array[2].IsDouble()
        && Array[3].IsArray()))
    {
        SetErrorInform("Parse Array Image format wrong");
        return NULL;
    }

    //���ѪR���ָ��;//
    MusicData* ptrMusicData = ParseArrayMusic(Array[3], isUseDefaultDir );
    if (ptrMusicData == NULL)
    {
        SetErrorInform("Parse Array Image music wrong");
        return NULL;
    }
        
    //���o��Ƹ��|�μv��ƥ�;//
    ImageData *ptrImageData = new ImageData;
    if (isUseDefaultDir)
        ptrImageData->strFilePath.append(m_strDirImage);
    ptrImageData->strFilePath.append(Array[0].GetString());
    ptrImageData->nAniNum = Array[1].GetInt();
    ptrImageData->dAniSpeed = Array[2].GetDouble();
    ptrImageData->Music = *ptrMusicData;

    delete ptrMusicData;
    ptrMusicData = NULL;

    return ptrImageData;
}

string oConfig::GetErrorInfom()
{
    return m_strErrorInfor;
}

void oConfig::SetErrorInform(string strError, bool NewLine )
{
    if (NewLine)
        m_strErrorInfor.append("\\n");

    m_strErrorInfor.append("[ JSON ]");
    m_strErrorInfor.append(strError);

    cocos2d::log("[ oConfig ] %s", strError.c_str() );
}

void oConfig::SetErrorInform(string strError, string strMember, bool NewLine )
{
    if (NewLine)
        m_strErrorInfor.append("\\n");

    m_strErrorInfor.append("[ JSON ]");
    m_strErrorInfor.append(strError);
    m_strErrorInfor.append(strMember);

    cocos2d::log("[ oConfig ] %s %s ", strError.c_str(), strMember.c_str());
}


