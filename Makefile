# Common compilation definitions.
COMPILER := g++
LINKER := ar
COMMON_COMPILE_FLAGS := -Wall -Werror -std=c++11
LIBRARY_COMPILE_FLAGS := -fPIC -c
LIBRARY_BUILD_FLAGS := -shared
ENGINE_ROOT=$(shell pwd)/

# Add debug flags to compilation.
DEBUG_COMPILE_FLAGS := -g
ifdef DEBUG
    COMMON_COMPILE_FLAGS += $(DEBUG_COMPILE_FLAGS)
endif

# Common module definitions.
LIBRARY_OUTPUT_PATH=$(ENGINE_ROOT)lib/
INCLUDE_SUBDIRECTORY := include/
SOURCE_SUBDIRECTORY := source/
BUILD_SUBDIRECTORY := build/

# Engine common library definitions.
ENGINE_COMMON_NAME := engine_common
ENGINE_COMMON_ROOT := $(ENGINE_ROOT)$(ENGINE_COMMON_NAME)/
ENGINE_COMMON_VERSION_MAJOR := 1
ENGINE_COMMON_VERSION_MINOR := 0
ENGINE_COMMON_VERSION_BUILD := 0
ENGINE_COMMON_INCLUDE_FLAGS := -I$(ENGINE_COMMON_ROOT)$(INCLUDE_SUBDIRECTORY)
ENGINE_COMMON_SOURCE_DIRECTORY := $(ENGINE_COMMON_ROOT)$(SOURCE_SUBDIRECTORY)
ENGINE_COMMON_BUILD_DIRECTORY := $(ENGINE_COMMON_ROOT)$(BUILD_SUBDIRECTORY)
ENGINE_COMMON_COMPILE_FLAGS := $(ENGINE_COMMON_INCLUDE_FLAGS) $(ENGINE_COMMON_LIBRARY_FLAGS) $(COMMON_COMPILE_FLAGS) $(LIBRARY_COMPILE_FLAGS)
ENGINE_COMMON_OBJECTS := \
	$(ENGINE_COMMON_BUILD_DIRECTORY)renderer/attribute.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)renderer/buffer_layout.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)allocatable.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)error_stack.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)file.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)math_common.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)matrix3x3.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)matrix4x4.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)memory_manager.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)vector2.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)vector3.o \
	$(ENGINE_COMMON_BUILD_DIRECTORY)vector4.o

# Main engine module definitions.
ENGINE_NAME := engine
ENGINE_VERSION_MAJOR := 1
ENGINE_VERSION_MINOR := 0
ENGINE_VERSION_BUILD := 0
ENGINE_INCLUDE_DIRECTORY := $(ENGINE_ROOT)$(INCLUDE_SUBDIRECTORY)
ENGINE_SOURCE_DIRECTORY := $(ENGINE_ROOT)$(SOURCE_SUBDIRECTORY)
ENGINE_BUILD_DIRECTORY := $(ENGINE_ROOT)$(BUILD_SUBDIRECTORY)
ENGINE_INCLUDE_FLAGS := \
    -I$(ENGINE_INCLUDE_DIRECTORY) \
    -I/usr/include/SDL2/ \
    $(ENGINE_COMMON_INCLUDE_FLAGS)
ENGINE_LIBRARY_FLAGS := \
    -L$(LIBRARY_OUTPUT_PATH)
ENGINE_COMPILE_FLAGS := $(ENGINE_INCLUDE_FLAGS) $(ENGINE_LIBRARY_FLAGS) $(COMMON_COMPILE_FLAGS) $(LIBRARY_COMPILE_FLAGS)
ENGINE_OBJECTS := \
    $(ENGINE_BUILD_DIRECTORY)engine.o \
    $(ENGINE_BUILD_DIRECTORY)engine_export.o \
    $(ENGINE_BUILD_DIRECTORY)sdl_window.o \
    $(ENGINE_BUILD_DIRECTORY)window.o

