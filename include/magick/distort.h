/*
  Copyright 1999-2008 ImageMagick Studio LLC, a non-profit organization
  dedicated to making software imaging solutions freely available.
  
  You may not use this file except in compliance with the License.
  obtain a copy of the License at
  
    http://www.imagemagick.org/script/license.php
  
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  MagickCore image distortion methods.
*/
#ifndef _MAGICKCORE_DISTORT_H
#define _MAGICKCORE_DISTORT_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <magick/draw.h>

/* These two enum are linked, with common enumeriated values.
 * this because both DistortImages() and SparseColorInterpolate()
 * will often share code for determine functional coefficients for
 * common methods.
 * 
 * Caution should be taken to ensure that only the common methods
 * contain the same enumerated value, while all others remain unique
 * across both enumerations.
 *
 */
typedef enum
{
  UndefinedDistortion,
  AffineDistortion,
  AffineProjectionDistortion,
  ScaleRotateTranslateDistortion,
  PerspectiveDistortion,
  PerspectiveProjectionDistortion,
  BilinearDistortion,
  PolynomialDistortion,
  ArcDistortion,
  PolarDistortion,
  DePolarDistortion,
  BarrelDistortion,
  ShepardsDistortion,
  SentinelDistortion  /* End of methods for DistortImage() */
} DistortImageMethod;

typedef enum
{
  UndefinedColorInterpolate = UndefinedDistortion,
  BarycentricColorInterpolate = AffineDistortion,
  BilinearColorInterpolate = BilinearDistortion,
  PolynomialColorInterpolate = PolynomialDistortion,
  ShepardsColorInterpolate = ShepardsDistortion,
  /* Methods unique to SparseColorInterpolate() from here */
  VoronoiColorInterpolate = SentinelDistortion
} SparseColorInterpolateMethod;

extern MagickExport Image
  *DistortImage(Image *image,const DistortImageMethod,const unsigned long,
    const double *,MagickBooleanType,ExceptionInfo *exception),
  *SparseColorInterpolate(Image *image,const SparseColorInterpolateMethod,
    const unsigned long,const double *,ExceptionInfo *exception);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
