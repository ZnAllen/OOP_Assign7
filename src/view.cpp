#include "view.h"
#include "environment.h"
#include "AnsiPrint.h"
#include <codecvt>      // std::wstring_convert
#include <cwchar>       // wcwidth
#include <sys/ioctl.h>   // winsize, TIOCGWINSZ
#include <unistd.h>  //  STDOUT_FILENO
#include <iostream>
#include <string>
#include <locale>
#include <utility>
#include <algorithm>
#include "tank.h"

// return the actual length in terminal of a string.
int displayWidth(const std::string& utf8) {
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    auto u32 = conv.from_bytes(utf8);

    int w = 0;
    for (char32_t ch : u32)
    w += std::max(0, ::wcwidth(static_cast<wchar_t>(ch)));
    return std::max(1, w);
}

View::View() {
    std::setlocale(LC_ALL, "");
    
    auto make_str_buf = [] {
        return std::vector<std::vector<std::string>>(
            GAME_WINDOW_HEIGHT, std::vector<std::string>(GAME_WINDOW_WIDTH, ""));
    };
    auto make_col_buf = [] {
        return std::vector<std::vector<Color>>(
            GAME_WINDOW_HEIGHT, std::vector<Color>(GAME_WINDOW_WIDTH, Color::NOCHANGE));
    };
	auto make_int_buf = [] {
        return std::vector<std::vector<int>>(
        GAME_WINDOW_HEIGHT, std::vector<int>(GAME_WINDOW_WIDTH, 0));
    };

    latest_map   = last_map   = make_str_buf();
	latest_P1info = last_P1info = make_str_buf();
	latest_P2info = last_P2info = make_str_buf();
    latest_fg_color = last_fg_color = make_col_buf();
	latest_fg_color_P1info = last_fg_color_P1info = make_int_buf();
	latest_fg_color_P2info = last_fg_color_P2info = make_int_buf();
    latest_bg_color = last_bg_color = make_col_buf();
	latest_bg_color_P1info = last_bg_color_P1info = make_int_buf();
	latest_bg_color_P2info = last_bg_color_P2info = make_int_buf();
    resetLatest();

}

