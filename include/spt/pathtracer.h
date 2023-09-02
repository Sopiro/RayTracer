#pragma once

#include "common.h"

#include "bitmap.h"
#include "camera.h"
#include "constant_medium.h"
#include "dielectric.h"
#include "diffuse_light.h"
#include "image_texture.h"
#include "intersectable_pdf.h"
#include "isotropic.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "microfacet.h"
#include "mixture_pdf.h"
#include "model.h"
#include "pdf.h"
#include "postprocess.h"
#include "ray.h"
#include "scene.h"
#include "solid_color.h"
#include "sphere.h"
#include "timer.h"
#include "transform.h"
#include "triangle.h"
#include "util.h"

namespace spt
{

// Unidirectional path tracer
Color PathTrace(const Scene& scene, Ray primary_ray, i32 bounce_count);

} // namespace spt
