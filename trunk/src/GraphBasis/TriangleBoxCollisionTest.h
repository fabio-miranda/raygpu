#ifndef TriangleBoxCollisionTest_H
#define TriangleBoxCollisionTest_H

#include <math.h>
#include <stdio.h>

extern "C" {
	int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]);
}

#endif