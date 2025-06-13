#ifndef TANKFACTORY_H
#define TANKFACTORY_H

#include "tank.h"
#include "iconFactory.h"

class TankFactory{
	public:
		static Tank* create_RPS(int E1, int E2, status type); 
};

#endif