# Game manager module definitions.
GAME_MANAGER_NAME := game_manager
GAME_MANAGER_ROOT := $(ENGINE_ROOT)$(GAME_MANAGER_NAME)/
GAME_MANAGER_VERSION_MAJOR := 1
GAME_MANAGER_VERSION_MINOR := 0
GAME_MANAGER_VERSION_BUILD := 0
GAME_MANAGER_INCLUDE_DIRECTORY := $(GAME_MANAGER_ROOT)$(INCLUDE_SUBDIRECTORY)
GAME_MANAGER_SOURCE_DIRECTORY := $(GAME_MANAGER_ROOT)$(SOURCE_SUBDIRECTORY)
GAME_MANAGER_BUILD_DIRECTORY := $(GAME_MANAGER_ROOT)$(BUILD_SUBDIRECTORY)
GAME_MANAGER_INCLUDE_FLAGS := \
	$(ENGINE_COMMON_INCLUDE_FLAGS) \
	-I$(ENGINE_INCLUDE_DIRECTORY) \
	-I$(GAME_MANAGER_INCLUDE_DIRECTORY)
GAME_MANAGER_LIBRARY_FLAGS := \
	-L$(LIBRARY_OUTPUT_PATH)
GAME_MANAGER_COMPILE_FLAGS := \
	$(GAME_MANAGER_INCLUDE_FLAGS) \
	$(GAME_MANAGER_LIBRARY_FLAGS) \
	$(COMMON_COMPILE_FLAGS) $(LIBRARY_COMPILE_FLAGS)
GAME_MANAGER_OBJECTS := \
	$(GAME_MANAGER_BUILD_DIRECTORY)game_manager.o \
	$(GAME_MANAGER_BUILD_DIRECTORY)game_manager_export.o \
	$(GAME_MANAGER_BUILD_DIRECTORY)game_module.o

# Quake 2 common library definitions.
QUAKE2_COMMON_NAME := quake2_common
QUAKE2_COMMON_ROOT := $(ENGINE_ROOT)$(QUAKE2_COMMON_NAME)/
QUAKE2_COMMON_VERSION_MAJOR := 1
QUAKE2_COMMON_VERSION_MINOR := 0
QUAKE2_COMMON_VERSION_BUILD := 0
QUAKE2_COMMON_INCLUDE_DIRECTORY := $(QUAKE2_COMMON_ROOT)$(INCLUDE_SUBDIRECTORY)
QUAKE2_COMMON_SOURCE_DIRECTORY := $(QUAKE2_COMMON_ROOT)$(SOURCE_SUBDIRECTORY)
QUAKE2_COMMON_BUILD_DIRECTORY := $(QUAKE2_COMMON_ROOT)$(BUILD_SUBDIRECTORY)
QUAKE2_COMMON_INCLUDE_FLAGS := \
	-I$(QUAKE2_COMMON_INCLUDE_DIRECTORY) \
	$(ENGINE_COMMON_INCLUDE_FLAGS)
QUAKE2_COMMON_LIBRARY_FLAGS := \
	-L$(LIBRARY_OUTPUT_PATH)
QUAKE2_COMMON_COMPILE_FLAGS := \
	$(QUAKE2_COMMON_INCLUDE_FLAGS) \
	$(QUAKE2_COMMON_LIBRARY_FLAGS) \
	$(COMMON_COMPILE_FLAGS) \
	$(LIBRARY_COMPILE_FLAGS)
QUAKE2_COMMON_OBJECTS := \
	$(QUAKE2_COMMON_BUILD_DIRECTORY)bsp_map.o \
	$(QUAKE2_COMMON_BUILD_DIRECTORY)bsp_painter.o \
	$(QUAKE2_COMMON_BUILD_DIRECTORY)bsp_parser.o \
	$(QUAKE2_COMMON_BUILD_DIRECTORY)pack_manager.o \
	$(QUAKE2_COMMON_BUILD_DIRECTORY)pcx_parser.o \
	$(QUAKE2_COMMON_BUILD_DIRECTORY)plane.o \
	$(QUAKE2_COMMON_BUILD_DIRECTORY)quake_file_manager.o \
	$(QUAKE2_COMMON_BUILD_DIRECTORY)wal_parser.o

