/*************************************************************

gibbie-03
Copyright (c) 2020-2020, Gianluca Belardelli

File:    vk_init.cpp
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
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <memory>

#include <string>
#include <iostream>

#include <vulkan/vulkan.h>

#include "vk_layers.h"
#include "vk_extensions.h"

VkInstance gVKInstance = VK_NULL_HANDLE;
VkPhysicalDevice gVKDevice = VK_NULL_HANDLE;
VkDevice gMainDevice = VK_NULL_HANDLE;

constexpr uint32_t cVulkanVersion = VK_MAKE_VERSION(1, 2, 154);
constexpr uint32_t cEngineVersion = VK_MAKE_VERSION(0, 1, 0);
constexpr char *cEngineName = "GIBBIEngine";

bool gEnableVerboseInfo = true;

void printExtension( const char *layerName )
{
	if (!gEnableVerboseInfo)
		return;

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr);

	if (!layerName)
		std::cout << std::endl << std::endl <<"Lista estensioni disponibili:" << std::endl;

	if (!extensionCount)
	{
		std::cout << "   (Nessuna estensione disponibile)" << std::endl;
		return;
	}

	std::vector<VkExtensionProperties> instanceExtensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, instanceExtensions.data());

	for( const auto& extension : instanceExtensions)
	{
		std::cout << "    - ";
		std::cout << extension.extensionName << "  Vers: " <<
			VK_VERSION_MAJOR(extension.specVersion) << "." << VK_VERSION_MINOR(extension.specVersion) << 
			"." << VK_VERSION_PATCH(extension.specVersion) <<  std::endl;
	}

	return;
}

void printLayers()
{
	if (!gEnableVerboseInfo)
		return;

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	if (!layerCount)
	{
		std::cout << "Nessun layer non trovato nel sistema." << std::endl;
		return;
	}

	std::vector<VkLayerProperties> availableLayersList(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayersList.data());

	for (const auto& layerProp : availableLayersList)
	{
		std::cout << layerProp.layerName << " (Impl Vers: " <<
			VK_VERSION_MAJOR(layerProp.implementationVersion) << "." <<
			VK_VERSION_MINOR(layerProp.implementationVersion) << "." <<
			VK_VERSION_PATCH(layerProp.implementationVersion) <<
			") (Spec Vers: " <<
			VK_VERSION_MAJOR(layerProp.specVersion) << "." <<
			VK_VERSION_MINOR(layerProp.specVersion) << "." <<
			VK_VERSION_PATCH(layerProp.specVersion) <<
			")" << std::endl;

		std::cout << "   " << layerProp.description << std::endl;

		printExtension(layerProp.layerName);
	}
}

void printDeviceExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(gVKDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> deviceExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(gVKDevice, nullptr, &extensionCount, deviceExtensions.data());

	for (const auto& extension : deviceExtensions)
	{
		std::cout << "    - ";
		std::cout << extension.extensionName << "  Vers: " <<
			VK_VERSION_MAJOR(extension.specVersion) << "." << VK_VERSION_MINOR(extension.specVersion) <<
			"." << VK_VERSION_PATCH(extension.specVersion) << std::endl;
	}
}

void printDeviceLayers()
{
	uint32_t layerCount = 0;
	vkEnumerateDeviceLayerProperties(gVKDevice, &layerCount, nullptr);

	std::vector<VkLayerProperties> layerProperties(layerCount);
	vkEnumerateDeviceLayerProperties(gVKDevice, &layerCount, layerProperties.data());

	for (const auto& layer : layerProperties)
	{
		std::cout << "  - ";
		std::cout << layer.layerName << std::endl << "      "  << layer.description << "  Vers: " <<
			VK_VERSION_MAJOR(layer.specVersion) << "." << VK_VERSION_MINOR(layer.specVersion) <<
			"." << VK_VERSION_PATCH(layer.specVersion) << std::endl;
	}
}

bool checkLayersSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	if (!layerCount)
	{
		std::cout << "Nessun layer trovato nel sistema." << std::endl;
		return false;
	}

	std::vector<VkLayerProperties> availableLayersList(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayersList.data());

	for( auto requestedLayerName: validation_layers_list)
	{
		bool layerFound = false;

		for( const auto& layerProp: availableLayersList)
		{
			if (strcmp(requestedLayerName, layerProp.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}
		if (!layerFound)
		{
			std::cout << "Layer: " << requestedLayerName << " non trovato." << std::endl;
			return false;
		}
	}

	return true;
}

bool checkExtensionsSupport()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	if (!extensionCount)
	{
		std::cout << "Nessun estensione trovata nel sistema." << std::endl;
		return false;
	}

	std::vector<VkExtensionProperties> instanceExtensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, instanceExtensions.data());

	for (auto requestedExtensionName : extensions_list)
	{
		bool extensionFound = false;

		for (const auto& extension : instanceExtensions)
		{
			if (strcmp(requestedExtensionName, extension.extensionName) == 0)
			{
				extensionFound = true;
				break;
			}
		}
		if (!extensionFound)
		{
			std::cout << "Estensione: " << requestedExtensionName << " non trovata." << std::endl;
			return false;
		}
	}

	return true;
}

bool InitVulkan(std::string appName, int appMaj, int appMin, int appBuild)
{
	if( enableValidationLayers && validation_layers_list.size() > 0 && 
		( !checkLayersSupport() || !checkExtensionsSupport() ))
	{
		std::cout << "Richiesti layer di validazione che non sono disponibili!" << std::endl;
		std::abort();
	}
	
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = appName.c_str();
	app_info.pEngineName = cEngineName;
	app_info.engineVersion = cEngineVersion;
	app_info.applicationVersion = VK_MAKE_VERSION(appMaj, appMin, appBuild);
	app_info.apiVersion = cVulkanVersion;

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = static_cast<uint32_t>(extensions_list.size());
	create_info.ppEnabledExtensionNames = extensions_list.data();
	create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers_list.size());
	create_info.ppEnabledLayerNames = validation_layers_list.data();

	VkResult res = vkCreateInstance(
		&create_info,
		nullptr,
		&gVKInstance
	);

	printExtension(nullptr);

	if (res != VK_SUCCESS)
		return false;

	return true;
}

void ShutdownVulkan()
{
	vkDestroyDevice(gMainDevice, nullptr);

	vkDestroyInstance(gVKInstance, nullptr);
}

bool InitDevice()
{
	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(gVKInstance, &deviceCount, nullptr);
	if( deviceCount == 0 )
	{
		std::cout << "Nessuna device rilevata!" << std::endl;
		return false;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(gVKInstance, &deviceCount, devices.data());

	for( auto& device: devices )
	{
		VkPhysicalDeviceProperties devProp;
		vkGetPhysicalDeviceProperties(device, &devProp);

		if( devProp.deviceType & (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU | VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) )
		{
			std::cout << "Device trovata: " << std::endl;
			std::cout << "  Nome: " << devProp.deviceName << std::endl;
			std::cout << "  ID: " << devProp.deviceID << std::endl;

			gVKDevice = device;
			break;
		}
	}

	if (gVKDevice == VK_NULL_HANDLE)
		return false;

	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(gVKDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(gVKDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	bool hasQueue = false;
	for( const auto& queueFamily: queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			hasQueue = true;
			break;
		}
		
		i++;
	}

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = i;
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures devFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &devFeatures;
	createInfo.enabledExtensionCount = 0;
	createInfo.enabledLayerCount = 0;

	vkCreateDevice(gVKDevice, &createInfo, nullptr, &gMainDevice);
	if (gMainDevice == VK_NULL_HANDLE)
		return false;

	//printDeviceLayers(); 
	//printDeviceExtensions();

	return true;
}