void View::updateGameObject(GameObject* obj) {
    Icon icon = obj->getIcon();
        Position pos  = obj->getPosition();
		
					/*取得P1 HP*/
		Tank* tank = dynamic_cast<Tank*> (obj);
		if(tank && obj -> getIcon()[1][1].color == PLAYER1){
	        if(tank -> getStatus() == burn) P1BackgroundColor = 202;
    	    else{
        	    P1BackgroundColor = 117;
            	for (int r = 0; r < GAME_WINDOW_HEIGHT; ++r) std::fill(latest_bg_color_P1info[r].begin(), latest_bg_color_P1info[r].end(), P1BackgroundColor);
        	}
			int HP = tank -> getHP();
			if(HP >= 100) {
				latest_P1info         [12][7] = '1';
				latest_P1info         [12][8] = '0';
			}else latest_P1info         [12][8] = HP / 10 + '0';
			
			latest_P1info         [12][9] = HP % 10 + '0';
			if(HP < 0){
				latest_P1info         [12][8] = '0';
				latest_P1info         [12][9] = '0';
			}
			insertStr(latest_P1info, std::to_string(tank -> getLayer() + 1), 14, 14);
			for(int Layer = 0; Layer < 3; Layer++){
				if(tank -> getType(Layer, 0) == SPLIT){
					insertStr(latest_P1info, "S+", Layer * 2 + 16, 10);
					latest_fg_color_P1info[Layer * 2 + 16][10] = 231;
					latest_fg_color_P1info[Layer * 2 + 16][11] = 231;
					latest_bg_color_P1info[Layer * 2 + 16][10] = 54;
					latest_bg_color_P1info[Layer * 2 + 16][11] = 54;
				}
				if(tank -> getType(Layer, 1) == SPLITX){
            	    insertStr(latest_P1info, "Sx", Layer * 2 + 16, 12);
                    latest_fg_color_P1info[Layer * 2 + 16][12] = 231;
                    latest_fg_color_P1info[Layer * 2 + 16][13] = 231;
                    latest_bg_color_P1info[Layer * 2 + 16][12] = 30;
                    latest_bg_color_P1info[Layer * 2 + 16][13] = 30;
        	    }
				if(tank -> getType(Layer, 2) == REBOUND){
	                insertStr(latest_P1info, "RE", Layer * 2 + 16, 16);
					latest_fg_color_P1info[Layer * 2 + 16][16] = 231;
                    latest_fg_color_P1info[Layer * 2 + 16][17] = 231;
                    latest_bg_color_P1info[Layer * 2 + 16][16] = 136;
                    latest_bg_color_P1info[Layer * 2 + 16][17] = 136;
            	}
				if(tank -> getType(Layer, 3) == BURN){
    	            insertStr(latest_P1info, "BU", Layer * 2 + 16, 14);
					latest_fg_color_P1info[Layer * 2 + 16][14] = 231;
                    latest_fg_color_P1info[Layer * 2 + 16][15] = 231;
                    latest_bg_color_P1info[Layer * 2 + 16][14] = 124;
                    latest_bg_color_P1info[Layer * 2 + 16][15] = 124;
	            }
			}
		}
		
					/*取得2P資訊*/
        if(tank && obj -> getIcon()[1][1].color == PLAYER2){
			if(tank -> getStatus() == burn) P2BackgroundColor = 202;
            else{
                P2BackgroundColor = 229;
            for (int r = 0; r < GAME_WINDOW_HEIGHT; ++r) std::fill(latest_bg_color_P2info[r].begin(), latest_bg_color_P2info[r].end(), P2BackgroundColor);
            }
            int HP = tank -> getHP();
            if(HP >= 100) {
                latest_P2info         [12][7] = '1';
                latest_P2info         [12][8] = '0';
            }else latest_P2info       [12][8] = HP / 10 + '0';
            latest_P2info         	  [12][9] = HP % 10 + '0';
			if(HP < 0){
                latest_P2info         [12][8] = '0';
                latest_P2info         [12][9] = '0';
            }
			insertStr(latest_P2info, std::to_string(tank -> getLayer() + 1), 14, 14);
			for(int Layer = 0; Layer < 3; Layer++){
                if(tank -> getType(Layer, 0) == SPLIT){
                    insertStr(latest_P2info, "S+", Layer * 2 + 16, 10);
                    latest_fg_color_P2info[Layer * 2 + 16][10] = 231;
                    latest_fg_color_P2info[Layer * 2 + 16][11] = 231;
                    latest_bg_color_P2info[Layer * 2 + 16][10] = 54;
                    latest_bg_color_P2info[Layer * 2 + 16][11] = 54;
                }
                if(tank -> getType(Layer, 1) == SPLITX){
                    insertStr(latest_P2info, "Sx", Layer * 2 + 16, 12);
                    latest_fg_color_P2info[Layer * 2 + 16][12] = 231;
                    latest_fg_color_P2info[Layer * 2 + 16][13] = 231;
                    latest_bg_color_P2info[Layer * 2 + 16][12] = 30;
                    latest_bg_color_P2info[Layer * 2 + 16][13] = 30;
                    }
                 if(tank -> getType(Layer, 2) == REBOUND){
                    insertStr(latest_P2info, "RE", Layer * 2 + 16, 16);
                    latest_fg_color_P2info[Layer * 2 + 16][16] = 231;
                    latest_fg_color_P2info[Layer * 2 + 16][17] = 231;
                    latest_bg_color_P2info[Layer * 2 + 16][16] = 136;
                    latest_bg_color_P2info[Layer * 2 + 16][17] = 136;
                }
                if(tank -> getType(Layer, 3) == BURN){
                    insertStr(latest_P2info, "BU", Layer * 2 + 16, 14);
                    latest_fg_color_P2info[Layer * 2 + 16][14] = 231;
                    latest_fg_color_P2info[Layer * 2 + 16][15] = 231;
                    latest_bg_color_P2info[Layer * 2 + 16][14] = 124;
                    latest_bg_color_P2info[Layer * 2 + 16][15] = 124;
                }
            }
        }
        for (std::size_t dy = 0; dy < icon.size(); ++dy) {
            int row = pos.y() + static_cast<int>(dy);
            if (row < 0 || row >= GAME_WINDOW_HEIGHT) continue;

            for (std::size_t dx = 0; dx < icon[dy].size(); ++dx) {
                int col = pos.x() + static_cast<int>(dx);
                if (col < 0 || col >= GAME_WINDOW_WIDTH) continue;

                const Cell& cell = icon[dy][dx];
				if(latest_bg_color[row][col] != NOCHANGE && cell.color == NOCHANGE) continue;
                latest_map      [row][col] = cell.ascii;
                latest_bg_color [row][col] = cell.color;
				if(cell.color == PLAYER1) {
					for(int i = 0; i < 4; i++){
						latest_P1info         [ 2 * dy + 3][ 4 * dx + i + 3] = cell.ascii;
						latest_bg_color_P1info[ 2 * dy + 3][ 4 * dx + i + 3] = cell.color;
						latest_P1info         [ 2 * dy + 4][ 4 * dx + i + 3] = cell.ascii;
                        latest_bg_color_P1info[ 2 * dy + 4][ 4 * dx + i + 3] = cell.color;
					}
				}
				else if(cell.color == PLAYER2){
					for(int i = 0; i < 4; i++){
                        latest_P2info         [ 2 * dy + 3][ 4 * dx + i + 3] = cell.ascii;
                        latest_bg_color_P2info[ 2 * dy + 3][ 4 * dx + i + 3] = cell.color;
                        latest_P2info         [ 2 * dy + 4][ 4 * dx + i + 3] = cell.ascii;
                        latest_bg_color_P2info[ 2 * dy + 4][ 4 * dx + i + 3] = cell.color;
                    }
				}
            }
        }

}

