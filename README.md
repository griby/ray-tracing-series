# ray-tracing-series
A ray tracer implementation based on the **Ray Tracing in One Weekend Book Series** by Peter Shirley - https://raytracing.github.io/

![alt text](output/v1.0_example_1.jpg)

More examples are available [here](#examples).

## Overview
This repository features a C++11 implementation of a simple ray tracer based on the contents of the book **Ray Tracing in One Weekend**. There are no external dependencies. The two other books of the series haven't been covered yet: **Ray Tracing the Next Week** and **Ray Tracing the Rest of Your Life**.

It covers the following concepts:
 * Sphere shape, the only available shape (see [sphere.h](ray-tracing-series/src/sphere.h))
 * Diffuse material with an albedo, it is one of the three available materials (see [lambertian.h](ray-tracing-series/src/lambertian.h))
 * Metallic material with an albedo and a fuzz factor (see [metal.h](ray-tracing-series/src/metal.h))
 * Dielectric/glass material with an albedo and a refraction index (see [dielectric.h](ray-tracing-series/src/dielectric.h))
 * Camera with a lookFrom/lookAt, FOV, focus distance and aperture (see [camera.h](ray-tracing-series/src/camera.h))

The execution follows three main steps (see [main.cpp](ray-tracing-series/src/main.cpp) > *main()*):
 1. Setting up the world
 2. Performing ray tracing
 3. Writing the image file

The first step generates a world with one giant sphere for the ground, 3 bigger spheres in the center (each one of a different material) and approximately 500 smaller spheres with a random mix of materials. It also sets up the camera.

The second step performs the ray tracing. At the moment the implementation is CPU-based but it is fully multithreaded. For that, a number of async tasks are launched, each responsible for ray tracing a certain number of lines of the resulting image.

The third and final step simply outputs the image buffer to a PPM file.

## Observations

More detailed comments have been added to the following parts of the algorithm:
 * Ray/Sphere intersection (see [sphere.cpp](ray-tracing-series/src/sphere.cpp) > *rts::Sphere::hit* comments at the bottom of the file)
 * Vector refraction (see [utils.cpp](ray-tracing-series/src/utils.cpp) > *rts::getRefractedVector*) based on [the following post](http://psgraphics.blogspot.com/2015/06/ray-tracing-refraction.html)
 * Dielectric scattering (see [dielectric.cpp](ray-tracing-series/src/dielectric.cpp) > *rts::Dielectric::scatter*)

It's important to note that the dielectric scattering as described in **Chapter 9: Dielectrics** of the book is incorrect. More precisely, the computation of the cosine parameter which is passed to Schlick's approximation. The correct one can be found in [the following post](http://psgraphics.blogspot.com/2016/03/my-buggy-implimentation-of-schlick.html) from Peter Shirley.

The implementation suggested by this post can lead to *NaNs* caused by a *sqrt(negative_value)*. It's not easily noticeable since the visual impact is somewhat limited. And the *NaN* value doesn't propagate to the rest of the ray tracing algorithm since this value is only used locally to determine whether the ray is refracted or reflected. Either way, in the case of a negative value there's total internal reflection and the ray should simply be reflected.

## Configuration

A number of defines and constants can be adjusted to configure the execution.

The following defines can be added to the *Preprocessor Definitions* (see [defines.h](ray-tracing-series/src/defines.h)):
 * MULTITHREADING_ON: to activate the multithreading support
 * DETERMINISTIC_RNG: to render identical images given the same input (fixed random seeds even when multithreaded)
 * RENDER_NORMAL_MAP: to render the normal map of the scene (a ray is cast to get the normal but it isn't scattered)
 * RENDER_NO_MATERIAL: to render the image ignoring the objects material (the rays bounce with a simple reflection)
 * RENDER_GRAYSCALE: to render the grayscale image of the scene

The following constants can also be changed (see [config.h](ray-tracing-series/src/config.h)):
 * IMAGE_WIDTH / IMAGE_HEIGHT: the image resolution
 * IMAGE_GAMMA_CORRECTION: the gamma correction to apply
 * CAMERA_FOV: the camera field of view
 * RAY_COUNT_PER_PIXEL: the number of rays traced to generate a single pixel
 * RAY_DEPTH_MAX: the maximum of times a ray gets to bounce before it stops being scattered
 * MULTITHREADING_SUBTASK_COUNT: the number of tasks spawned by the ray tracing main task

There's more but these are the main ones.

## Examples

Those output examples have been generated with the following configuration:
 * IMAGE_WIDTH / IMAGE_HEIGHT = 800 x 600
 * IMAGE_GAMMA_CORRECTION = 2
 * CAMERA_FOV = 90
 * RAY_COUNT_PER_PIXEL = 150
 * RAY_DEPTH_MAX = 20
 * a camera aperture of 1/50th

Each run took approximately 130 seconds to generate on the 8 threads of an Intel Core i7-6700K Quad Core 4GHz processor.

Those images all feature the setup mentioned previously except for a few differences.

![alt text](output/v1.0_example_1.jpg)

In this first example (see above), the bigger sphere in the foreground is made out of smooth metal (metal with fuzz factor = 0), while some of the smaller spheres are made out of brushed metal (fuzz factor > 0).

![alt text](output/v1.0_example_2.jpg)

In this second example (see above), the bigger glass sphere in the center is tainted green and more of the smaller spheres are made out of glass (dielectric with refraction index = 1.5).

Those glass spheres appear to be "floating" because light simply passes through them, so they cast no shadow. Also it's interesting to notice how the refraction flips the image of what's behind them.

![alt text](output/v1.0_example_3.jpg)

In this third example (see above), the background color has been changed to a darker blue (see [config.h](ray-tracing-series/src/config.h) > WORLD_BACKGROUND_COLOR_*. This affects the lighting of the entire scene.

![alt text](output/v1.0_example_4.jpg)

In this fourth example (see above), more spheres are made out of smooth metal which gives the opportunity for more recursive reflections.

## License

The content of this repository is licensed under the MIT License (see [LICENSE](LICENSE)).
