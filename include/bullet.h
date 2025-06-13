#ifndef BULLET_H
#define BULLET_H


#include "gameObject.h"
#include "collider.h"
#include "environment.h"


enum BulletType{NONE, SPLIT, SPLITX, BURN, REBOUND};
enum Direction {N, U, D, L, R, UL, UR, DL, DR};

class Bullet: public GameObject, public ICollider{
	friend class Controller;
	public:
		Bullet(GameObject& obj, Direction dir, std::vector<std::vector<BulletType>> type, int layer);
		void update() override{
			if(_type[_layer][2] == REBOUND){
				if(_pos.e1 == 0 && _pos.e2 == 0){
					_dir = DR;
					_countR += 2;
				}else if(_pos.e1 == 0 && _pos.e2 == GAME_WINDOW_HEIGHT - 1){
					_dir = UR;
					_countR += 2;
				}else if(_pos.e1 == GAME_WINDOW_WIDTH - 1 && _pos.e2 == 0){
					_dir = DL;
					_countR += 2;
				}else if(_pos.e1 == GAME_WINDOW_WIDTH - 1 && _pos.e2 == GAME_WINDOW_HEIGHT - 1){
					_dir = UL;
					_countR += 2;
				}else if(_pos.e1 == 0 && _dir == L){
					_dir = R;
					_countR++;
				}else if(_pos.e1 == GAME_WINDOW_WIDTH - 1 && _dir == R){
					_dir = L;
					_countR++;
				}else if(_pos.e1 == 0 && _dir == UL){
					_dir = UR;
					_countR++;
				}else if(_pos.e1 == 0 && _dir == DL){
					_dir = DR;
					_countR++;
				}else if(_pos.e1 == GAME_WINDOW_WIDTH - 1 && _dir == UR){
					_dir = UL;
					_countR++;
				}else if(_pos.e1 == GAME_WINDOW_WIDTH - 1 && _dir == DR){
					_dir = DL;
					_countR++;
				}else if(_pos.e2 == 0 && _dir == U){
                    _dir = D;
                    _countR++;
                }else if(_pos.e2 == GAME_WINDOW_HEIGHT - 1 && _dir == D){
                    _dir = U;
                    _countR++;
                }else if(_pos.e2 == 0 && _dir == UL){
                    _dir = DL;
                    _countR++;
                }else if(_pos.e2 == 0 && _dir == UR){
                    _dir = DR;
                    _countR++;
                }else if(_pos.e2 == GAME_WINDOW_HEIGHT - 1 && _dir == DR){
                    _dir = UR;
                    _countR++;
                }else if(_pos.e2 == GAME_WINDOW_HEIGHT - 1 && _dir == DL){
                    _dir = UL;
                    _countR++;
                }
			}
			switch(_dir){
				case U:
					_pos.e2--;
					if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX) _countS ++;
					break;
				case D:
					_pos.e2++;
					if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX) _countS ++;
					break;
				case L:
					_pos.e1--;
					if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX) _countS ++;
					break;
					case R:
					_pos.e1++;
					if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX) _countS ++;
					break;
				case UR:
                    _pos.e2--;
					_pos.e1++;
                    if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX) _countS ++;
                    break;
                case DR:
                    _pos.e2++;
					_pos.e1++;
                    if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX) _countS ++;
                    break;
                case UL:
                    _pos.e1--;
					_pos.e2--;
                    if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX) _countS ++;
                    break;
                case DL:
                    _pos.e1--;
					_pos.e2++;
                    if(_type[_layer][0] == SPLIT || _type[_layer][1] == SPLITX) _countS ++;
                    break;
			}
		}
		bool intersect(ICollider *other) override{}
		void onCollision(ICollider *other) override{}
		bool specialCheck();
		BulletType getType(int IDX);
	private:
		Direction _dir;
		std::vector<std::vector<BulletType>> _type;
		int _damage;
		int _countR;
		int _countS;
		int _layer;
		bool _del;
};
#endif
