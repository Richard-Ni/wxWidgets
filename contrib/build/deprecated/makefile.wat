# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.0 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../../build/msw/config.wat

# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
!  loaddll wlink    wlink
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = msw
!endif
VENDORTAG =
!ifeq OFFICIAL_BUILD 0
VENDORTAG = _$(VENDOR)
!endif
!ifeq OFFICIAL_BUILD 1
VENDORTAG = 
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
WXDEBUGFLAG = d
!endif
!endif
!ifeq DEBUG_FLAG 1
WXDEBUGFLAG = d
!endif
WXUNICODEFLAG =
!ifeq UNICODE 1
WXUNICODEFLAG = u
!endif
WXUNIVNAME =
!ifeq WXUNIV 1
WXUNIVNAME = univ
!endif
WXDLLFLAG =
!ifeq SHARED 1
WXDLLFLAG = dll
!endif
LIBTYPE_SUFFIX =
!ifeq SHARED 0
LIBTYPE_SUFFIX = lib
!endif
!ifeq SHARED 1
LIBTYPE_SUFFIX = dll
!endif
EXTRALIBS_FOR_BASE =
!ifeq MONOLITHIC 0
EXTRALIBS_FOR_BASE = 
!endif
!ifeq MONOLITHIC 1
EXTRALIBS_FOR_BASE =  
!endif
__deprecateddll___depname =
!ifeq SHARED 1
__deprecateddll___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated_wat$(VENDORTAG).dll
!endif
__DEBUGINFO_2 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_2 = debug all
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_2 = 
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_2 = 
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_2 = debug all
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
!endif
__LIB_TIFF_p =
!ifeq USE_GUI 1
__LIB_TIFF_p = wxtiff$(WXDEBUGFLAG).lib
!endif
__LIB_JPEG_p =
!ifeq USE_GUI 1
__LIB_JPEG_p = wxjpeg$(WXDEBUGFLAG).lib
!endif
__LIB_PNG_p =
!ifeq USE_GUI 1
__LIB_PNG_p = wxpng$(WXDEBUGFLAG).lib
!endif
__GDIPLUS_LIB_p =
!ifeq USE_GDIPLUS 1
__GDIPLUS_LIB_p = gdiplus.lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core.lib
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = &
	wxbase$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
!endif
__deprecatedlib___depname =
!ifeq SHARED 0
__deprecatedlib___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated.lib
!endif
__DEBUGINFO_1 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = -d2
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = -d0
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_1 = -d0
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_1 = -d2
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__THREADSFLAG =
!ifeq USE_THREADS 0
__THREADSFLAG = 
!endif
!ifeq USE_THREADS 1
__THREADSFLAG = -bm
!endif
__RUNTIME_LIBS =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS = 
!endif
__RTTIFLAG =
!ifeq USE_RTTI 0
__RTTIFLAG = 
!endif
!ifeq USE_RTTI 1
__RTTIFLAG = -xr
!endif
__EXCEPTIONSFLAG =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONSFLAG = 
!endif
!ifeq USE_EXCEPTIONS 1
__EXCEPTIONSFLAG = -xs
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
!endif
!ifeq DEBUG_FLAG 1
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__EXCEPTIONS_DEFINE_p =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONS_DEFINE_p = -dwxNO_EXCEPTIONS
!endif
__RTTI_DEFINE_p =
!ifeq USE_RTTI 0
__RTTI_DEFINE_p = -dwxNO_RTTI
!endif
__THREAD_DEFINE_p =
!ifeq USE_THREADS 0
__THREAD_DEFINE_p = -dwxNO_THREADS
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
__GFXCTX_DEFINE_p =
!ifeq USE_GDIPLUS 1
__GFXCTX_DEFINE_p = -dwxUSE_GRAPHICS_CONTEXT=1
!endif

### Variables: ###

