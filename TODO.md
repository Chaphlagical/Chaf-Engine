# TODO

## Feature

Platform: Windows

API: OpenGL

Build: premake, ~~cmake~~ 

- [ ] Component
	- [ ] Camera
	- [ ] Transform
	- [ ] Material
	- [ ] Geometry
		- [ ] Sphere
		- [ ] Plane
		- [ ] Cube
	- [ ] Light
		- [ ] Point Light
		- [ ] Rectangle Light
		- [ ] Spot Light
		- [ ] Directional Light
- [ ] Offline Rendering
	- [ ] Path Tracing
- [ ] Digital Geometry Processing
	- [ ] Parameterization
	- [ ] Minimum Surface
	- [ ] Loop Subdivision
- [ ] Physics Simulation System

Continuing updating...

## Structure

### Renderer

```mermaid
graph TD
OpenGL --> Interface
DirectX --> Interface
Vulkan --> Interface
Interface --> Texture
Interface --> VertexArray
Interface --> Shader
Interface --> Buffer
Interface --> Command
Buffer --> VertexBuffer
Buffer --> IndexBuffer
Buffer --> FrameBuffer
Texture --> Texture2D
Texture --> Texture3D
```

### Engine

* Logging System
* Profile System
* Event System
* File System
* Layer System

### Scene

* Entity Component System

### Editor

* UI

