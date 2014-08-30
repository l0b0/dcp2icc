#include <iostream>
#include "dng_camera_profile.h"
#include "dng_matrix.h"
#include "dng_hue_sat_map.h"
#include "matrix.h"

dng_matrix Interpolate(double t1, double t2, double t,
		       const dng_matrix &m1, const dng_matrix &m2)
{
  double r1, r2, r;
  double t0;
  double g;
  int inv;
  int i, j;
  if(m1.IsEmpty())
    return m2;
  if(m2.IsEmpty())
    return m1;
  if((m1.Rows() != m2.Rows()) || (m1.Cols() != m2.Cols())) {
    ThrowMatrixMath("Interpolate of matrices of differerent sizes");
  }
  if(t1 <= 1.0)
    return m2;
  if(t2 <= 1.0)
    return m1;
  inv=0;
  if(t2 < t1) {
    t0=t2;
    t2=t1;
    t1=t0;
    inv=1;
  }
  if(t <= t1) {
    if(inv)
      return m2;
    else
      return m1;
  }
  if(t >= t2) {
    if(inv)
      return m1;
    else
      return m2;
  }
  r1=1.0/t1;
  r2=1.0/t2;
  r=1.0/t;
  g=(r2-r)/(r2-r1);
  if(inv)
    g=1.0-g;
  dng_matrix m(m1.Rows(), m1.Cols());
  for(i=0; i<m.Rows(); i++) {
    for(j=0; j<m.Cols(); j++) {
      m[i][j]=m1[i][j]*g+m2[i][j]*(1.0-g);
    }
  }
  m.Round(10000);
  return m;
}

void PrintMatrix(const char *title, const dng_matrix &m)
{
  int i, j;
  if(title) {
    std::cout << title << "=";
  }
  for(i=0; i<m.Rows(); i++) {
    for(j=0; j<m.Cols(); j++) {
      std::cout << " " << m[i][j];
    }
    std::cout << std::endl;
  }
  if(!m.Rows())
    std::cout << std::endl;
}

void PrintMatrix(const char *title, const dng_vector &v)
{
  int i;
  if(title) {
    std::cout << title << "=";
  }
  for(i=0; i<v.Count(); i++) {
    std::cout << " " << v[i];
  }
  std::cout << std::endl;
}

/* Возвращает 1, если матрицы равны, иначе - 0 */
int equal(const dng_matrix &m1, const dng_matrix &m2)
{
  int i,j;
  if(m1.Rows() != m2.Rows())
    return 0;
  if(m1.Cols() != m2.Cols())
    return 0;
  for(i=0; i<m1.Rows(); i++) {
    for(j=0; j<m1.Cols(); j++) {
      if(fabs(m1[i][j]-m2[i][j]) > 0.00005)
	return 0;
    }
  }
  return 1;
}
dng_hue_sat_map Interpolate(double t1, double t2, double t,
		       const dng_hue_sat_map &m1, const dng_hue_sat_map &m2)
{
  double r1, r2, r;
  double t0;
  double g;
  int inv;
  if(m1.IsNull())
    return m2;
  if(m2.IsNull())
    return m1;
  if(t1 <= 1.0)
    return m2;
  if(t2 <= 1.0)
    return m1;
  inv=0;
  if(t2 < t1) {
    t0=t2;
    t2=t1;
    t1=t0;
    inv=1;
  }
  if(t <= t1) {
    if(inv)
      return m2;
    else
      return m1;
  }
  if(t >= t2) {
    if(inv)
      return m1;
    else
      return m2;
  }
  r1=1.0/t1;
  r2=1.0/t2;
  r=1.0/t;
  g=(r2-r)/(r2-r1);
  if(inv)
    g=1.0-g;
  return *dng_hue_sat_map::Interpolate(m1,m2,g);
}
