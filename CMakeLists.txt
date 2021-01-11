cmake_minimum_required(VERSION 3.7.1)

set(project_name GBEngine)

project(${project_name})

set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT "GBEngine")

set(HAVE_LIBC TRUE)
set(src
    src/main.cpp
	src/vk_init.cpp
	src/gbe_baseapplication.cpp
	src/gbe_win32Window.cpp
	src/gbengine.cpp
)
 
set(include
	src/vk_layers.h
	src/vk_extensions.h
	src/gbe_baseapplication.h
	src/gbe_basewindow.h
	src/gbe_win32Window.h
	src/gbe_types.h
	src/gbengine.h
)


add_executable(${project_name} ${src} ${include} build.licenseheader)

find_package(Vulkan)

if(Vulkan_FOUND)
	target_include_directories(${project_name} PRIVATE Vulkan::Vulkan)
	target_link_libraries(${project_name} Vulkan::Vulkan)

	foreach(source IN LISTS src)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "/" "\\" source_path_msvc "${source_path}")
	    source_group("${source_path_msvc}" FILES "${source}")
	endforeach()

	foreach(source IN LISTS include)
    	source_group(inc FILES "${source}" )
    endforeach()

endif()
