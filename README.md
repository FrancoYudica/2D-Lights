
# 2D Lights

It's an offline 2D Light renderer. Uses a mix of Ray Marching and Path tracing techniques to render.

![ ](renders/samples/metaballs0.png)

# Features
## Shapes
The renderer uses ray marching, that's why it can render multiple shapes:

    - Circle
    - Box
    - Line
    - Plane
    - Pentagon
    - Arc
    - Heart
    - Egg

## Materials

Materials determine how an object interacts with light.

- ***Emission and intensity***
- ***Reflectance***: Schlick's approximation of Fresnel equations
- ***Refraction***: Snell's law and total internal reflection
- ***Absorption***: Beer Lambert

## Outputs

The renderer outputs an Image object, that holds the 24 bit RGB buffer. Then on_render_callback, stores *.png* images using STB

There is also the possibility of generating image sequences, that when joined make a video

## Goal
The main goal of this project was to learn about PBR (Physically Based Rendering) in a simple environment, where no GPU and 3D graphics is required. 

## Samples

![ ](renders/video/metaballs/metaballs.mp4)


### Solid objects

![ ](renders/samples/light_intensity_interpolation.png)
![ ](renders/samples/color_interpolation.png)
![ ](renders/samples/metaballs1.png)

#### Caustics effect

![ ](renders/samples/caustics_reflective-100spp.png)

### Lens

Circular:

![ ](renders/samples/circular_lens.png)

Convex:

![ ](renders/samples/convex_lens.png)

Concave:

![ ](renders/samples/concave_lens.png)

Semicircular:

![ ](renders/samples/semicircular_lens.png)

### Glass

![ ](renders/samples/glass_metaballs.png)

### Light absorption

![ ](renders/samples/metaballs_absorption.png)

![ ](renders/samples/glass_absorption.png)


