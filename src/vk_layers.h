/*************************************************************

gibbie-01
Copyright (c) 2020-2020, Gianluca Belardelli

File:    vk_layers.h
Author:  Gianluca Belardelli
Date:    17/12/2020

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
#pragma once

#include <vector>

const std::vector<const char*> validation_layers_list = {
	"VK_LAYER_KHRONOS_validation",
	"VK_LAYER_LUNARG_api_dump"
};

#ifdef _DEBUG
	constexpr bool enableValidationLayers = true;
#else
	constexpr bool enableValidationLayers = false;
#endif

