#pragma once

#include "CameraBase.h"
#include "Scene.h"

class Pixel {
public:
  HOST DEVICE Pixel(const int _x = 0, const int _y = 0);
  int x;
  int y;
};

class RenderTarget {
public:
  HOST RenderTarget(const Scene *_scene, const CameraBase *_camera, const std::string &_path);
  const Scene *scene;
  const CameraBase *camera;
  const std::string path;
};

class RenderOptions {
public:
  HOST RenderOptions(
    const int _samples = 16,
    const int _bounces = 16,
    const Vector3 &_ambient = Vector3(0.1, 0.1, 0.1));
  int samples;
  int bounces;
  Vector3 ambient;
};

HOST DEVICE void ComputeRay(
  const Ray &_ray,
  const Scene *_scene,
  const int _bounce,
  const Vector3 &_ambient,
  RandomGenerator *_generator,
  Vector3 &_radiance,
  Scalar &_pdf);

HOST void HostRender(const RenderTarget &_target, const RenderOptions &_options);

// GLOBAL void DeviceRenderTileKernel(const RenderTarget _target, const RenderOptions _options, );

GLOBAL void DepthKernel(const Scene *_scene, const CameraBase *_camera, unsigned char *_pixels);

HOST void DeviceRender(const RenderTarget &_target, const RenderOptions &_options);
