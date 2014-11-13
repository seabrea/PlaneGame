#include "GameScene.h"

static int blood = 0;

Scene* GameScene::createScene()
{
	Scene* scene = Scene::create();
	GameScene* layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	score = 0;

	visible = Director::getInstance()->getVisibleSize();

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm.OGG");

	return true;
}

void GameScene::onExit()
{
	Layer::onExit();
}
void GameScene::onEnter()
{
	Layer::onEnter();

	background();

	uiLayer();

	playerCreate();

	this->schedule(schedule_selector(GameScene::moveBg), 0.05f);
	this->scheduleOnce(schedule_selector(GameScene::delayHandler), 3.0f);
	this->schedule(schedule_selector(GameScene::countDownTimer), 1.0f);

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
void GameScene::delayHandler(float t)
{
	this->scheduleOnce(schedule_selector(GameScene::delayMissiles), 10.0f);
	this->scheduleOnce(schedule_selector(GameScene::delayhint), 7.0f);
	
	this->schedule(schedule_selector(GameScene::newBullet), 0.5f);
	this->schedule(schedule_selector(GameScene::moveBullet), 0.01f);

	this->schedule(schedule_selector(GameScene::newEnemy), 0.5f);
	this->schedule(schedule_selector(GameScene::moveEnemy), 0.01f);

	this->schedule(schedule_selector(GameScene::update), 0.01f);

	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm.OGG");
}
void GameScene::countDownTimer(float t)
{
	auto countDowmLabel = static_cast<LabelTTF*>(this->getChildByTag(4));
	if (countDowmLabel->isVisible())
	{
		countDowmLabel->setString(String::createWithFormat("%d", countdown)->_string);
		--countdown;
		if (countdown == -1)
		{
			countDowmLabel->setVisible(false);
		}
	}
	else
	{
		this->unschedule(schedule_selector(GameScene::delayHandler));
	}
}
void GameScene::delayhint(float t)
{
	auto hintLabel = LabelTTF::create("Be careful!", "Marker Felt", 60);
	this->addChild(hintLabel);
	hintLabel->setTag(6);
	hintLabel->setPosition(Vec2(visible.width + hintLabel->getContentSize().width, visible.height / 2));
	hintLabel->runAction(Repeat::create(MoveTo::create(5.0f, Vec2((0 - hintLabel->getContentSize().width), visible.height / 2)), 1));
	
}
void GameScene::delayMissiles(float t)
{
	auto lablel = this->getChildByTag(6);
	lablel->setVisible(false);
	this->schedule(schedule_selector(GameScene::newMissiles), 0.5f);
	this->schedule(schedule_selector(GameScene::moveMissiles), 0.01f);
}

void GameScene::background()
{
	auto bg1 = Sprite::create("bg.jpg");
	bg1->setAnchorPoint(Point::ZERO);
	bg1->setPosition(Vec2(0, 0));
	bg1->setTag(0);
	intermediatePoint = Vec2(0, bg1->getContentSize().height);
	auto bg2 = Sprite::create("bg.jpg");
	bg2->setPosition(intermediatePoint);
	bg2->setTag(1);
	this->addChild(bg1 , 0);
	this->addChild(bg2 , 0);

}

void GameScene::uiLayer()
{
	auto soundChannel = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GameScene::soundButtonHandler , this),
		MenuItemImage::create("pauseSoundOn.png", "pauseSoundOn.png"),
		MenuItemImage::create("pauseSoundOff.png", "pauseSoundOff.png"),
		NULL
		);

	soundChannel->setAnchorPoint(Point::ZERO);
	soundChannel->setPosition(Vec2(0, visible.height - soundChannel->getContentSize().height));

	auto pauseChannel = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GameScene::pauseButtonHandler, this),
		MenuItemImage::create("pause.png", "pause.png"),
		MenuItemImage::create("pause_resume.png", "pause_resume.png"),
		NULL
		);

	pauseChannel->setAnchorPoint(Point::ZERO);
	pauseChannel->setPosition(Vec2(visible.width - pauseChannel->getContentSize().width, visible.height - pauseChannel->getContentSize().height * 1.3));
	auto menu = Menu::create(soundChannel, pauseChannel , NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu , 2);

	auto scoreLabel = LabelTTF::create("score : 0" , "Marker Felt" , 42);
	this->addChild(scoreLabel, 2);
	scoreLabel->setTag(3);
	scoreLabel->setPosition(Vec2(visible.width/2,visible.height - 100));

	countdown = 2;
	auto countDowmLabel = LabelTTF::create("3", "Marker Felt", 60);
	this->addChild(countDowmLabel, 2);
	countDowmLabel->setTag(4);
	countDowmLabel->setPosition(Vec2(visible.width / 2, visible.height / 2));
}

