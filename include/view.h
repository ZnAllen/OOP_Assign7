#ifndef VIEW_H
#define VIEW_H
#include "unit.h"
#include "gameObject.h"
#include <string>
#include <vector>
#include <string>
#include <utility>

class View{
public:
    View();
    void render();
    void updateGameObject(GameObject*);
    void resetLatest();
private:
    int _termWidth;
    int _termHeight;
    std::vector<std::vector<std::string>> latest_map, last_map;
    std::vector<std::vector<Color>>       latest_fg_color, last_fg_color;
    std::vector<std::vector<Color>>       latest_bg_color, last_bg_color;
	std::vector<std::vector<std::string>> latest_P1info, last_P1info;
	std::vector<std::vector<int>>       latest_fg_color_P1info, last_fg_color_P1info;
	std::vector<std::vector<int>>       latest_bg_color_P1info, last_bg_color_P1info;
	std::vector<std::vector<std::string>> latest_P2info, last_P2info;
	std::vector<std::vector<int>>       latest_fg_color_P2info, last_fg_color_P2info;
	std::vector<std::vector<int>>       latest_bg_color_P2info, last_bg_color_P2info;
    static std::pair<int, int> get_terminal_size();
	void insertStr(std::vector<std::vector<std::string>>&, std::string what, int y, int pos);
};

#endif
