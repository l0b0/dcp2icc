PATCH = /usr/bin/patch
SCONS = /usr/bin/scons
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
	make patch

compile: dependencies
	$(SCONS)

clean:
	$(RM) -r build/
	$(RM) -r dng_sdk_*/
	$(RM) -r lcms-*/
	$(RM) .sconsign.dblite
	$(RM) -r XMP-Toolkit-SDK-*/