WX_RELEASE_NODOT = 27
WX_VERSION_NODOT = $(WX_RELEASE_NODOT)1
OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = ..\..\src\deprecated\..\..\..\lib\wat_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
DEPRECATEDDLL_CFLAGS = -bd $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=..\..\src\deprecated\..\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 &
	-wcd=667 -i=..\..\src\deprecated\..\..\include -dWXUSINGDLL &
	-dWXMAKINGDLL_DEPRECATED -i=..\..\src\deprecated $(CPPFLAGS) $(CFLAGS)
DEPRECATEDDLL_CXXFLAGS = -bd $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) &
	$(__THREADSFLAG) $(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) &
	$(__DEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) &
	$(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) &
	-i=$(SETUPHDIR) -i=..\..\src\deprecated\..\..\..\include -wx -wcd=549 &
	-wcd=656 -wcd=657 -wcd=667 -i=..\..\src\deprecated\..\..\include &
	-dWXUSINGDLL -dWXMAKINGDLL_DEPRECATED -i=..\..\src\deprecated &
	/fh=$(OBJS)\wxprec_deprecateddll.pch $(__RTTIFLAG) $(__EXCEPTIONSFLAG) &
	$(CPPFLAGS) $(CXXFLAGS)
DEPRECATEDDLL_OBJECTS =  &
	$(OBJS)\deprecateddll_dummy.obj &
	$(OBJS)\deprecateddll_prop.obj &
	$(OBJS)\deprecateddll_propform.obj &
	$(OBJS)\deprecateddll_proplist.obj &
	$(OBJS)\deprecateddll_resource.obj &
	$(OBJS)\deprecateddll_tbarsmpl.obj &
	$(OBJS)\deprecateddll_treelay.obj &
	$(OBJS)\deprecateddll_wxexpr.obj &
	$(OBJS)\deprecateddll_dosyacc.obj
DEPRECATEDLIB_CFLAGS = $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=..\..\src\deprecated\..\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 &
	-wcd=667 -i=..\..\src\deprecated\..\..\include -i=..\..\src\deprecated &
	$(CPPFLAGS) $(CFLAGS)
DEPRECATEDLIB_CXXFLAGS = $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=..\..\src\deprecated\..\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 &
	-wcd=667 -i=..\..\src\deprecated\..\..\include -i=..\..\src\deprecated &
	/fh=$(OBJS)\wxprec_deprecatedlib.pch $(__RTTIFLAG) $(__EXCEPTIONSFLAG) &
	$(CPPFLAGS) $(CXXFLAGS)
DEPRECATEDLIB_OBJECTS =  &
	$(OBJS)\deprecatedlib_dummy.obj &
	$(OBJS)\deprecatedlib_prop.obj &
	$(OBJS)\deprecatedlib_propform.obj &
	$(OBJS)\deprecatedlib_proplist.obj &
	$(OBJS)\deprecatedlib_resource.obj &
	$(OBJS)\deprecatedlib_tbarsmpl.obj &
	$(OBJS)\deprecatedlib_treelay.obj &
	$(OBJS)\deprecatedlib_wxexpr.obj &
	$(OBJS)\deprecatedlib_dosyacc.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__deprecateddll___depname) $(__deprecatedlib___depname)

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated_wat$(VENDORTAG).dll del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated_wat$(VENDORTAG).dll
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated.lib
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated.lib

!ifeq SHARED 1
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated_wat$(VENDORTAG).dll :  $(DEPRECATEDDLL_OBJECTS) $(OBJS)\deprecateddll_version.res
	@%create $(OBJS)\deprecateddll.lbc
	@%append $(OBJS)\deprecateddll.lbc option quiet
	@%append $(OBJS)\deprecateddll.lbc name $^@
	@%append $(OBJS)\deprecateddll.lbc option caseexact
	@%append $(OBJS)\deprecateddll.lbc $(LDFLAGS) $(__DEBUGINFO_2)  libpath $(LIBDIRNAME)
	@for %i in ($(DEPRECATEDDLL_OBJECTS)) do @%append $(OBJS)\deprecateddll.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  $(__GDIPLUS_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p) ) do @%append $(OBJS)\deprecateddll.lbc library %i
	@%append $(OBJS)\deprecateddll.lbc option resource=$(OBJS)\deprecateddll_version.res
	@%append $(OBJS)\deprecateddll.lbc system nt_dll
	wlink @$(OBJS)\deprecateddll.lbc
	wlib -q -n -b $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated.lib +$^@
