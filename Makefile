SCONS = scons
UNZIP = unzip -o

all: compile

dependencies:
	$(UNZIP) dng_sdk_*.zip
	$(UNZIP) lcms-*.zip
	$(UNZIP) XMP-Toolkit-SDK-*.zip

compile: dependencies
	$(SCONS)

clean:
	$(RM) -r build/
	$(RM) -r dng_sdk_*/
	$(RM) -r lcms-*/
	$(RM) .sconsign.dblite
	$(RM) -r XMP-Toolkit-SDK-*/
