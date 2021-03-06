/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2012, Øyvind Kolås
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <stdint.h>
#include <stdlib.h>

#include "babl.h"
#include "babl-cpuaccel.h"
#include "extensions/util.h"
#include "base/util.h"

#define INLINE inline


static INLINE long
conv_rgbaF_linear_rgbAF_gamma (unsigned char *src, 
                               unsigned char *dst, 
                               long           samples)
{
   float *fsrc = (float *) src;
   float *fdst = (float *) dst;
   int n = samples;

   while (n--)
     {
       float alpha = fsrc[3];
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++) * alpha;
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++) * alpha;
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++) * alpha;
       *fdst++ = *fsrc++;
     }
  return samples;
}

static INLINE long
conv_rgbAF_linear_rgbAF_gamma (unsigned char *src, 
                               unsigned char *dst, 
                               long           samples)
{
   float *fsrc = (float *) src;
   float *fdst = (float *) dst;
   int n = samples;

   while (n--)
     {
       float alpha = fsrc[3];
       if (alpha < BABL_ALPHA_THRESHOLD)
         {
           *fdst++ = 0.0;
           *fdst++ = 0.0;
           *fdst++ = 0.0;
           *fdst++ = 0.0;
           fsrc+=4;
         }
       else if (alpha >= 1.0)
         {
           *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
           *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
           *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
           *fdst++ = *fsrc++;
         }
       else
         {
           float alpha_recip = 1.0 / alpha;
           *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++ * alpha_recip) * alpha;
           *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++ * alpha_recip) * alpha;
           *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++ * alpha_recip) * alpha;
           *fdst++ = *fsrc++;
         }
     }
  return samples;
}

static INLINE long
conv_rgbaF_linear_rgbaF_gamma (unsigned char *src, 
                               unsigned char *dst, 
                               long           samples)
{
   float *fsrc = (float *) src;
   float *fdst = (float *) dst;
   int n = samples;

   while (n--)
     {
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
       *fdst++ = *fsrc++;
     }
  return samples;
}

static INLINE long
conv_rgbF_linear_rgbF_gamma (unsigned char *src, 
                             unsigned char *dst, 
                             long           samples)
{
   float *fsrc = (float *) src;
   float *fdst = (float *) dst;
   int n = samples;

   while (n--)
     {
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
       *fdst++ = babl_linear_to_gamma_2_2 (*fsrc++);
     }
  return samples;
}


static INLINE long
conv_rgbaF_gamma_rgbaF_linear (unsigned char *src, 
                               unsigned char *dst, 
                               long           samples)
{
   float *fsrc = (float *) src;
   float *fdst = (float *) dst;
   int n = samples;

   while (n--)
     {
       *fdst++ = babl_gamma_2_2_to_linear (*fsrc++);
       *fdst++ = babl_gamma_2_2_to_linear (*fsrc++);
       *fdst++ = babl_gamma_2_2_to_linear (*fsrc++);
       *fdst++ = *fsrc++;
     }
  return samples;
}

static INLINE long
conv_rgbF_gamma_rgbF_linear (unsigned char *src, 
                             unsigned char *dst, 
                             long           samples)
{
   float *fsrc = (float *) src;
   float *fdst = (float *) dst;
   int n = samples;

   while (n--)
     {
       *fdst++ = babl_gamma_2_2_to_linear (*fsrc++);
       *fdst++ = babl_gamma_2_2_to_linear (*fsrc++);
       *fdst++ = babl_gamma_2_2_to_linear (*fsrc++);
     }
  return samples;
}

#define o(src, dst) \
  babl_conversion_new (src, dst, "linear", conv_ ## src ## _ ## dst, NULL)

int init (void);

int
init (void)
{
  const Babl *rgbaF_linear = babl_format_new (
    babl_model ("RGBA"),
    babl_type ("float"),
    babl_component ("R"),
    babl_component ("G"),
    babl_component ("B"),
    babl_component ("A"),
    NULL);
  const Babl *rgbAF_linear = babl_format_new (
    babl_model ("RaGaBaA"),
    babl_type ("float"),
    babl_component ("Ra"),
    babl_component ("Ga"),
    babl_component ("Ba"),
    babl_component ("A"),
    NULL);
  const Babl *rgbaF_gamma = babl_format_new (
    babl_model ("R'G'B'A"),
    babl_type ("float"),
    babl_component ("R'"),
    babl_component ("G'"),
    babl_component ("B'"),
    babl_component ("A"),
    NULL);
  const Babl *rgbAF_gamma = babl_format_new (
    babl_model ("R'aG'aB'aA"),
    babl_type ("float"),
    babl_component ("R'a"),
    babl_component ("G'a"),
    babl_component ("B'a"),
    babl_component ("A"),
    NULL);
  const Babl *rgbF_linear = babl_format_new (
    babl_model ("RGB"),
    babl_type ("float"),
    babl_component ("R"),
    babl_component ("G"),
    babl_component ("B"),
    NULL);
  const Babl *rgbF_gamma = babl_format_new (
    babl_model ("R'G'B'"),
    babl_type ("float"),
    babl_component ("R'"),
    babl_component ("G'"),
    babl_component ("B'"),
    NULL);

  o (rgbAF_linear, rgbAF_gamma);
  o (rgbaF_linear, rgbAF_gamma);
  o (rgbaF_linear, rgbaF_gamma);
  o (rgbaF_gamma,  rgbaF_linear);
  o (rgbF_linear, rgbF_gamma);
  o (rgbF_gamma,  rgbF_linear);

  return 0;
}

