## Overview

This repository implements a **basic ray tracer** designed for 3D scene rendering. A ray tracer works by simulating rays of light cast from a camera into a virtual scene, calculating how these rays interact with objects, and producing a final image based on these interactions. This implementation focuses on simplicity and is ideal for understanding the foundational principles of ray tracing.

### Key Features

- **Ray Tracing Core**: Simulates the interaction of light rays with objects in a 3D environment. This includes detecting intersections between rays and objects in the environment.
  
- **Dynamic Resizing**: Supports real-time viewport resizing, recalculating the aspect ratio and allocating memory for rendering at different resolutions.
  
- **Basic Lighting Model**: Implements simple directional lighting to give objects depth and realism. The shading is calculated based on the object's surface normals and light direction, resulting in a visually appealing specular highlight effect. (Much more is in development)
  
### Purpose

This project is designed for:
- **Learning and experimentation**: A minimalistic implementation that avoids unnecessary complexity, making it suitable for developers new to ray tracing or computer graphics.
- **Expanding capabilities**: Provides a solid foundation for building more advanced features like shadows, reflections, and additional geometries.

### How It Works

1. **Scene Setup**: A collection of spheres with positions, radii, and material properties define the objects in the scene.
2. **Camera Rays**: Rays are cast from a camera's position through each pixel in the viewport.
3. **Ray-Sphere Intersection**: Each ray is tested for intersections with the spheres in the scene, and the closest intersection determines the pixel's color.
4. **Lighting Calculation**: Using the object's surface normals and a fixed directional light, the renderer calculates shading for each pixel.
5. **Image Output**: The final pixel colors are written to an image buffer, which is displayed to the screen.

This project is an excellent starting point for exploring ray tracing and can be extended with additional features to support more realistic rendering techniques.
