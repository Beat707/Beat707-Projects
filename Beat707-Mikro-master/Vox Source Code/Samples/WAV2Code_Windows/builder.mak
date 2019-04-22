# Microsoft Developer Studio Generated NMAKE File, Based on builder.dsp
!IF "$(CFG)" == ""
CFG=builder - Win32 Debug
!MESSAGE No configuration specified. Defaulting to builder - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "builder - Win32 Release" && "$(CFG)" != "builder - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "builder.mak" CFG="builder - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "builder - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "builder - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "builder - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\builder.exe"


CLEAN :
	-@erase "$(INTDIR)\builder.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\builder.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=xicl6.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\builder.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\builder.bsc" 
BSC32_SBRS= \
	
LINK32=xilink6.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\builder.pdb" /machine:I386 /out:"$(OUTDIR)\builder.exe" 
LINK32_OBJS= \
	"$(INTDIR)\builder.obj"

"$(OUTDIR)\builder.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "builder - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\WusikSND.exe"


CLEAN :
	-@erase ".\Output\builder.obj"
	-@erase ".\Output\vc60.idb"
	-@erase ".\WusikSND.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=xicl6.exe
CPP_PROJ=/nologo /G6 /MT /W3 /GX /Ob2 /I "..\..\common" /I ".\source" /D "WIN32" /D "_WINDOWS" /D WINDOWS=1 /D "_CONSOLE" /D "_MBCS" /D "__WINDOWS_DS__" /D "__LITTLE_ENDIAN__" /Fo"Output/" /Fd"Output/" /FD /GA -O3 -Qxi -Qip /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\builder.bsc" 
BSC32_SBRS= \
	
LINK32=xilink6.exe
LINK32_FLAGS=oldnames.lib kernel32.lib user32.lib gdi32.lib winspool.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comdlg32.lib msvcrt.lib /nologo /subsystem:console /pdb:none /machine:I386 /nodefaultlib:"libcmt.lib" /out:"WusikSND.exe" /libpath:"..\Common" 
LINK32_OBJS= \
	".\Output\builder.obj"

".\WusikSND.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("builder.dep")
!INCLUDE "builder.dep"
!ELSE 
!MESSAGE Warning: cannot find "builder.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "builder - Win32 Release" || "$(CFG)" == "builder - Win32 Debug"
SOURCE=.\builder.cpp

!IF  "$(CFG)" == "builder - Win32 Release"


"$(INTDIR)\builder.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\builder.pch"


!ELSEIF  "$(CFG)" == "builder - Win32 Debug"


".\Output\builder.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

