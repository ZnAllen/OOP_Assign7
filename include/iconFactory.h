// TODO 
// Finish the declaration of IconFactory that
// 1. offers some methods to create an Icon.
// 2. encapsulates the constructor from the caller.
#ifndef ICONFACTORY_H
#define ICONFACTORY_H
#include "icon.h"
#include "bullet.h"

enum status{me, object};

class iconFactory{
	public:
	   	static Icon createIcon(status which);
		static Icon createRPSIcon(status type);
		static Icon createBulletIcon(std::vector<BulletType> type);
};

#endif