!endif

!ifeq SHARED 0
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated.lib :  $(DEPRECATEDLIB_OBJECTS)
	@%create $(OBJS)\deprecatedlib.lbc
	@for %i in ($(DEPRECATEDLIB_OBJECTS)) do @%append $(OBJS)\deprecatedlib.lbc +%i
	wlib -q -p4096 -n -b $^@ @$(OBJS)\deprecatedlib.lbc
!endif

$(OBJS)\deprecateddll_dummy.obj :  .AUTODEPEND ../../src/deprecated\..\..\..\src\common\dummy.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CXXFLAGS) $<

$(OBJS)\deprecateddll_version.res :  .AUTODEPEND ../../src/deprecated\..\..\..\src\msw\version.rc
	wrc -q -ad -bt=nt -r -fo=$^@   -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) -i=..\..\src\deprecated\..\..\..\include -dWXDLLNAME=wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated_wat$(VENDORTAG) $<

$(OBJS)\deprecateddll_prop.obj :  .AUTODEPEND ../../src/deprecated\prop.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CXXFLAGS) $<

$(OBJS)\deprecateddll_propform.obj :  .AUTODEPEND ../../src/deprecated\propform.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CXXFLAGS) $<

$(OBJS)\deprecateddll_proplist.obj :  .AUTODEPEND ../../src/deprecated\proplist.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CXXFLAGS) $<

$(OBJS)\deprecateddll_resource.obj :  .AUTODEPEND ../../src/deprecated\resource.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CXXFLAGS) $<

$(OBJS)\deprecateddll_tbarsmpl.obj :  .AUTODEPEND ../../src/deprecated\tbarsmpl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CXXFLAGS) $<

$(OBJS)\deprecateddll_treelay.obj :  .AUTODEPEND ../../src/deprecated\treelay.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CXXFLAGS) $<

$(OBJS)\deprecateddll_wxexpr.obj :  .AUTODEPEND ../../src/deprecated\wxexpr.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CXXFLAGS) $<

$(OBJS)\deprecateddll_dosyacc.obj :  .AUTODEPEND ../../src/deprecated\dosyacc.c
	$(CC) -bt=nt -zq -fo=$^@ $(DEPRECATEDDLL_CFLAGS) $<

$(OBJS)\deprecatedlib_dummy.obj :  .AUTODEPEND ../../src/deprecated\..\..\..\src\common\dummy.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CXXFLAGS) $<

$(OBJS)\deprecatedlib_prop.obj :  .AUTODEPEND ../../src/deprecated\prop.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CXXFLAGS) $<

$(OBJS)\deprecatedlib_propform.obj :  .AUTODEPEND ../../src/deprecated\propform.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CXXFLAGS) $<

$(OBJS)\deprecatedlib_proplist.obj :  .AUTODEPEND ../../src/deprecated\proplist.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CXXFLAGS) $<

$(OBJS)\deprecatedlib_resource.obj :  .AUTODEPEND ../../src/deprecated\resource.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CXXFLAGS) $<

$(OBJS)\deprecatedlib_tbarsmpl.obj :  .AUTODEPEND ../../src/deprecated\tbarsmpl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CXXFLAGS) $<

$(OBJS)\deprecatedlib_treelay.obj :  .AUTODEPEND ../../src/deprecated\treelay.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CXXFLAGS) $<

$(OBJS)\deprecatedlib_wxexpr.obj :  .AUTODEPEND ../../src/deprecated\wxexpr.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CXXFLAGS) $<

$(OBJS)\deprecatedlib_dosyacc.obj :  .AUTODEPEND ../../src/deprecated\dosyacc.c
	$(CC) -bt=nt -zq -fo=$^@ $(DEPRECATEDLIB_CFLAGS) $<

