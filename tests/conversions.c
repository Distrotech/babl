/* perform a symmetricality of conversion test on a set of randomized
 * RGBA data */

#include <stdlib.h>
#include <math.h>
#include "babl-internal.h"

int OK=1;

#define pixels    512
#define TOLERANCE 0.001

#define ERROR_TOLERANCE 0.5

double test[pixels * 4];

static void
test_init (void)
{
  int    i;

  for (i = 0; i < pixels * 4; i++)
     test [i] = (double)random () / RAND_MAX;
}

static void
validate_conversion (BablConversion *conversion)
{
  Babl *fmt_rgba_double;
  Babl *fmt_source;
  Babl *fmt_destination;

  double error=0.0;
  
  void    *source;
  void    *destination;
  double  *destination_rgba_double;
  void    *ref_destination;
  double  *ref_destination_rgba_double;

  fmt_rgba_double = babl_format_new (
       babl_model ("RGBA"),
       babl_type ("double"),
       babl_component ("R"),
       babl_component ("G"),
       babl_component ("B"),
       babl_component ("A"),
       NULL);
  fmt_source      = BABL(conversion->source);
  fmt_destination = BABL(conversion->destination);

  source          = babl_calloc (pixels, fmt_source->format.bytes_per_pixel);
  destination     = babl_calloc (pixels, fmt_destination->format.bytes_per_pixel);
  ref_destination = babl_calloc (pixels, fmt_destination->format.bytes_per_pixel);
  destination_rgba_double     = babl_calloc (pixels, fmt_rgba_double->format.bytes_per_pixel);
  ref_destination_rgba_double = babl_calloc (pixels, fmt_rgba_double->format.bytes_per_pixel);
  
  babl_process (babl_fish_reference (fmt_rgba_double, fmt_source),
      test, source, pixels);
  babl_process (babl_fish_simple (conversion),
      source, destination, pixels);

  babl_process (babl_fish_reference (fmt_source, fmt_destination),
      source, ref_destination, pixels);

  babl_process (babl_fish_reference (fmt_destination, fmt_rgba_double),
      ref_destination, ref_destination_rgba_double, pixels);
  babl_process (babl_fish_reference (fmt_destination, fmt_rgba_double),
      destination, destination_rgba_double, pixels);

  {
    int i;
    int cnt=0;

    for (i=0;i<pixels;i++)
      {
        int j;
        int log=0;
        for (j=0;j<4;j++)
          {
            error += fabs (destination_rgba_double[i*4+j] - 
                            ref_destination_rgba_double[i*4+j]);

           if (fabs (destination_rgba_double[i*4+j] - 
                     ref_destination_rgba_double[i*4+j])>TOLERANCE)
                log=1;
          }
        if (0 && log && cnt < 5)
          {
            /* enabling this code prints out the RGBA double values at various stages,
             * which are used for the average relative error
             */
          
            babl_log ("%s", conversion->instance.name);
            babl_log ("\ttest:           %2.5f %2.5f %2.5f %2.5f", test [i*4+0],
                                                                   test [i*4+1],
                                                                   test [i*4+2],
                                                                   test [i*4+3]);
            babl_log ("\tconversion:     %2.5f %2.5f %2.5f %2.5f", destination_rgba_double [i*4+0],
                                                                   destination_rgba_double [i*4+1],
                                                                   destination_rgba_double [i*4+2],
                                                                   destination_rgba_double [i*4+3]);
            babl_log ("\tref_conversion: %2.5f %2.5f %2.5f %2.5f", ref_destination_rgba_double [i*4+0],
                                                                   ref_destination_rgba_double [i*4+1],
                                                                   ref_destination_rgba_double [i*4+2],
                                                                   ref_destination_rgba_double [i*4+3]);
            cnt++;
            OK=0;
          }
      }
     error /= pixels;
     error *= 100;

     conversion->error = error;
     if (error >= ERROR_TOLERANCE)
       {
          babl_log ("%s\terror:%f", conversion->instance.name, error);
          OK = 0;
       }
  }

  
  babl_free (source);
  babl_free (destination);
  babl_free (destination_rgba_double);
  babl_free (ref_destination);
  babl_free (ref_destination_rgba_double);
}

static int
each_conversion (Babl *babl,
                 void *userdata)
{
  Babl *source = BABL(babl->conversion.source);
  Babl *destination = BABL(babl->conversion.destination);

  if (source->instance.id      != BABL_RGBA   &&
      destination->instance.id != BABL_RGBA   &&
      source->instance.id      != BABL_DOUBLE &&
      destination->instance.id != BABL_DOUBLE &&
      source->class_type       == BABL_FORMAT &&
      destination->class_type  == BABL_FORMAT)
  {
    validate_conversion ((BablConversion*)babl);
  }
  return 0;
}

int main (void)
{
  babl_init ();
  test_init ();

  babl_set_extender (babl_extension_quiet_log ());
  babl_conversion_each (each_conversion, NULL);

  babl_destroy ();

  return !OK;
}
