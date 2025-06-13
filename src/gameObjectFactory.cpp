// TODO implementation
#include "gameObjectFactory.h"
#include "tank.h"
#include "iconFactory.h"
GameObject* gameObjectFactory::create(int E1, int E2, status which){
		Icon ic = iconFactory::createIcon(which);
		struct Vec2 v;
		v.e1 = E1;
		v.e2 = E2;
        GameObject* NEW = new GameObject(v, ic);
        return NEW;
}

GameObject* gameObjectFactory::createRPS(int E1, int E2, status type){
	Icon ic = iconFactory::createRPSIcon(type);
	struct Vec2 v;
	v.e1 = E1;
	v.e2 = E2;
	GameObject* NEW = new GameObject(v, ic);
	return NEW;
}

GameObject* gameObjectFactory::createBullet(int  E1, int E2, std::vector<BulletType> type){
	Icon ic = iconFactory::createBulletIcon(type);
	struct Vec2 v;
	v.e1 = E1;
	v.e2 = E2;
	GameObject* NEW = new GameObject(v, ic);
	return NEW;
}
