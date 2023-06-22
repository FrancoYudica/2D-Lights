
# 2D Lights

It's an offline 2D Light renderer. Uses a mix of Ray Marching and Path tracing techniques to render.

## Features

### Multiple primitives
### Materials
Primitives can have materials, with the following properties

    - Emission
    - Intensity
    - Reflection
    - Refraction
    - Beer Lambert absorption

### Output:

The renderer output an Image object, that holds the 24 bit RGB buffer. Then the user should provide the on_render_callback, and if desired the image buffer can be stored on disk with STB

There is also the possibility of generating image sequences, that when joined make a video
