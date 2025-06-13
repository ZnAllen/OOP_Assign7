// TODO 
// Finish the declaration of GameObjectFactory that
// 1. offers some methods to create a GameObject.
// 2. encapsulates the constructor from the caller. 


#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "iconFactory.h"
#include "icon.h"
#include "gameObject.h"
#include "bullet.h"

class gameObjectFactory{
	public:
		static GameObject* create(int E1, int E2, status which);
		static GameObject* createRPS(int E1, int E2, status type);
		static GameObject* createBullet(int E1, int E2, std::vector<BulletType> type);
};

#endif
