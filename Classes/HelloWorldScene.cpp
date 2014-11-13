#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width,
		origin.y + visibleSize.height - closeItem->getContentSize().height));

	auto aboutItem = MenuItemImage::create(
		"zhizuo1_cover.png",
		"zhizuo2_cover.png",
		CC_CALLBACK_1(HelloWorld::menuAboutCallback, this));
    
	aboutItem->setPosition(Vec2(origin.x + visibleSize.width - aboutItem->getContentSize().width,
		origin.y + aboutItem->getContentSize().height));

	auto startItem = MenuItemImage::create(
		"StartGameNormal.png", 
		"StartGameSelected.png", 
		CC_CALLBACK_1(HelloWorld::menuStartCallback, this));
	startItem->setScale(0.8);

	startItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));


    // create menu, it's an autorelease object
    menu = Menu::create(closeItem,aboutItem,startItem, NULL);
    menu->setPosition(Vec2(0 , 0));
    this->addChild(menu, 1);


	backButton = MenuItemImage::create(
		"zhuye1_cover.png",
		"zhuye2_cover.png",
		CC_CALLBACK_1(HelloWorld::menuBackCallback, this));

	backButton->setPosition(Vec2(origin.x + visibleSize.width + backButton->getContentSize().width,
		origin.y + backButton->getContentSize().height));

	backButtonPoint = backButton->getPosition();

	auto menuBack = Menu::create(backButton, NULL);
	menuBack->setPosition(Vec2(0, 0));
	this->addChild(menuBack, 1);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keycode , Event* event)
	{
		if (keycode == EventKeyboard::KeyCode::KEY_BACK ||
			keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
		{
			MessageBox("Are you sure close it?", "Alert");
			Director::getInstance()->end();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener , this);
    
	aboutLabel = LabelTTF::create(
		"Developer : seabrea",
		"Arial", 30
		);
	this->addChild(aboutLabel);
	aboutLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
								 origin.y + visibleSize.height / 2
								 ));
	aboutLabel->setVisible(false);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::menuAboutCallback(Ref* pSender)
{
	menu->setVisible(false);
	aboutLabel->setVisible(true);
	auto mt = MoveTo::create(0.2, Vec2(origin.x + visibleSize.width - backButton->getContentSize().width,
		origin.y + backButton->getContentSize().height));
	backButton->runAction(Repeat::create(mt , 1));
}

void HelloWorld::menuStartCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1, GameScene::createScene()));
}

void HelloWorld::menuBackCallback(cocos2d::Ref* pSender)
{
		menu->setVisible(true);
		aboutLabel->setVisible(false);
		auto mt = MoveTo::create(0.2, backButtonPoint);
		backButton->runAction(Repeat::create(mt, 1));
}
