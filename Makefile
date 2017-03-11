CHMOD = /usr/bin/chmod
CP = /usr/bin/cp
MAKE = /usr/bin/make
PATCH = /usr/bin/patch
SCONS = /usr/bin/scons
SED = /usr/bin/sed
UNZIP = /usr/bin/unzip -o

all: compile

.PHONY: patch
patch:
	for file in patch/*; do \
		$(PATCH) -p0 < $$file || exit 1; \
	done

dependencies:
	$(UNZIP) dng_sdk_*.zip
	$(UNZIP) lcms-*.zip
	$(UNZIP) XMP-Toolkit-SDK-*.zip
	$(MAKE) patch
	$(CHMOD) u=rw dng_sdk_1_2/dng_sdk/source/dng_hue_sat_map.h
	$(CP) fixes/dng_sdk/dng_hue_sat_map.h dng_sdk_1_2/dng_sdk/source
	$(CP) fixes/XMP-Toolkit/XMPFiles.hpp XMP-Toolkit-SDK-4.4.2/source/XMPFiles
	$(CP) fixes/XMP-Toolkit/LargeFileAccess.hpp XMP-Toolkit-SDK-4.4.2/source/common
	${SED} -i -e '/mimetype/s,delete,&[],' XMP-Toolkit-SDK-4.4.2/source/XMPFiles/FileHandlers/UCF_Handler.cpp

compile: dependencies
	$(SCONS)

clean:
	$(RM) -r build/
	$(RM) dcp2icc
	$(RM) -r dng_sdk_*/
	$(RM) -r lcms-*/
	$(RM) .sconsign.dblite
	$(RM) -r XMP-Toolkit-SDK-*/
