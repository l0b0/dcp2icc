dcp2icc - converter from DNG camera profile to ICC camera profile.
            
To compile in Windows you need:
   Mingw32 C++ compiler. - http://www.mingw.org.
   Python - www.python.org. Required for scons below.
   scons - see www.scons.org.
   expat - headers and libraries for mingw32. Download precompiled
            from http://www.mingw.org
   little cms - source code package version 1.17 from www.littlecms.com.
           Precompiled binary for Windows doesn't work, so we will compile the
           library ourselves.

To compile in Linux you need:
   GNU c++
   Python
   scons
   expat library - development package
   little cms library - development package

You also need:
   XMP-Toolkit-SDK-4.4.2 - source code from Adobe. It is required for dng_sdk.
   dng_sdk_1_2 - DNG SDK from Adobe. www.adobe.com/dng

Below is a picture of directory structure, which you should have
after unpacking all source packages. It will help you to unpack packages.

				  +-public
         +-XMP-Toolkit-SDK-4.4.2--+-source
	 |			  +-third-party
         |
         +-dng_sdk_1_2---dng_sdk--source
         |
	 |              +-include
dcp2icc--+-lcms-1.17----+-src
	 |
         +-dcp2icc.src
	 |
         +-fixes---+--dng_sdk
	           +--XMP-Toolkit
 
Unpack the source of dcp2icc-0.1.src.zip, then unpack XMP-Toolkit-SDK-4.4.2, dng_sdk_1_2, lcms-1.17 in directory dcp2icc.
Run "scons" in directory dcp2icc,  containing this ReadMe.txt file
and SConstruct files. It should compile XMP-Toolkit, dng_sdk,
(and lcms-1.17 in Windows), creating static libraries, and dcp2icc.src,
creating executable dcp2icc.

Using the converter:

dcp2icc <filename.dcp> <color temperature>

where:
   filename.dcp - DNG camera profile
   color temperature - recommended 5000. For most DNG profiles
             result doesn't depend on this value.

filename.icc will be created. Be careful, the existing file will be overwritten
without prompt.

To get DNG camera profiles for different cameras, download and install a
recent version (5.2) of DNG Converter from Adobe. In Linux under Wine it unpacks profiles in "windows/profiles/All Users/Application Data/Adobe/CameraRaw/CameraProfiles". In Windows the directory may be different.