void View::render(){
    auto [rows, cols] = get_terminal_size();
    bool resized = (rows != _termHeight || cols != _termWidth);
    _termHeight = rows;
    _termWidth  = cols;

    if (resized) std::cout << "\033[2J\033[H";

    bool dirty = (   last_map      != latest_map
        || last_fg_color != latest_fg_color
        || last_bg_color != latest_bg_color
		|| last_P1info   != latest_P1info
		|| last_P2info   != latest_P2info);
    if (!dirty) return;

    std::string frame;
    frame.reserve((GAME_WINDOW_HEIGHT + 2) * (GAME_WINDOW_CELL_WIDTH * GAME_WINDOW_WIDTH + 3));


    // Top line

    frame += '+' + std::string( 20 , '-') + '+' + std::string(GAME_WINDOW_WIDTH * GAME_WINDOW_CELL_WIDTH, '-') + '+' + std::string( 20 , '-') + "+\n";

    for (int r = 0; r < GAME_WINDOW_HEIGHT; ++r) {
        int rowScreen = r + 2;
		// Left line
		frame += '|';

		/* 1P 資訊欄*/
		for(int c = 0; c < 20; ++c){
			const std::string& txt = latest_P1info[r][c];
			const auto         fg  = latest_fg_color_P1info[r][c];
			const auto         bg  = latest_bg_color_P1info[r][c];
			frame += AnsiPrint(txt.c_str(), fg, bg, true, false);
		}
        frame += '|';

        int pixelCol = 1;

        for (int c = 0; c < GAME_WINDOW_WIDTH; ++c) {
            const std::string& txt = latest_map[r][c];
            const auto         fg  = latest_fg_color[r][c];
            const auto         bg  = latest_bg_color[r][c];

            int w        = std::max(1, displayWidth(txt));
            int padTotal = GAME_WINDOW_CELL_WIDTH - w;
            int padLeft  = padTotal / 2;
            int padRight = padTotal - padLeft;

            // Left blank
            for (int p = 0; p < padLeft; ++p) {
                frame += AnsiPrint(" ", Color::NOCHANGE, bg, false, false);
            }

            // icon 
            frame += AnsiPrint(txt.c_str(), fg, bg, false, false);

            // Right 
            for (int p = 0; p < padRight; ++p) {
                frame += AnsiPrint(" ", Color::NOCHANGE, bg, false, false);
            }

            pixelCol += GAME_WINDOW_CELL_WIDTH;
        }

		frame += '|';

		/* 2P 資訊欄 */
		for(int c = 0; c < 20; ++c){
            const std::string& txt = latest_P2info[r][c];
            const auto         fg  = latest_fg_color_P2info[r][c];
            const auto         bg  = latest_bg_color_P2info[r][c];
            frame += AnsiPrint(txt.c_str(), fg, bg, true, false);
        }


        // Right line
        frame += "|\n";

    }
    // Bottom line
	frame += '+' + std::string( 20 , '-') + '+' + std::string(GAME_WINDOW_WIDTH * GAME_WINDOW_CELL_WIDTH, '-') + '+' + std::string( 20 , '-') + "+\n";
    std::cout << "\033[H" << frame << std::flush;


    // update buffer
    last_map             = latest_map;
	last_P1info          = latest_P1info;
	last_P2info          = latest_P2info;
    last_fg_color        = latest_fg_color;
	last_fg_color_P1info = latest_fg_color_P1info;
	last_fg_color_P2info = latest_fg_color_P2info;
    last_bg_color        = latest_bg_color;
	last_bg_color_P1info = latest_bg_color_P1info;
	last_bg_color_P2info = latest_bg_color_P2info;
}


