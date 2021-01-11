#pragma once

#include "gbe_types.h"


class GBEngine
{
public:
	static GBEngineConfig &getDefaultEngineConfigStruct();
	static bool Init(GBEngineConfig &initConfig);


};
