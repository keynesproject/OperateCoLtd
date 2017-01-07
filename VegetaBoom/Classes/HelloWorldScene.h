#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	
	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

    void onMouseDown(Event *event);

private:
    Sprite *m_sLogo;
    Label  *m_lHelloWorld;
};

#endif // __HELLOWORLD_SCENE_H__