void GameScene::moveBg(float t)
{
	auto bg1 = this->getChildByTag(0);
	auto bg2 = this->getChildByTag(1);
	bg1->setAnchorPoint(Point::ZERO);
	bg2->setAnchorPoint(Point::ZERO);
	bg1->setPosition(Vec2(0, bg1->getPositionY() - 1));
	bg2->setPosition(Vec2(0, bg2->getPositionY() - 1));
	if (bg1->getPositionY() < -1600)
	{
		bg1->setPosition(Vec2(0 , intermediatePoint.y - 1));
	}
	else if (bg2->getPositionY() < -1600)
	{
		bg2->setPosition(Vec2(0, intermediatePoint.y - 1));
	}
}

void GameScene::soundButtonHandler(Ref* r)
{
	if (SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() == 0)
	{
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
	}
	else 
	{
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
	}
}

void GameScene::pauseButtonHandler(Ref* r)
{
	if (Director::getInstance()->isPaused())
	{
		Director::getInstance()->resume();
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	else
	{
		Director::getInstance()->pause();
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}
}

void GameScene::playerCreate()
{
	player = Sprite::create("Plane.png");
	this->addChild(player , 1);
	player->setScale(1.2);

	player->setPosition(Vec2(visible.width / 2, 0 + visible.height / 3));

	auto playerListener = EventListenerTouchOneByOne::create();
	playerListener->onTouchBegan = [this](Touch* t, Event* e)
	{
		auto oldPoint = t->getLocation();
		distance.x = oldPoint.x - player->getPosition().x;
		distance.y = oldPoint.y - player->getPosition().y;
		return true;
	};

	playerListener->onTouchMoved = [this](Touch* t , Event* e)
	{
		auto nowPoint = t->getLocation();
		int _x = nowPoint.x - distance.x;
		int _y = nowPoint.y - distance.y;
		Point point = Vec2(_x ,_y);
		player->setPosition(point);
		if (player->getPositionX() <= player->getContentSize().width/2)
		{
			player->setPositionX(player->getContentSize().width / 2);
		}
		else if (player->getPositionX() >= (visible.width - player->getContentSize().width/2))
		{
			player->setPositionX(visible.width - player->getContentSize().width/2);
		}
		else if (player->getPositionY() <= player->getContentSize().height / 2)
		{
			player->setPositionY(player->getContentSize().height / 2);
		}
		else if (player->getPositionY() >= (visible.height - player->getContentSize().height*2))
		{
			player->setPositionY(visible.height - player->getContentSize().height*2);
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(playerListener , this);
}

void GameScene::moveBullet(float t)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		auto bulletItem = bullets.at(i);
		bulletItem->setPositionY(bulletItem->getPositionY() + 10);
		if (bulletItem->getPositionY() > 800)
		{
			bulletItem->removeFromParent();
			bullets.eraseObject(bulletItem);
		}
	}

}

void GameScene::newBullet(float t)
{
	auto bullet = Sprite::create("bullet/fireball.png");
	bullet->setPosition(Vec2(player->getPositionX(),player->getPositionY()+player->getContentSize().height/2));
	bullets.pushBack(bullet);
	this->addChild(bullet , 1);
}

void GameScene::moveEnemy(float t)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		auto enemyItem = enemies.at(i);
		enemyItem->setPositionY(enemyItem->getPositionY() - 10);
		enemyItem->setPositionX(enemyItem->getPositionX() + sin(enemyItem->getPositionY()/100)*5);
		if (enemyItem->getPositionY() > 800)
		{
			enemyItem->removeFromParent();
			enemies.eraseObject(enemyItem);
		}
	}
}

void GameScene::newEnemy(float t)
{
	Sprite* enemy;
	int chooseEnemy = static_cast<int>(rand() % 100);
	if ( chooseEnemy < 35)
	{
		enemy = Sprite::create("enemy/enemy_1.png");
		enemy->setTag(10);
	}
	else if (chooseEnemy >= 35 && chooseEnemy < 70)
	{
		enemy = Sprite::create("enemy/enemy_3.png");
		enemy->setTag(10);
	}
	else if (chooseEnemy >= 70 && chooseEnemy < 85)
	{
		enemy = Sprite::create("enemy/enemy_2.png");
		enemy->setTag(25);
	}
	else if (chooseEnemy >= 85 && chooseEnemy <= 100)
	{
		enemy = Sprite::create("enemy/enemy_4.png");
		enemy->setTag(25);
	}
	else
	{
		return;
	}

	int beginEnemy = static_cast<int>(rand() % 450 + 20);
	enemy->setPosition(Vec2(beginEnemy, visible.height));
	enemies.pushBack(enemy);
	this->addChild(enemy, 1);
}

void GameScene::moveMissiles(float t)
{
	Sprite* missileF;
	Sprite* missileR;
	for (int i = 0; i < missilesFromLeft.size(); i++)
	{
		missileF = missilesFromLeft.at(i);
		missileF->setPositionX(missileF->getPositionX() + 8);
		if (missileF->getPositionX() >(800 + missileF->getContentSize().width / 2))
		{
			missileF->removeFromParent();
			missilesFromLeft.eraseObject(missileF);
			//log("removemissilef");
		}
	}
	for (int i = 0; i < missilesFromRight.size(); i++)
	{
		missileR = missilesFromRight.at(i);
		missileR->setPositionX(missileR->getPositionX() - 8);
		if (missileR->getPositionX() < (0-missileR->getContentSize().width/2))
		{
			missileR->removeFromParent();
			missilesFromRight.eraseObject(missileR);
			//log("removemissiler");
		}
	}
	//log("move missiles");
}

