#include "GameOverScene.h"

Scene* GameOverScene::createScene()
{

	auto scene = Scene::create();

	GameOverScene* layer = GameOverScene::create();

	scene->addChild(layer);

	return scene;
}
bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	work();

	return true;
}

void GameOverScene::work()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto girl = Sprite::create("messageGirl.png");
	this->addChild(girl, 0);
	girl->setPosition(Vec2(visibleSize.width - girl->getContentSize().width/2,girl->getContentSize().height/2));

	auto time = UserDefault::getInstance()->getIntegerForKey("score");
	auto timescore = LabelTTF::create("", "Marker Felt", 36);
	timescore->setString(String::createWithFormat("your score is:%d",time)->_string);
	timescore->setPosition(Vec2(timescore->getContentSize().width / 2, girl->getContentSize().height*2 / 3));
	this->addChild(timescore);

	auto restart = MenuItemImage::create("resultBtn2_0.png", "resultBtn2_1.png",CC_CALLBACK_1(GameOverScene::restartHandler, this));
	restart->setPosition(Vec2(restart->getContentSize().width / 2, girl->getContentSize().height / 2));

	auto end = MenuItemImage::create("back_peek0.png", "back_peek1.png",CC_CALLBACK_1(GameOverScene::endHandler, this));
	end->setPosition(Vec2(end->getContentSize().width / 2, girl->getContentSize().height * 1 / 3));

	auto menu = Menu::create(restart,end,NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	menu->setTag(10);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keycode, Event* event)
	{
		if (keycode == EventKeyboard::KeyCode::KEY_BACK ||
			keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
		{
			MessageBox("Are you sure close it?", "Alert");
			Director::getInstance()->end();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

}

void GameOverScene::restartHandler(Ref* f)
{
	auto menu = this->getChildByTag(10);
	menu->setVisible(false);
	cutScreen();
	UserDefault::getInstance()->purgeSharedUserDefault();
	Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0f, GameScene::createScene()));
}

void GameOverScene::endHandler(Ref* f)
{
	auto menu = this->getChildByTag(10);
	menu->setVisible(false);
	cutScreen();
	UserDefault::getInstance()->purgeSharedUserDefault();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HelloWorld::createScene()));
}

void GameOverScene::cutScreen()
{
	utils::captureScreen(CC_CALLBACK_2(GameOverScene::cutScreenHandler , this),"shareScreen.png");
}

void GameOverScene::cutScreenHandler(bool b, std::string name)
{

}