void View::resetLatest(){
    for (int r = 0; r < GAME_WINDOW_HEIGHT; ++r) {
        std::fill(latest_map[r].begin(),   latest_map[r].end(),   " ");
		std::fill(latest_P1info[r].begin(), latest_P1info[r].end(), " ");
		std::fill(latest_P2info[r].begin(), latest_P2info[r].end(), " ");
        std::fill(latest_fg_color[r].begin(), latest_fg_color[r].end(), Color::NOCHANGE);
		std::fill(latest_fg_color_P1info[r].begin(), latest_fg_color_P1info[r].end(), P1ForegroundColor);
        std::fill(latest_fg_color_P2info[r].begin(), latest_fg_color_P2info[r].end(), P2ForegroundColor);
		std::fill(latest_bg_color[r].begin(), latest_bg_color[r].end(),BackgroundColor);
		std::fill(latest_bg_color_P1info[r].begin(), latest_bg_color_P1info[r].end(), P1BackgroundColor);
		std::fill(latest_bg_color_P2info[r].begin(), latest_bg_color_P2info[r].end(), P2BackgroundColor);
    }
	insertStr(latest_P1info, "HP:", 12, 3);
	insertStr(latest_P2info, "HP:", 12, 3);
	insertStr(latest_P1info, "Now Layer:", 14, 3);
	insertStr(latest_P2info, "Now Layer:", 14, 3);
	insertStr(latest_P1info, "Layer1:", 16, 3);
	insertStr(latest_P1info, "Layer2:", 18, 3);
	insertStr(latest_P1info, "Layer3:", 20, 3);
	insertStr(latest_P2info, "Layer1:", 16, 3);
    insertStr(latest_P2info, "Layer2:", 18, 3);
    insertStr(latest_P2info, "Layer3:", 20, 3);
}
std::pair<int,int> View::get_terminal_size() {
    int rows, cols;
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        rows = w.ws_row;
        cols = w.ws_col;
    } else {
        rows = cols = -1; // Error case
    }
    return std::make_pair(rows, cols);
}
void View::insertStr(std::vector<std::vector<std::string>>& S, std::string what, int y, int pos){
	for(int i = 0; i < what.size(); i++) S[y][i + pos] = what[i];
}
