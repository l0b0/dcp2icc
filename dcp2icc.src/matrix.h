#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED
#include "dng_matrix.h"
#include "dng_hue_sat_map.h"

dng_matrix Interpolate(double t1, double t2, double t,
		       const dng_matrix &m1, const dng_matrix &m2);
dng_hue_sat_map Interpolate(double t1, double t2, double t,
		       const dng_hue_sat_map &m1, const dng_hue_sat_map &m2);
void PrintMatrix(const char *title, const dng_matrix &m);
void PrintMatrix(const char *title, const dng_vector &v);
/* Возвращает 1, если матрицы равны, иначе - 0 */
int equal(const dng_matrix &m1, const dng_matrix &m2);
#endif
