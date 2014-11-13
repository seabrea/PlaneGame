#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "GameScene.h"
#include "HelloWorldScene.h"


USING_NS_CC;
using namespace ui;

class GameOverScene:public Layer
{
public:
	virtual bool init();
	static Scene* createScene();
	CREATE_FUNC(GameOverScene);
private:
	void work();

	void restartHandler(Ref* f);

	void endHandler(Ref* f);

	void cutScreen();

	void cutScreenHandler(bool b ,std::string name);
};