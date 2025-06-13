#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <termios.h>

#include "environment.h"
#include "controller.h"
#include "gameObjectFactory.h"
#include "tankFactory.h"
#include "tank.h"
#include "bulletFactory.h"
#include "bullet.h"
#include <algorithm>

Controller::Controller(View& view) : _view(view){
	_objs.push_back(TankFactory::create_RPS(1, 1, me));
	_objs.push_back(TankFactory::create_RPS(GAME_WINDOW_WIDTH - 3, GAME_WINDOW_HEIGHT - 3, object));
}

void Controller::run() {
	// initial setup
    std::cin.tie(0);
    std::ios::sync_with_stdio(0);
    configure_terminal();

    // init state
    int input = -1;
    clock_t start, end;
	const int MAXNUM = 15;

    // Main loop
    while (true) {
        start = clock();
		if(dynamic_cast<Tank*>(_objs[0]) -> _hp <= 0 && dynamic_cast<Tank*>(_objs[1]) -> _hp <= 0){
            std::cout << " 平手 \n";
            break;
        }else if(dynamic_cast<Tank*>(_objs[0]) -> _hp <= 0){
            std::cout << " 2P 獲勝 \n";
            break;
        }else if(dynamic_cast<Tank*>(_objs[1]) -> _hp <= 0){
            std::cout << " 1P 獲勝 \n";
            break;
        }
		for(GameObject* obj : _objs){
            for(auto it = _bullets.begin(); it != _bullets.end();){
                if(dynamic_cast<Tank*> (obj) -> intersect(dynamic_cast<ICollider*>(*it))){
                    dynamic_cast<Tank*> (obj) -> onCollision(dynamic_cast<ICollider*>(*it));
                	it = _bullets.erase(it);
				}else{
					++it;
				}
            }
        }
		for(auto it = _bullets.begin(); it != _bullets.end(); it++){
			for(auto It = _bullets.begin(); It != _bullets.end(); It++){
				if((*it) != (*It)){	
					if((*it) -> _pos.e1 == (*It) -> _pos.e1 && (*it) -> _pos.e2 == (*It) -> _pos.e2){
						dynamic_cast<Bullet*> (*it) -> _del = true;
						dynamic_cast<Bullet*> (*It) -> _del = true;
					}
				}
			}
		}
		_bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),[](GameObject* b){
							return dynamic_cast<Bullet*> (b) -> _del;
                     }), _bullets.end());
		std::vector<GameObject*> temp;
		for(auto it = _bullets.begin(); it != _bullets.end();){
			if(dynamic_cast<Bullet*>(*it) -> specialCheck()){
				if(dynamic_cast<Bullet*>(*it) -> _type[dynamic_cast<Bullet*>(*it) -> _layer][0] == SPLIT){
					int x = dynamic_cast<Bullet*>(*it) -> _pos.e1;
					int y = dynamic_cast<Bullet*>(*it) -> _pos.e2;
					temp.push_back(BulletFactory::createBullet(x, y,U, dynamic_cast<Bullet*>(*it) -> _type, dynamic_cast<Bullet*>(*it) -> _layer + 1));
					temp.push_back(BulletFactory::createBullet(x, y,D, dynamic_cast<Bullet*>(*it) -> _type, dynamic_cast<Bullet*>(*it) -> _layer + 1));
					temp.push_back(BulletFactory::createBullet(x, y,R, dynamic_cast<Bullet*>(*it) -> _type, dynamic_cast<Bullet*>(*it) -> _layer + 1));
					temp.push_back(BulletFactory::createBullet(x, y,L, dynamic_cast<Bullet*>(*it) -> _type, dynamic_cast<Bullet*>(*it) -> _layer + 1));
				}
				if(dynamic_cast<Bullet*>(*it) -> _type[dynamic_cast<Bullet*>(*it) -> _layer][1] == SPLITX){
                    int x = dynamic_cast<Bullet*>(*it) -> _pos.e1;
                    int y = dynamic_cast<Bullet*>(*it) -> _pos.e2;
                    temp.push_back(BulletFactory::createBullet(x, y,UR, dynamic_cast<Bullet*>(*it) -> _type, dynamic_cast<Bullet*>(*it) -> _layer + 1));
                    temp.push_back(BulletFactory::createBullet(x, y,DR, dynamic_cast<Bullet*>(*it) -> _type, dynamic_cast<Bullet*>(*it) -> _layer + 1));
                    temp.push_back(BulletFactory::createBullet(x, y,UL, dynamic_cast<Bullet*>(*it) -> _type, dynamic_cast<Bullet*>(*it) -> _layer + 1));
                    temp.push_back(BulletFactory::createBullet(x, y,DL, dynamic_cast<Bullet*>(*it) -> _type, dynamic_cast<Bullet*>(*it) -> _layer + 1));
				}
				it = _bullets.erase(it);
			}else ++it;
		}
		for(auto T : temp) _bullets.push_back(T);
		temp.clear();
        // game loop goes here
        input = read_input();
        // ESC to exit program
        if(input==27)break;
		_view.resetLatest();
        this->handleInput(input);
        for(GameObject* obj : _objs) 
        {
			
            obj->update();

            _view.updateGameObject(obj);
        }
		for(GameObject* bullet : _bullets){
			bullet -> update();
			_view.updateGameObject(bullet);
		}
		_bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),[](const GameObject* b){
						if(b -> _pos.e1 < 0) return true;
						else if(b -> _pos.e1 >= GAME_WINDOW_WIDTH) return true;
						else if(b -> _pos.e2 < 0) return true;
						else if(b -> _pos.e2 >= GAME_WINDOW_HEIGHT) return true;
						else return false;
					}), _bullets.end());
		_view.render();
        end = clock();
        // frame rate normalization
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_taken > SPF) continue;
        int frameDelay = int((SPF - time_taken) * 1000); // 0.1 seconds
        if(frameDelay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay)); // frame delay
    }

}



