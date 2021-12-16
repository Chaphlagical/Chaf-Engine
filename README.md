# Chaf Engine

My toy engine

## Updated

I am currently working on a new renderer [IlumEngine](https://github.com/Chaf-Libraries/IlumEngine), so I may not add any new features to this renderer any longer.

But I still use `Chaf-Engine` for my homework, check:

[Computer Aided Geometric Design](https://github.com/Chaphlagical/CAGD)

[Digital Image Process](https://github.com/Chaphlagical/DIP)

## Platform

* **Windows 10**

* **Visual Studio 2019** (Haven't tested on other version or platform)

## Installation

Clone the repo

```shell
git clone --recursive https://github.com/Chaphlagical/Chaf-Engine.git
```

Make sure you get CMake. Then run:

```shell
mkdir build
cd build
cmake ..
cmake --build ./
```

## Feature

* Build: Premake5
* Platform: Windows 10
* Graphics API: OpenGL

- [x] Component
	- [x] Transform
	- [x] Material
		- [x] Emisson
		- [x] Phong Lighting Model
		- [x] Cook Torrance PBR Model
	- [x] Geometry
		- [x] Sphere
		- [x] Plane
		- [x] Cube
		- [x] Load .obj from File
	- [x] Light Source
		- [x] Ideal Light
		- [x] Directional Light
		- [x] Point Light
		- [x] Spot Light

## Demo

**UI**

![](./images/ui.png)

**WireFrame**

![](./images/wireframe.png)

**Phong Light Model**

![](./images/phong.png)

**PBR**

![](./images/pbr.png)