# Client library definitions.
CLIENT_NAME := client
CLIENT_ROOT := $(ENGINE_ROOT)$(CLIENT_NAME)/
CLIENT_VERSION_MAJOR := 1
CLIENT_VERSION_MINOR := 0
CLIENT_VERSION_BUILD := 0
CLIENT_INCLUDE_DIRECTORY := $(CLIENT_ROOT)$(INCLUDE_SUBDIRECTORY)
CLIENT_SOURCE_DIRECTORY := $(CLIENT_ROOT)$(SOURCE_SUBDIRECTORY)
CLIENT_BUILD_DIRECTORY := $(CLIENT_ROOT)$(BUILD_SUBDIRECTORY)
CLIENT_INCLUDE_FLAGS := \
	$(ENGINE_COMMON_INCLUDE_FLAGS) \
	-I$(GAME_MANAGER_INCLUDE_DIRECTORY) \
	-I$(QUAKE2_COMMON_INCLUDE_DIRECTORY) \
	-I$(CLIENT_INCLUDE_DIRECTORY)
CLIENT_LIBRARY_FLAGS := \
	-L$(LIBRARY_OUTPUT_PATH)
CLIENT_COMPILE_FLAGS := \
	$(CLIENT_INCLUDE_FLAGS) \
	$(CLIENT_LIBRARY_FLAGS) \
	$(COMMON_COMPILE_FLAGS) \
	$(LIBRARY_COMPILE_FLAGS)
CLIENT_OBJECTS := \
	$(CLIENT_BUILD_DIRECTORY)camera.o \
	$(CLIENT_BUILD_DIRECTORY)client_export.o \
	$(CLIENT_BUILD_DIRECTORY)entity_model.o \
	$(CLIENT_BUILD_DIRECTORY)game_client.o \
	$(CLIENT_BUILD_DIRECTORY)md2_parser.o \
	$(CLIENT_BUILD_DIRECTORY)quake_normals.o

# OpenGL renderer library definitions.
OPENGL_NAME := opengl_renderer
OPENGL_ROOT = $(ENGINE_ROOT)$(OPENGL_NAME)/
OPENGL_VERSION_MAJOR := 1
OPENGL_VERSION_MINOR := 0
OPENGL_VERSION_BUILD := 0
OPENGL_INCLUDE_DIRECTORY := $(OPENGL_ROOT)$(INCLUDE_SUBDIRECTORY)
OPENGL_INCLUDE_FLAGS := \
	-I$(OPENGL_INCLUDE_DIRECTORY) \
	$(ENGINE_COMMON_INCLUDE_FLAGS)
OPENGL_SOURCE_DIRECTORY := $(OPENGL_ROOT)$(SOURCE_SUBDIRECTORY)
OPENGL_BUILD_DIRECTORY := $(OPENGL_ROOT)$(BUILD_SUBDIRECTORY)
OPENGL_LIBRARY_FLAGS := \
    -L$(LIBRARY_OUTPUT_PATH)