void Controller::handleInput(int keyInput){
    // If there is no input, do nothing.
    if(keyInput==-1)return;
    GameObject* myCharacter;
	if(keyInput == 'a') {
		dynamic_cast<Tank*>(_objs[0]) -> _dir = L;
		dynamic_cast<Tank*>(_objs[0]) -> _mov = true;
	
	}else if(keyInput == 'w') {
		dynamic_cast<Tank*>(_objs[0]) -> _dir = U;
		dynamic_cast<Tank*>(_objs[0]) -> _mov = true;

	}else if(keyInput == 's') {
		dynamic_cast<Tank*>(_objs[0]) -> _dir = D;
		dynamic_cast<Tank*>(_objs[0]) -> _mov = true;
	}else if(keyInput == 'd')	 {
		dynamic_cast<Tank*>(_objs[0]) -> _dir = R;
		dynamic_cast<Tank*>(_objs[0]) -> _mov = true;
	}else if(keyInput == ' '){
		switch(dynamic_cast<Tank*>(_objs[0]) -> _dir){
			case U:
				_bullets.push_back(BulletFactory::createBullet(_objs[0] -> _pos.e1 + 1, _objs[0] -> _pos.e2,U, dynamic_cast<Tank*>(_objs[0]) -> _type, 0));
				break;
			case D:
				_bullets.push_back(BulletFactory::createBullet(_objs[0] -> _pos.e1 + 1, _objs[0] -> _pos.e2 + 2,D, dynamic_cast<Tank*>(_objs[0]) -> _type, 0));
				break;
			case R:
				_bullets.push_back(BulletFactory::createBullet(_objs[0] -> _pos.e1 + 2, _objs[0] -> _pos.e2 + 1,R, dynamic_cast<Tank*>(_objs[0]) -> _type, 0));
				break;
			case L:
				_bullets.push_back(BulletFactory::createBullet(_objs[0] -> _pos.e1, _objs[0] -> _pos.e2 + 1,L, dynamic_cast<Tank*>(_objs[0]) -> _type, 0));
				break;
		}
	}else if(keyInput == 'v'){
		if(dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][0] != SPLIT) dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][0] = SPLIT;
		else dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][0] = NONE;
	}else if(keyInput == 'b'){
		if(dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][1] != SPLITX) dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][1] = SPLITX;
        else dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][1] = NONE;
	}else if(keyInput == 'n'){
		if(dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][3] != BURN) dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][3] = BURN;
        else dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][3] = NONE;
	}else if(keyInput == 'm'){
		if(dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][2] != REBOUND) dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][2] = REBOUND;
        else dynamic_cast<Tank*> (_objs[0]) -> _type[dynamic_cast<Tank*>(_objs[0]) -> _layer][2] = NONE;
	}else if(keyInput == 'j'){
		dynamic_cast<Tank*> (_objs[0]) -> _layer = 0;
	}else if(keyInput == 'k'){
		dynamic_cast<Tank*> (_objs[0]) -> _layer = 1;
	}else if(keyInput == 'l'){
		dynamic_cast<Tank*> (_objs[0]) -> _layer = 2;
	}else if(keyInput == 68) {
        dynamic_cast<Tank*>(_objs[1]) -> _dir = L;
        dynamic_cast<Tank*>(_objs[1]) -> _mov = true;
    }else if(keyInput == 65) {
        dynamic_cast<Tank*>(_objs[1]) -> _dir = U;
        dynamic_cast<Tank*>(_objs[1]) -> _mov = true;
    }else if(keyInput == 66) {
        dynamic_cast<Tank*>(_objs[1]) -> _dir = D;
        dynamic_cast<Tank*>(_objs[1]) -> _mov = true;
    }else if(keyInput == 67)     {
        dynamic_cast<Tank*>(_objs[1]) -> _dir = R;
        dynamic_cast<Tank*>(_objs[1]) -> _mov = true;
    }
    if(keyInput == '0'){
        switch(dynamic_cast<Tank*>(_objs[1]) -> _dir){
            case U:
                _bullets.push_back(BulletFactory::createBullet(_objs[1] -> _pos.e1 + 1, _objs[1] -> _pos.e2,U, dynamic_cast<Tank*>(_objs[1]) -> _type, 0));
                break;
            case D:
                _bullets.push_back(BulletFactory::createBullet(_objs[1] -> _pos.e1 + 1, _objs[1] -> _pos.e2 + 2,D, dynamic_cast<Tank*>(_objs[1]) -> _type, 0));
                break;
            case R:
                _bullets.push_back(BulletFactory::createBullet(_objs[1] -> _pos.e1 + 2, _objs[1] -> _pos.e2 + 1,R, dynamic_cast<Tank*>(_objs[1]) -> _type, 0));
                break;
            case L:
                _bullets.push_back(BulletFactory::createBullet(_objs[1] -> _pos.e1, _objs[1] -> _pos.e2 + 1,L, dynamic_cast<Tank*>(_objs[1]) -> _type, 0));
                break;
        }
    }else if(keyInput == '1'){
		dynamic_cast<Tank*> (_objs[1]) -> _layer = 0;
	}else if(keyInput == '2'){
		dynamic_cast<Tank*> (_objs[1]) -> _layer = 1;
	}else if(keyInput == '3'){
		dynamic_cast<Tank*> (_objs[1]) -> _layer = 2;
	}else if(keyInput == '4'){
        if(dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][0] != SPLIT) dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][0] = SPLIT;
        else dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][0] = NONE;
    }else if(keyInput == '5'){
        if(dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][1] != SPLITX) dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][1] = SPLITX;
        else dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][1] = NONE;
    }else if(keyInput == '6'){
        if(dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][3] != BURN) dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][3] = BURN;
        else dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][3] = NONE;
    }else if(keyInput == '7'){
        if(dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][2] != REBOUND) dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][2] = REBOUND;
        else dynamic_cast<Tank*> (_objs[1]) -> _type[dynamic_cast<Tank*>(_objs[1]) -> _layer][2] = NONE;
	}
	// TODO 
    // handle key events.
}

void Controller::update(){
}
void reset_terminal() {
    printf("\e[m"); // reset color changes
    printf("\e[?25h"); // show cursor
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// terminal initial configuration setup
void configure_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
	new_termios = old_termios; // save it to be able to reset on exit
    
    new_termios.c_lflag &= ~(ICANON | ECHO); // turn off echo + non-canonical mode
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    printf("\e[?25l"); // hide cursor
    std::atexit(reset_terminal);
}


int read_input() {
    fflush(stdout);
   	char buf[4096]; // maximum input buffer
	int n = read(STDIN_FILENO, buf, sizeof(buf));
    return n > 0 ? buf[n - 1] : -1;
}

