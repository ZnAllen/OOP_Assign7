#ifndef TANK_H
#define TANK_H

#include "collider.h"
#include "gameObject.h"
#include "unit.h"
#include "bullet.h"
#include "environment.h"
#include <math.h>

enum Status{none, burn};

class Tank : public GameObject, public ICollider {

	friend class Controller;
public:
  Tank(GameObject& obj);
  void update() override{
	if(_status == burn) onBurn();
	Color mycolor = _icon[1][1].color;
	if(_mov){
	    switch(_dir){
			case U:
				if(_pos.e2 > 0){
					_pos.e2 --;
					for(auto& i:_icon){
						for(auto& j:i){
							j.color = mycolor;
						}
					}
					_icon[0][0].color = NOCHANGE;
					_icon[0][2].color = NOCHANGE;
				}
				break;
			case D:
				if(_pos.e2 < GAME_WINDOW_HEIGHT - 3){
					_pos.e2 ++;
					for(auto& i:_icon){
                		for(auto& j:i){
            	        	j.color = mycolor;
        	   	    	}
    	        	}
					_icon[2][0].color = NOCHANGE;
					_icon[2][2].color = NOCHANGE;
				}
				break;
			case L:
				if(_pos.e1 > 0){
					_pos.e1 --;
					for(auto& i:_icon){
           		   		for(auto& j:i){
        	    			j.color = mycolor;
    	       		    }
	          		}
					_icon[0][0].color = NOCHANGE;
					_icon[2][0].color = NOCHANGE;
				}
			break;
			case R:
				if(_pos.e1 < GAME_WINDOW_WIDTH - 3){
					_pos.e1 ++;
					for(auto& i:_icon){
             		   for(auto& j:i){
            	  	      j.color = mycolor;
        	      	  }
    	     	   }
					_icon[0][2].color = NOCHANGE;
					_icon[2][2].color = NOCHANGE;
				}
				break;
		}
		}
	_mov = false;
  }
  void onCollision(ICollider *other) override{
		_hp -= 10;
		if(dynamic_cast<Bullet*>(other) -> getType(3) == BURN) {
			_status = burn;
			_Timer = 30;
		}
  }
  bool intersect(ICollider *other) override{
		int x = dynamic_cast<GameObject*> (other) -> getPosition().e1;
		int y = dynamic_cast<GameObject*> (other) -> getPosition().e2;
		int pos_x = x - _pos.e1;
		int pos_y = y - _pos.e2;
		if(pos_x < 3 && pos_y < 3 && pos_x >= 0 && pos_y >= 0){
			if(_icon[pos_y][pos_x].color != NOCHANGE) return true;
		}
		return false;
  }
  void setDirection(Direction);
  int getHP();
  void onBurn();
  int getLayer();
  Status getStatus();
  BulletType getType(int l, int which);

private:
  Direction _dir;
  Status _status;
  std::vector<std::vector<BulletType>> _type;
  int _layer;
  int _hp;
  int _Timer;
  bool _mov;
};
#endif
