import sys

platform=sys.platform

if platform == "win32":
    env=Environment(
        CPPDEFINES=[("WIN_ENV",1),("_WIN32_WINNT",0x500)],
	LIBS=["dng_1_2","XMPFiles-4.4.2","XMPCore-4.4.2","XMPcommon-4.4.2","expat","lcms-1.17"],
       	LINKFLAGS="",
       	CCFLAGS="-O2 -iquote-",
       	tools=["mingw"],
       	LCMS_INCLUDE="lcms-1.17/include")
    SConscript("SConstruct.lcms", exports="env")
else:
    env=Environment(
	CPPDEFINES=[("UNIX_ENV",1),("_FILE_OFFSET_BITS",64)],
	LIBS=["dng_1_2","XMPFiles-4.4.2","XMPCore-4.4.2","XMPcommon-4.4.2","expat","lcms","pthread"],
   	LINKFLAGS="",
   	CCFLAGS="-O2 -iquote-",
   	LCMS_INCLUDE=None)

SConscript("SConstruct.XMP", exports="env")
SConscript("SConstruct.dng_sdk", exports="env")
SConscript("SConstruct.dcp2icc", exports="env")
