#include "bullet.h"

Bullet::Bullet(GameObject& obj,Direction dir, std::vector<std::vector<BulletType>> type, int layer)
	:GameObject(obj), _dir(dir), _damage(10), _countS(0), _countR(0), _type(type), _del(false), _layer(layer){
	}

bool Bullet::specialCheck(){
	if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX){
		return _countS > 20;
	}else if(_type[_layer][2] == REBOUND){
		return _countR > 5;
	}
}

BulletType Bullet::getType(int IDX){
	return _type[_layer][IDX];
}
