#include "tank.h"
#include <iostream>

Tank::Tank(GameObject& obj)
	:GameObject(obj), _dir(U), _mov(false), _hp(100), _status(none), _Timer(0), _type(4,std::vector<BulletType>(4, NONE)), _layer(0){}

void Tank::setDirection(Direction D){ _dir = D; }

int Tank::getHP(){
	return _hp;
}

void Tank::onBurn(){
	_hp-- ;
	_Timer--;
	if(_Timer <= 0) _status = none;
}

int Tank::getLayer(){
	return _layer;
}

BulletType Tank::getType(int l, int which){
	return _type[l][which];
}

Status Tank::getStatus(){
	return _status;
}
