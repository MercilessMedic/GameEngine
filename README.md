# Sandrito's Engine

A lightweight, high-performance 3D game engine backend built from scratch using modern C++ and the OpenGL 3.3 Core Profile graphics pipeline. 
Engine implements a custom Entity Component System (ECS) architecture for optimal memory layout and decoupling game logic from hardware systems.

**Quickstart**

This repository features a pre bundled  zero-configuration setup. All dependencies are included directly in the workspace.

1)Clone the Project: git clone https://github.com/MercilessMedic/GameEngine.git

2) Open  GameEngine.sln to load the project in Visual Studio 2022 or later

3)Compile and run

**Core features**

Panoramic skyboxes,

Model laoding,

Custom rendering and transformation systems,

PBR rendering,

Phong Rendering,

custom Entity component system,

ImGui editor interface.



Assets/                 # Runtime assets (Shaders, Textures, Models, Skybox maps)


Game/                   # Application/Sandbox-specific code and scene definitions


Source/                 # Core engine framework (Renderer, ECS Architecture, Windows)


ThirdParty/             # Pre-bundled external development headers and static libraries


GameEngine.vcxproj      # Visual Studio Project configuration file


main.cpp                # Core application runtime entry point


SDL2.dll                # Windowing & OS context dynamic link library


SDL2_ttf.dll            # Text rasterization dynamic link library


assimp-vc145-mtd.dll    # 3D Model importing dynamic link library

**Contribution Guidelines**

If you want to contribute or add more functionality to the engine or optimizations feel free to do so. Fork the repository and submit a pull request.


#Here are the repositories of all the used libraries
https://github.com/libsdl-org/SDL

https://github.com/g-truc/glm

https://github.com/assimp/assimp

https://github.com/ocornut/imgui

https://github.com/Dav1dde/glad

