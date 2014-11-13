#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	MenuItemImage* backButton;

	Size visibleSize;

	Vec2 origin;

	Point backButtonPoint;

	LabelTTF* aboutLabel;

	Menu* menu;

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void menuAboutCallback(cocos2d::Ref* pSender);

	void menuStartCallback(cocos2d::Ref* pSender);

	void menuBackCallback(cocos2d::Ref* pSender );
};

#endif // __HELLOWORLD_SCENE_H__
