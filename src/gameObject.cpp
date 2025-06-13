// TODO implementation

#include "gameObject.h"

GameObject::GameObject(Position P, Icon I)
	:_pos(P), _icon(I){}

Position GameObject::getPosition() const{
	return _pos;
}

Icon GameObject::getIcon() const{
	return _icon;
}
