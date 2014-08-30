#include <iostream>
#include <icc34.h>
#include <lcms.h>
#include <malloc.h>
#include <string.h>
#include "dng_camera_profile.h"
#include "dng_file_stream.h"
#include "dng_matrix.h"
#include "dng_xy_coord.h"
#include "matrix.h"
#include "sampler.h"


int CreateICC(const char *fname, Sampler *smp,
	      const char *name, const char *model)
{
  cmsHPROFILE h;
  LPLUT clut;

  h=cmsOpenProfileFromFile(fname,"w");
  if(!h) {
    std::cerr << "Error creating \"" <<  fname << "\"" << std::endl;
    return -1;
  }
  cmsSetDeviceClass(h, icSigInputClass);
  cmsSetColorSpace(h, icSigRgbData);
  cmsSetPCS(h, icSigXYZData);
  cmsSetRenderingIntent(h, INTENT_PERCEPTUAL);
  cmsAddTag(h, icSigProfileDescriptionTag, (std::string("Converted from ")+std::string(name)).c_str());
  cmsAddTag(h, icSigCopyrightTag, "(c) Nobody");
  cmsAddTag(h, icSigDeviceModelDescTag, model);
  clut=cmsAllocLUT();
  cmsAlloc3DGrid(clut, 33, 3, 3);

  if(!cmsSample3DGrid(clut, dcpTest1, smp, 0)) {
    std::cerr << "Error creating CLUT" << std::endl;
  }

  if(!cmsSample3DGrid(clut, dcpTest2, smp, 0)) {
    std::cerr << "Error creating CLUT" << std::endl;
  }

  std::cout << "maxrgb=" << smp->getmaxrgb() << std::endl;
  std::cout << "maxxyz=" << smp->getmaxxyz() << std::endl;

  if(!cmsSample3DGrid(clut, dcpSampler, smp, 0)) {
    std::cerr << "Error creating CLUT" << std::endl;
  }

  if(!cmsAddTag(h,icSigAToB0Tag, clut)) {
    std::cerr << "Error adding CLUT" << std::endl;
  }

  cmsFreeLUT(clut);
  cmsCloseProfile(h);
}

int main(int argc, char *argv[])
{
  dng_camera_profile dcp;
  double t1, t2, t;
  dng_matrix cm1, cm2, cm;
  dng_matrix rm1, rm2, rm;
  dng_matrix fm1, fm2, fm;
  dng_hue_sat_map ltd, hsm;
  dng_tone_curve tc;
  dng_matrix XYZtoCamera, CameratoXYZ;
  dng_matrix CameratoXYZ_D50;
  dng_matrix CameraWBtoXYZ;
  int ColorPlanes;
  dng_vector CameraNeutral;
  char *dcp_name, *cp;
  char *icc_name;
  if(argc < 3) {
    fprintf(stderr,"Usage: %s <dcp> <color temperature>\n", argv[0]);
    return 1;
  }
  t=atof(argv[2]);
  dcp_name=argv[1];

  icc_name=(char *)malloc(strlen(dcp_name)+16);
  strcpy(icc_name, dcp_name);
  cp=strrchr(icc_name,'/');
  if(!cp)
    cp=icc_name;
  cp=strrchr(cp,'.');
  if(!cp)
    cp=icc_name+strlen(icc_name);
  strcpy(cp,".icc");
  // icc_name now points to the name of .icc file

  dng_file_stream dcp_file(argv[1]);
  dcp.ParseExtended(dcp_file);
  std::cout << "Profile name=" << dcp.Name().Get() << std::endl;
  std::cout << "Profile copyright=" << dcp.Copyright().Get() << std::endl;
  std::cout << "Camera Model=" << dcp.UniqueCameraModelRestriction().Get() << std::endl;
  t1=dcp.CalibrationTemperature1();
  t2=dcp.CalibrationTemperature2();

  std::cout << "Temp1=" << t1 <<std::endl;
  std::cout << "Temp2=" << t2 <<std::endl;
  std::cout << "Temp=" << t <<std::endl;

  fm1=dcp.ForwardMatrix1();
  PrintMatrix("ForwardMatrix1", fm1);

  fm2=dcp.ForwardMatrix2();
  if(!equal(fm1, fm2)) {
    PrintMatrix("ForwardMatrix2", fm2);

    fm=Interpolate(t1,t2,t,fm1,fm2);
    PrintMatrix("ForwardMatrix", fm);
  } else {
    std::cout << "ForwardMatrix2=ForwardMatrix1=ForwardMatrix" << std::endl;
    fm=fm1;
  }
  if(fm.IsEmpty()) {
    // Если нет ForwardMatrix, используем ColorMatrix
    cm1=dcp.ColorMatrix1();
    PrintMatrix("ColorMatrix1", cm1);

    cm2=dcp.ColorMatrix2();
    if(!equal(cm1,cm2)) {
      PrintMatrix("ColorMatrix2", cm2);

      cm=Interpolate(t1,t2,t,cm1,cm2);
      PrintMatrix("ColorMatrix",cm);
    } else {
      std::cout << "ColorMatrix2=ColorMatrx1=ColorMatrix" << std::endl;
      cm=cm1;
    }

    XYZtoCamera=cm;
    CameraNeutral=XYZtoCamera*PCStoXYZ();
    PrintMatrix("CameraNeutral", CameraNeutral);
    ColorPlanes=cm.Cols();
    if(ColorPlanes != 3) {
      rm1=dcp.ReductionMatrix1();
      PrintMatrix("ReductionMatrix1", rm1);

      rm2=dcp.ReductionMatrix2();
      if(!equal(rm1, rm2)) {
	PrintMatrix("ReductionMatrix2", rm2);

	rm=Interpolate(t1,t2,t,rm1,rm2);
	PrintMatrix("ReductionMatrix", rm);
      } else {
	std::cout << "ReductionMatrix2=ReductionMatrx1=ReductionMatrix" << std::endl;
	rm=rm1;
      }
    }
    CameratoXYZ=Invert(XYZtoCamera, rm);
    PrintMatrix("CameratoXYZ", CameratoXYZ);
    CameraWBtoXYZ=CameratoXYZ*CameraNeutral.AsDiagonal();
    PrintMatrix("CameraWBtoXYZ", CameraWBtoXYZ);
    fm=CameraWBtoXYZ;
  }
  hsm=Interpolate(t1,t2,t,dcp.HueSatDeltas1(),dcp.HueSatDeltas2());
  ltd=dcp.LookTable();
  tc=dcp.ToneCurve();

  Sampler smp(fm);
  if(hsm.IsValid())
    smp.sethsm(hsm);
  if(ltd.IsValid())
    smp.setltd(ltd);
  if(tc.IsValid())
    smp.settc(tc);

  CreateICC(icc_name, &smp,
	    dcp.Name().Get(), dcp.UniqueCameraModelRestriction().Get());

  return 0;
}