#    -lGL -lGLU -l$(ENGINE_COMMON_NAME)
OPENGL_COMPILE_FLAGS := $(OPENGL_INCLUDE_FLAGS) $(OPENGL_LIBRARY_FLAGS) $(COMMON_COMPILE_FLAGS) $(LIBRARY_COMPILE_FLAGS)
OPENGL_OBJECTS := \
    $(OPENGL_BUILD_DIRECTORY)attribute.o \
    $(OPENGL_BUILD_DIRECTORY)buffer.o \
    $(OPENGL_BUILD_DIRECTORY)buffer_layout.o \
    $(OPENGL_BUILD_DIRECTORY)common.o \
    $(OPENGL_BUILD_DIRECTORY)index_buffer.o \
    $(OPENGL_BUILD_DIRECTORY)material.o \
    $(OPENGL_BUILD_DIRECTORY)material_layout.o \
    $(OPENGL_BUILD_DIRECTORY)opengl_export.o \
    $(OPENGL_BUILD_DIRECTORY)renderer.o \
    $(OPENGL_BUILD_DIRECTORY)resources.o \
    $(OPENGL_BUILD_DIRECTORY)texture.o \
    $(OPENGL_BUILD_DIRECTORY)variable.o

# Quake II executable definitions.
QUAKE2_TARGET_NAME=quake2
QUAKE2_LIBRARIES=-l$(ENGINE_COMMON_LIBRARY_NAME)

# List of libraries.
LIBRARIES := \
	$(ENGINE_COMMON_NAME) \
    $(ENGINE_NAME) \
	$(GAME_MANAGER_NAME) \
	$(QUAKE2_COMMON_NAME) \
	$(CLIENT_NAME) \
	$(OPENGL_NAME)

# Main make target.
all: libraries

# Library targets.
libraries: create_library_directory $(LIBRARIES)

# Game executable target.
$(QUAKE2_TARGET_NAME): null

# Engine common library target.
# TODO: Can probably put these defs into a macro.
$(ENGINE_COMMON_NAME): BUILD_DIRECTORY := $(ENGINE_COMMON_BUILD_DIRECTORY) $(ENGINE_COMMON_BUILD_DIRECTORY)renderer/
$(ENGINE_COMMON_NAME): BUILD_OBJECTS := $(ENGINE_COMMON_OBJECTS)
$(ENGINE_COMMON_NAME): VERSION_MAJOR := $(ENGINE_COMMON_VERSION_MAJOR)
$(ENGINE_COMMON_NAME): VERSION_FULL := $(VERSION_MAJOR).$(ENGINE_COMMON_VERSION_MINOR).$(ENGINE_COMMON_VERSION_BUILD)
$(ENGINE_COMMON_NAME): create_$(ENGINE_COMMON_NAME)_build_directory $(ENGINE_COMMON_OBJECTS)
$(ENGINE_COMMON_BUILD_DIRECTORY)%.o : $(ENGINE_COMMON_SOURCE_DIRECTORY)%.cpp
	$(COMPILER) -o $@ $< $(ENGINE_COMMON_COMPILE_FLAGS)

# Base engine library target.
$(ENGINE_NAME): BUILD_DIRECTORY := $(ENGINE_BUILD_DIRECTORY)
$(ENGINE_NAME): BUILD_OBJECTS := $(ENGINE_OBJECTS)
$(ENGINE_NAME): VERSION_MAJOR := $(ENGINE_VERSION_MAJOR)
$(ENGINE_NAME): VERSION_FULL := $(VERSION_MAJOR).$(ENGINE_VERSION_MINOR).$(ENGINE_VERSION_BUILD)
$(ENGINE_NAME): create_$(ENGINE_NAME)_build_directory $(ENGINE_OBJECTS)
$(ENGINE_BUILD_DIRECTORY)%.o : $(ENGINE_SOURCE_DIRECTORY)%.cpp
	$(COMPILER) -o $@ $< $(ENGINE_COMPILE_FLAGS)

# Game manager library target.
$(GAME_MANAGER_NAME): BUILD_DIRECTORY := $(GAME_MANAGER_BUILD_DIRECTORY)
$(GAME_MANAGER_NAME): BUILD_OBJECTS := $(GAME_MANAGER_OBJECTS)
$(GAME_MANAGER_NAME): VERSION_MAJOR := $(GAME_MANAGER_VERSION_MAJOR)
$(GAME_MANAGER_NAME): VERSION_FULL := $(VERSION_MAJOR).$(GAME_MANAGER_VERSION_MINOR).$(GAME_MANAGER_VERSION_BUILD)
$(GAME_MANAGER_NAME): create_$(GAME_MANAGER_NAME)_build_directory $(GAME_MANAGER_OBJECTS)
$(GAME_MANAGER_BUILD_DIRECTORY)%.o : $(GAME_MANAGER_SOURCE_DIRECTORY)%.cpp
	$(COMPILER) -o $@ $< $(GAME_MANAGER_COMPILE_FLAGS)

