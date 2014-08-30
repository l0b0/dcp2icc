#include "dng_color_space.h"
#include "dng_memory.h"
#include "dng_bottlenecks.h"
#include "dng_spline.h"
#include "dng_render.h"
#include "matrix.h"
#include "sampler.h"

Sampler::Sampler(const dng_matrix &nfm)
{
  fm=nfm;
  cam2rgb_matrix=dng_space_ProPhoto::Get().MatrixFromPCS()*fm;
  rgb2xyz_matrix=dng_space_ProPhoto::Get().MatrixToPCS();
  tc.Initialize(gDefaultDNGMemoryAllocator, dng_tone_curve_acr3_default::Get());
  maxrgb=1.0;
  maxxyz=0.5;
}

void Sampler::setfm(const dng_matrix &nfm)
{
  if(!equal(fm,nfm)) {
    fm=nfm;
    cam2rgb_matrix=dng_space_ProPhoto::Get().MatrixFromPCS()*fm;
  }
}

dng_matrix Sampler::getfm()
{
  return fm;
}

void Sampler::sethsm(const dng_hue_sat_map &nhsm)
{
  hsm=nhsm;
}

void Sampler::setltd(const dng_hue_sat_map &nltd)
{
  ltd=nltd;
}

void Sampler::settc(const dng_tone_curve &ntc)
{
  dng_spline_solver ptc;
  ntc.Solve(ptc);
  tc.Initialize(gDefaultDNGMemoryAllocator,ptc);
}

dng_vector Sampler::cam2rgb(const dng_vector &cam, int test)
{
  dng_vector rgb;
  rgb = cam2rgb_matrix * cam;
  if(test) {
    if(rgb[0] > maxrgb)
      maxrgb=rgb[0];
    if(rgb[1] > maxrgb)
      maxrgb=rgb[1];
    if(rgb[2] > maxrgb)
      maxrgb=rgb[2];
  } else {
    rgb[0] /= maxrgb;
    rgb[1] /= maxrgb;
    rgb[2] /= maxrgb;
  }
  return rgb;
}

dng_vector Sampler::cam2xyz(const dng_vector &cam, int test)
{
  dng_vector rgb;
  dng_vector xyz;
  real32 r,g,b,r1,g1,b1;
  rgb = cam2rgb_matrix * cam;
  rgb[0] /= maxrgb;
  rgb[1] /= maxrgb;
  rgb[2] /= maxrgb;
  if(hsm.IsValid()) {
    r1=r=rgb[0];
    g1=g=rgb[1];
    b1=b=rgb[2];
    DoBaselineHueSatMap(&r,&g,&b,&r1,&g1,&b1,1,hsm);
    rgb[0]=r1;
    rgb[1]=g1;
    rgb[2]=b1;
  }
  if(ltd.IsValid()) {
    r1=r=rgb[0];
    g1=g=rgb[1];
    b1=b=rgb[2];
    DoBaselineHueSatMap(&r,&g,&b,&r1,&g1,&b1,1,ltd);
    rgb[0]=r1;
    rgb[1]=g1;
    rgb[2]=b1;
  }
  if(0) {
    /* Don't use tone curve */
    r=rgb[0];
    g=rgb[1];
    b=rgb[2];
    if(r < 0.0)
      r=0.0;
    if(r > 1.0)
      r=1.0;
    if(g < 0.0)
      g=0.0;
    if(g > 1.0)
      g=1.0;
    if(b < 0.0)
      b=0.0;
    if(b > 1.0)
      b=1.0;
    DoBaselineRGBTone(&r,&g,&b,&r1,&g1,&b1,1,tc);
    rgb[0]=r1;
    rgb[1]=g1;
    rgb[2]=b1;
  }
  rgb[0] *= maxrgb/(1.0+32767.0/32768.0);
  rgb[1] *= maxrgb/(1.0+32767.0/32768.0);
  rgb[2] *= maxrgb/(1.0+32767.0/32768.0);
  xyz=rgb2xyz_matrix * rgb;
  if(test) {
    if(xyz[1]>maxxyz)
      maxxyz=xyz[1];
  }
  return xyz;
}

static WORD f2w(double x)
{
  long dw;
  dw=x*(65535.0)+0.5;
  if(dw < 0)
    dw=0;
  if(dw > 0xffff)
    dw=0xffff;
  return dw;
}

static double w2f(WORD x)
{
  return x/65535.0;
}

int dcpSampler(register WORD in[],
	       register WORD out[],
	       register LPVOID cargo)
{
  Sampler *smp;
  dng_vector fin(3), fout;
  smp=(Sampler *)cargo;
  fin[0]=w2f(in[0]);
  fin[1]=w2f(in[1]);
  fin[2]=w2f(in[2]);
  fout=smp->cam2xyz(fin,0);
  out[0]=f2w(fout[0]);
  out[1]=f2w(fout[1]);
  out[2]=f2w(fout[2]);
  return TRUE;
}

int dcpTest1(register WORD in[],
	     register WORD out[],
	     register LPVOID cargo)
{
  Sampler *smp;
  dng_vector fin(3), fout;
  smp=(Sampler *)cargo;
  fin[0]=w2f(in[0]);
  fin[1]=w2f(in[1]);
  fin[2]=w2f(in[2]);
  fout=smp->cam2rgb(fin,1);
  out[0]=f2w(fout[0]);
  out[1]=f2w(fout[1]);
  out[2]=f2w(fout[2]);
  return TRUE;
}

int dcpTest2(register WORD in[],
	     register WORD out[],
	     register LPVOID cargo)
{
  Sampler *smp;
  dng_vector fin(3), fout;
  smp=(Sampler *)cargo;
  fin[0]=w2f(in[0]);
  fin[1]=w2f(in[1]);
  fin[2]=w2f(in[2]);
  fout=smp->cam2xyz(fin,1);
  out[0]=f2w(fout[0]);
  out[1]=f2w(fout[1]);
  out[2]=f2w(fout[2]);
  return TRUE;
}

real64 Sampler::getmaxrgb()
{
  return maxrgb;
}
real64 Sampler::getmaxxyz()
{
  return maxxyz;
}
