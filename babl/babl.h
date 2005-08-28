/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2005, Øyvind Kolås.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _BABL_H
#define _BABL_H

#if __GNUC__ >=4
   #define BABL_NULL_TERMINATED __attribute_((__sentinel__))
#else
   #define BABL_NULL_TERMINATED
#endif

#include <stdlib.h>
#include "babl-classes.h"
#include "babl-component.h"
#include "babl-conversion.h"
#include "babl-fish.h"
#include "babl-image.h"
#include "babl-model.h"
#include "babl-format.h"
#include "babl-type.h"
#include "babl-sampling.h"

void babl_init       (void);
void babl_destroy    (void);

#endif
