# Chaf Engine

My toy engine, including real-time rendering, offline rendering and scene editing

## What you need?

* **Visual Studio 2019** (Haven't tested on other version or platform)

## How to download?

The Engine is only support Windows for now, you can use git command:

```shell
git clone --recursive https://github.com/Chaphlagical/ChafEngine.git
```

Download release executable file from [https://github.com/premake/premake-core/releases](https://github.com/premake/premake-core/releases) and place `premake5.exe` into `/projects`

Run the batch script `projects/Win-GenerateProject.bat`, then open the Visual Studio Project `CEngine.sln`

If you trying to run the `.exe` in `/bin`, you should copy the asserts from `/src/App`

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

**Future:**

- [ ] Digital Geometry Process
- [ ] Offline Rendering
- [ ] Physics Simulation

## Demo

**UI**

![](https://chaphlagical.github.io/external/CG_Demo/ChafEngine/UI.png)

**WireFrame**

![](https://chaphlagical.github.io/external/CG_Demo/ChafEngine/wireframe.png)

**Phong Light Model**

![](https://chaphlagical.github.io/external/CG_Demo/ChafEngine/Phong.png)

**Texture**

![](https://chaphlagical.github.io/external/CG_Demo/ChafEngine/texture.png)

![](https://chaphlagical.github.io/external/CG_Demo/ChafEngine/texture1.png)

**PBR**

![](https://chaphlagical.github.io/external/CG_Demo/ChafEngine/pbr.png)

