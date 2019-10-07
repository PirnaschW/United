# Microsoft Visual C++ generated build script - Do not modify

PROJ = UNITED
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG 
R_RCDEFINES = /dNDEBUG 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\MSVC\UNITED\
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /G3 /Gy /W4 /Zi /AH /Gt8192 /Gx- /Od /D "_DEBUG" /FR /Zn /GA /Fd"UNITED.PDB"
CFLAGS_R_WEXE = /nologo /Gs /G3 /W4 /AH /Gt8192 /Gx- /O2 /Ob2 /D "NDEBUG" /GA 
LFLAGS_D_WEXE = /NOLOGO /NOD /NOI /PACKC:61440 /FARCALL /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /NOI /PACKC:61440 /FARCALL /STACK:10240 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = lafxcwd oldnames libw llibcew commdlg.lib 
LIBS_R_WEXE = lafxcw oldnames libw llibcew commdlg.lib 
RCFLAGS = /nologo 
RESFLAGS = /nologo 
RUNFLAGS = 
DEFFILE = UNITED.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif

all:	$(PROJ).EXE $(PROJ).BSC


$(PROJ).EXE::	$(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
e:\msvc\lib\+
e:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) $@


run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