# Quake 2 common library.
$(QUAKE2_COMMON_NAME): BUILD_DIRECTORY := $(QUAKE2_COMMON_BUILD_DIRECTORY)
$(QUAKE2_COMMON_NAME): BUILD_OBJECTS := $(QUAKE2_COMMON_OBJECTS)
$(QUAKE2_COMMON_NAME): VERSION_MAJOR := $(QUAKE2_COMMON_VERSION_MAJOR)
$(QUAKE2_COMMON_NAME): VERSION_FULL := $(VERSION_MAJOR).$(QUAKE2_COMMON_VERSION_MINOR).$(QUAKE2_COMMON_VERSION_BUILD)
$(QUAKE2_COMMON_NAME): create_$(QUAKE2_COMMON_NAME)_build_directory $(QUAKE2_COMMON_OBJECTS)
$(QUAKE2_COMMON_BUILD_DIRECTORY)%.o : $(QUAKE2_COMMON_SOURCE_DIRECTORY)%.cpp
	$(COMPILER) -o $@ $< $(QUAKE2_COMMON_COMPILE_FLAGS)

# Client module library.
$(CLIENT_NAME): BUILD_DIRECTORY := $(CLIENT_BUILD_DIRECTORY)
$(CLIENT_NAME): BUILD_OBJECTS := $(CLIENT_OBJECTS)
$(CLIENT_NAME): VERSION_MAJOR := $(CLIENT_VERSION_MAJOR)
$(CLIENT_NAME): VERSION_FULL := $(VERSION_MAJOR).$(CLIENT_VERSION_MINOR).$(CLIENT_VERSION_BUILD)
$(CLIENT_NAME): create_$(CLIENT_NAME)_build_directory $(CLIENT_OBJECTS)
$(CLIENT_BUILD_DIRECTORY)%.o : $(CLIENT_SOURCE_DIRECTORY)%.cpp
	$(COMPILER) -o $@ $< $(CLIENT_COMPILE_FLAGS)

# OpenGL library target.
$(OPENGL_NAME): BUILD_DIRECTORY := $(OPENGL_BUILD_DIRECTORY)
$(OPENGL_NAME): BUILD_OBJECTS := $(OPENGL_OBJECTS)
$(OPENGL_NAME): VERSION_MAJOR := $(OPENGL_VERSION_MAJOR)
$(OPENGL_NAME): VERSION_FULL := $(VERSION_MAJOR).$(OPENGL_VERSION_MINOR).$(OPENGL_VERSION_BUILD)
$(OPENGL_NAME): create_$(OPENGL_NAME)_build_directory $(OPENGL_OBJECTS)
$(OPENGL_BUILD_DIRECTORY)%.o : $(OPENGL_SOURCE_DIRECTORY)%.cpp
	$(COMPILER) -o $@ $< $(OPENGL_COMPILE_FLAGS)

# Generic library target.
$(LIBRARIES):
	$(COMPILER) \
		-o $(LIBRARY_OUTPUT_PATH)lib$@.so.$(VERSION_FULL) \
		-Wl,-soname,lib$@.so.$(VERSION_MAJOR) \
		$(BUILD_OBJECTS) \
		$(LIBRARY_BUILD_FLAGS)

# Creating library directory.
create_library_directory:
	mkdir -p $(LIBRARY_OUTPUT_PATH)

# Creating build directory.
create_%_build_directory:
	mkdir -p $(BUILD_DIRECTORY)
