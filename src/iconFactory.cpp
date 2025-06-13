// TODO implementation

#include "iconFactory.h"
#include <iostream>
#include "tank.h"

Icon iconFactory::createIcon(status which){
	Icon icon;
	icon.push_back(std::vector<Cell>());
	if(which == me){
		icon[0].push_back(Cell(RED, " "));
	}else if(which == object){
		icon[0].push_back(Cell(BLUE, " "));
	}
	return icon;
}

Icon iconFactory::createRPSIcon(status which){
	Icon icon(3);
	switch(which){
		case me:
			icon[1].push_back(Cell(PLAYER1, " "));
			icon[1].push_back(Cell(PLAYER1, " "));
			icon[1].push_back(Cell(PLAYER1, " "));
			icon[0].push_back(Cell(NOCHANGE, " "));
			icon[0].push_back(Cell(PLAYER1, " "));
			icon[0].push_back(Cell(NOCHANGE, " "));
			icon[2].push_back(Cell(PLAYER1, " "));
			icon[2].push_back(Cell(PLAYER1, " "));
			icon[2].push_back(Cell(PLAYER1, " "));
			break;
		case object:
			icon[0].push_back(Cell(NOCHANGE, " "));
			icon[0].push_back(Cell(PLAYER2, " "));
	 		icon[0].push_back(Cell(NOCHANGE, " "));
			icon[1].push_back(Cell(PLAYER2, " "));
			icon[1].push_back(Cell(PLAYER2, " "));
			icon[1].push_back(Cell(PLAYER2, " "));
			icon[2].push_back(Cell(PLAYER2, " "));
			icon[2].push_back(Cell(PLAYER2, " "));
			icon[2].push_back(Cell(PLAYER2, " "));
			break;
	}
	return icon;
}

Icon iconFactory::createBulletIcon(std::vector<BulletType> type){
	Icon icon;
	icon.push_back(std::vector<Cell>());
	if(type[3] != BURN) icon[0].push_back(Cell(CYAN, " "));
	else icon[0].push_back(Cell(RED, " "));
	return icon;
}
