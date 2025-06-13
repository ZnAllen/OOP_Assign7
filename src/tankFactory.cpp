#include "tankFactory.h"
#include "gameObjectFactory.h"
#include "tank.h"

Tank* TankFactory::create_RPS(int E1, int E2, status which){
	Tank* re = new Tank(*gameObjectFactory::createRPS(E1, E2, which));
	return re;
}
