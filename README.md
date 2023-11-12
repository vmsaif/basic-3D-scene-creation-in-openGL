# Basic 3D Scene Creation in OpenGL

[![Hits](https://hits.sh/github.com/vmsaif/basic-3D-scene-creation-in-openGL.svg?label=Visits&color=100b75)](https://hits.sh/github.com/vmsaif/basic-3D-scene-creation-in-openGL/)


This project demonstrates the creation of a 3D scene using OpenGL. The scene encompasses a textured background, composite 3D objects, multiple light sources, and an atmospheric attenuation effect.

## Features

- A textured background.
- Five 3D composite objects, each composed of at least five geometric shapes.
- Two distinct light sources with varying lighting colors.
- An atmospheric attenuation effect, specifically fog.
- Efficient rendering using complex display lists.


## Requirements

### This Repository

Clone this repository into your local machine using the following command:

```bash
git clone https://github.com/vmsaif/basic-3D-scene-creation-in-openGL
```

### Visual Studio

Ensure you have Visual Studio installed. If not, download it from [Visual Studio Official Website](https://visualstudio.microsoft.com/).

### GLUT Library

This project requires the OpenGL library or `glut.h`. The easiest way to set it up in Visual Studio is as follows:

1. Go to the directory where you cloned the project.
2. Open the `basic 3D Scene Creation in OpenGL.sln` file to open the project in Visual Studio.
3. Click on `Project` in the menu bar.
4. Select `Manage NuGet Packages`.(If you don't see it, close the VS window, then resume from step 2.)
5. Click on the "Browse" tab.
6. Search for "freeglut".
7. Select "nupengl.core" (which includes freeGLUT) and click `Install`.(If already installed, then uninstall then install again).

**Note**: This installation will only be available for this project.

## Running the Project

1. Open the project in Visual Studio.
2. Press `F5` or click on `Run Without Debugging`.

## Credits

The background image used in this project was sourced from [Freepik](https://www.freepik.com/free-vector/mountain-background_995152.htm#query=bitmap%20landscape&position=8&from_view=search&track=ais).
