# Nocture Engine
A work-in-progress 3D game engine built from scratch in modern C++.

Nocturne Engine is a learning project exploring low-level game engine design, featuring a custom Entity Component System and OpenGL rendering pipeline. Built to understand how modern game engines work under the hood.

## Current State

https://github.com/user-attachments/assets/d29f2a72-4e67-4705-a75e-093db2eeda7b

## Current Features
### Core Systems:
Custom Entity Component System (ECS) architecture
- Entities and Entity Manager/Factory
- Components and Component Manager/Factory
- Rendering System
- Phong Lighting Model
- Resource Manager

GLFW window management

### Architecture Highlights
The engine follows data-oriented design principles
- Contiguous component storage for cache efficiency
- Separation of data (components) and behavior (systems)

## Building
Make sure to download included submodules with:
```sh
git submodule init
git submodule update
```
Then build with cmake for your preferred C++20 compatible compiler :)
```sh
cmake -B build
cmake --build build
```

### Requirements:
- C++20 compatible compiler
- CMake 3.15+
- OpenGL 3.3+ compatible GPU

Builds both on Arch (GCC and Clang) and Windows 11 on MSVC (VS2022).
