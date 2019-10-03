#include "Renderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

HOST DEVICE Pixel::Pixel(const int _x, const int _y):
    x(_x), y(_y) {
}

HOST RenderTarget::RenderTarget(
    const Scene *_scene, const CameraBase *_camera, const std::string &_path):
    scene(_scene),
    camera(_camera),
    path(_path) {
}

HOST RenderOptions::RenderOptions(
    const int _samples, const int _bounces, const Vector3 &_ambient):
    samples(_samples),
    bounces(_bounces),
    ambient(_ambient) {
}

HOST DEVICE void ComputeRay(
    const Ray &_ray,
    const Scene *_scene,
    const int _bounce,
    const Vector3 &_ambient,
    RandomGenerator *_generator,
    Vector3 &_radiance,
    Scalar &_pdf) {
  _radiance = Vector3(0, 0, 0);
  Intersection<ObjectBase> intersection;
  if (_scene->RayIntersect(_ray, intersection)) {
    const MaterialBase *material = intersection.element->Material();
    _radiance += material->Emission(-_ray.direction, intersection.normal);
    if (_bounce > 0 && _pdf > SCALAR_LOOSE_DELTA) {
      Vector3 sample_direction;
      Scalar sample_pdf;
      if (material->Sample(_generator, _ray.direction, intersection.normal, sample_direction, sample_pdf)) {
        _pdf *= sample_pdf;
        Ray next_ray(intersection.position, sample_direction);
        Vector3 sample_radiance;
        ComputeRay(next_ray, _scene, _bounce - 1, _ambient, _generator, sample_radiance, _pdf);
        _radiance += sample_radiance * material->BSDF(
            _ray.direction,
            _ray.Distance(intersection.position),
            next_ray.direction,
            intersection.normal);
      }
    }
  } else {
    _radiance = _ambient;
  }
}

HOST void HostRender(const RenderTarget &_target, const RenderOptions &_options) {
  RandomGenerator generator(1);
  const int resolution_x = _target.camera->ResolutionX();
  const int resolution_y = _target.camera->ResolutionY();
  const Scalar min_pdf = Scalar(1) / Scalar(_options.samples);
  std::vector<unsigned char> pixels(resolution_x * resolution_y * 3);
  for (int xi = 0; xi < resolution_x; ++ xi) {
    std::cout << "x = " << xi << std::endl;
    for (int yi = 0; yi < resolution_y; ++ yi) {
      Vector3 total_weighted_radiance = Vector3(0, 0, 0);
      Scalar total_inverse_pdf = 0;
      for (int i = 0; i < _options.samples; ++ i) {
        Scalar x = xi + generator.NextRange(0, 1);
        Scalar y = yi + generator.NextRange(0, 1);
        Ray camera_ray = _target.camera->Unproject(Vector2(x, y));
        Vector3 radiance;
        Scalar pdf = 1;
        ComputeRay(
          camera_ray, _target.scene, _options.bounces, _options.ambient, &generator, radiance, pdf);
        pdf = (1 - min_pdf) * pdf + min_pdf;
        total_weighted_radiance += (radiance / pdf);
        total_inverse_pdf += (Scalar(1) / pdf);
      }
      Vector3 pixel_radiance = total_weighted_radiance / total_inverse_pdf;
      Vector3 pixel_color = pixel_radiance / (pixel_radiance + Vector3(1, 1, 1));
      int index = (xi + yi * resolution_x) * 3;
      pixels[index + 0] = (unsigned char)(pixel_color.x * 255);
      pixels[index + 1] = (unsigned char)(pixel_color.y * 255);
      pixels[index + 2] = (unsigned char)(pixel_color.z * 255);
    }
  }
  stbi_write_png(_target.path.data(), resolution_x, resolution_y, 3, pixels.data(), resolution_x * 3);
}

GLOBAL void DepthKernel(const Scene *_scene, const CameraBase *_camera, const int _offset_x, const int _offset_y, unsigned char *_pixels) {
  int resolution_x = _camera->ResolutionX();
  int resolution_y = _camera->ResolutionY();
  int x = blockIdx.x + _offset_x;
  int y = threadIdx.x + _offset_y;
  Ray ray = _camera->Unproject(Vector2(x, y));
  Intersection<ObjectBase> intersection;
  bool intersect = _scene->RayIntersect(ray, intersection);
  if (intersect) {
    // Scalar depth = _camera->Depth(intersection.position);
    // unsigned char gray = (unsigned char)(depth / (depth + 1) * 255);
    Vector3 color = (intersection.normal + Vector3(1, 1, 1)) / 2;
    int index = (x + resolution_x * y) * 3;
    _pixels[index + 0] = (unsigned char)(color.x * 255);
    _pixels[index + 1] = (unsigned char)(color.y * 255);
    _pixels[index + 2] = (unsigned char)(color.z * 255);
  }
}

HOST void DeviceRender(const RenderTarget &_target, const RenderOptions &_options) {
  const int resolution_x = _target.camera->ResolutionX();
  const int resolution_y = _target.camera->ResolutionY();
  // Vector<RandomGenerator> generators(resolution_x * resolution_y);
  // for (int i = 0; i < generators.Size(); ++ i) {
  //   generators[i].SetSeed(i + 1);
  // }
  Vector<unsigned char> pixels(resolution_x * resolution_y * 3, (unsigned char)0);
  // DepthKernel<<<resolution_x, resolution_y>>>(_target.scene, _target.camera, pixels.Data());
  std::cout << resolution_x << ' ' << resolution_y << '\n';
  for (int tx = 0; tx < 4; ++ tx) {
    for (int ty = 0; ty < 4; ++ ty) {
      std::cout << tx << ' ' << ty << '\n';
      DepthKernel<<<100, 100>>>(_target.scene, _target.camera, 100 * tx, 100 * ty, pixels.Data());
      cudaDeviceSynchronize();
    }
  }
  stbi_write_png(_target.path.data(), resolution_x, resolution_y, 3, pixels.Data(), resolution_x * 3);
}

