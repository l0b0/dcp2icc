#ifndef SAMPLER_H_INCLUDED
#define SAMPLER_H_INCLUDED
#include "dng_matrix.h"
#include "dng_hue_sat_map.h"
#include "dng_tone_curve.h"
#include "dng_1d_table.h"
#include "lcms.h"

class Sampler {
private:
  dng_matrix fm;
  dng_matrix rm;
  dng_matrix cam2rgb_matrix;
  dng_matrix rgb2xyz_matrix;
  dng_hue_sat_map hsm;  // Hue Saturation Map
  dng_hue_sat_map ltd;  // Profile Look Table
  dng_1d_table tc;      // Tone curve
  real64 maxrgb;
  real64 maxxyz;
public:
  Sampler(const dng_matrix &nfm);
  void setfm(const dng_matrix &nfm);
  dng_matrix getfm();
  void sethsm(const dng_hue_sat_map &nhsm);
  void settc(const dng_tone_curve &ntc);
  void setltd(const dng_hue_sat_map &nltd);
  dng_vector cam2xyz(const dng_vector &cam, int test);
  dng_vector cam2rgb(const dng_vector &cam, int test);
  real64 getmaxrgb();
  real64 getmaxxyz();
};

extern "C" {
int dcpSampler(register WORD in[],
	       register WORD out[],
	       register LPVOID cargo);
int dcpTest1(register WORD in[],
	     register WORD out[],
	     register LPVOID cargo);
int dcpTest2(register WORD in[],
	     register WORD out[],
	     register LPVOID cargo);
}
#endif
