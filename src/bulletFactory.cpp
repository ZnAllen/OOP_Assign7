#include "bulletFactory.h"
#include "gameObjectFactory.h"
#include "gameObject.h"
#include "tankFactory.h"

Bullet* BulletFactory::createBullet(int E1, int E2, Direction dir,  std::vector<std::vector<BulletType>> type, int layer){
	if(layer > 3) layer = 3;	
	Bullet* re = new Bullet(*gameObjectFactory::createBullet(E1, E2, type[layer]), dir, type, layer);	
	return re;
}