void GameScene::newMissiles(float t)
{
	int choosemissileL = static_cast<int>(rand() % 100);
	int choosemissileR = static_cast<int>(rand() % 100);

	Sprite* missileL;
	Sprite* missileR;

	if (choosemissileL < 45)
	{
		missileL = Sprite::create("bullet/fllowBullet.png");
	}
	else if (choosemissileL >= 45 && choosemissileL < 75)
	{
		missileL = Sprite::create("bullet/monsterBullet1.png");
	}
	else if (choosemissileL >= 75 && choosemissileL <= 100)
	{
		missileL = Sprite::create("bullet/disperseBullet.png");
	}
	missileL->setRotation(90);
	missilesFromLeft.pushBack(missileL);

	if (choosemissileL < 45)
	{
		missileR = Sprite::create("bullet/fllowBullet.png");
	}
	else if (choosemissileL >= 45 && choosemissileL < 75)
	{
		missileR = Sprite::create("bullet/greenBall.png");
	}
	else if (choosemissileL >= 75 && choosemissileL <= 100)
	{
		missileR = Sprite::create("bullet/blueBall2.png");
	}
	missileR->setRotation(-90);
	missilesFromRight.pushBack(missileR);

	int leftBeginMissile = static_cast<int>(rand() % 700 + 10);
	missileL->setPosition(Vec2(0,leftBeginMissile));
	int rightBeginMissile = static_cast<int>(rand() % 700 + 10);
	missileR->setPosition(Vec2(visible.width , rightBeginMissile));
	this->addChild(missileL , 1);
	this->addChild(missileR , 1);
}

void GameScene::update(float t)
{
	Sprite* enemy;
	Sprite* bullet;
	for (int i = 0; i < enemies.size(); i++)
	{
		enemy = enemies.at(i);
		for (int j = 0; j < bullets.size(); j++)
		{
			bullet = bullets.at(j);
			if (enemy->boundingBox().intersectsRect(bullet->boundingBox()))
			{
				bullet->removeFromParent();
				bullets.eraseObject(bullet);

				if (enemy->getTag() == 10)
				{
					enemy->removeFromParent();
					enemies.eraseObject(enemy);
					score += 10;
					auto label = static_cast<LabelTTF*>(this->getChildByTag(3));
					label->setString(String::createWithFormat("score : %d" , score)->_string);
				}
				else
				{
					if (blood == 0)
					{
						blood++;
					}
					else if (blood == 1)
					{
						enemy->removeFromParent();
						enemies.eraseObject(enemy);
						score += 25;
						auto label = static_cast<LabelTTF*>(this->getChildByTag(3));
						label->setString(String::createWithFormat("score : %d", score)->_string);
						blood = 0;
					}
				}

				break;
			}
		}
	}

	for (int i = 0; i < missilesFromLeft.size(); i++)
	{
		auto missile = static_cast<Sprite*>(missilesFromLeft.at(i));
		if (missile->boundingBox().intersectsRect(player->boundingBox()))
		{
			player->setVisible(false);
			missile->removeFromParent();
			missilesFromLeft.eraseObject(missile);
			overScene();
			break;
		}
	}
	for (int i = 0; i < missilesFromRight.size(); i++)
	{
		auto missile = static_cast<Sprite*>(missilesFromRight.at(i));
		if (missile->boundingBox().intersectsRect(player->boundingBox()))
		{
			player->setVisible(false);
			missile->removeFromParent();
			missilesFromLeft.eraseObject(missile);
			overScene();
			break;
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		auto enemy = enemies.at(i);	
		if (enemy->boundingBox().intersectsRect(player->boundingBox()))
		{
			player->setVisible(false);
			enemy->removeFromParent();
			enemies.eraseObject(enemy);
			overScene();
			break;
		}
	}

}

void GameScene::overScene()
{
	this->unschedule(schedule_selector(GameScene::newBullet));
	this->unschedule(schedule_selector(GameScene::newMissiles));
	this->unschedule(schedule_selector(GameScene::newEnemy));

	if (this->getChildByTag(6))
	{
		auto lablel = this->getChildByTag(6);
		lablel->setVisible(false);
	}

	if (!UserDefault::getInstance()->getIntegerForKey("score"))
	{
		UserDefault::getInstance()->setIntegerForKey("score", score);
		UserDefault::getInstance()->flush();
	}
	else
	{
		UserDefault::getInstance()->setIntegerForKey("score", score);
	}

	this->scheduleOnce(schedule_selector(GameScene::delayNextScene), 1.0f);
}
void GameScene::delayNextScene(float t)
{
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	this->unscheduleAllSelectors();
	Director::getInstance()->replaceScene(TransitionSlideInL::create(1.f , GameOverScene::createScene()));
}

