#ifndef BULLETFACTORY_H
#define BULLETFACTORY_H

#include "bullet.h"
class BulletFactory{
	public:
		static Bullet* createBullet(int E1, int E2, Direction dir,  std::vector<std::vector<BulletType>> type, int layer);	
};


#endif
