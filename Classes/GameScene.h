#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include "network\HttpClient.h"
#include "network\WebSocket.h"
#include "network\SocketIO.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace network;

class GameScene :public Layer

{
public:
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	static Scene* createScene();
	CREATE_FUNC(GameScene);
private:
	int score;
	int countdown;
	Size visible;
	Sprite* player;
	Point distance;
	Vector<Sprite*> bullets;
	Vector<Sprite*> missilesFromLeft;
	Vector<Sprite*> missilesFromRight;
	Vector<Sprite*> enemies;
	void moveBullet(float t);
	void newBullet(float t);
	void moveEnemy(float t);
	void newEnemy(float t);
	void moveMissiles(float t);
	void newMissiles(float t);
	void moveBg(float t);
	void update(float t);
	void delayHandler(float t);
	void delayMissiles(float t);
	void countDownTimer(float t);
	void delayhint(float t);
	void delayNextScene(float t);
	Point intermediatePoint;
	void background();
	void uiLayer();
	void playerCreate();
	void soundButtonHandler(Ref* r);
	void pauseButtonHandler(Ref* r);
	void overScene();

};