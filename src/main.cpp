/*************************************************************

gibbie-03
Copyright (c) 2020-2020, Gianluca Belardelli

File:    main.cpp
Author:  Gianluca Belardelli
Date:    22/12/2020

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************/
#include <string>
#include <iostream>
#include "GameApplication.h"

extern bool InitVulkan(std::string appName, int appMaj, int appMin, int appBuild);
extern void ShutdownVulkan();
extern bool InitDevice();

GameApplication gGame;

int main(int argc, char* argv[])
{
	bool res = InitVulkan("Test", 1, 0, 0);
	if (!res)
	{
		std::cout << "Errore durante l'inizializzazione di Vulkan" << std::endl;
		ShutdownVulkan();
		return 1;
	}

	std::cout << "Vulkan instance creata con successo." << std::endl;

	res = InitDevice();
	if (!res)
	{
		std::cout << "Errore durante l'inizializzazione della device" << std::endl;
		ShutdownVulkan();
		return 1;
	}

	std::cout << "Vulkan device creata con successo." << std::endl;
	ShutdownVulkan();

	gGame.run();
	return 0;
}
