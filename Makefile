
# ----------------------------------------------------------------------------
# Project: ViperPay Mx
#
# Purpose: To create the application file "VPAY9xx" from the src 
#
# Author : Ladvine D Almeida
#
# Version History:
# ------------------------------------------
# Version	   Date			    Author               Changes
# -------	---------		----------------     ---------------- 
# 1.0.0		28 July 2015    Ladvine D Almeida     Initial MakeFile
# 1.0.1		29 July 2015    Ladvine D Almeida     1)Added FA(FormAgent Support), 
#									              	LOG4CPP_FIX_ERROR_COLLISION(for Fixing Collision if DEBUG already present)
#												  2)Fixed Issue .c files getting deleted while cleaning
# 1.0.2		30 July 2015	Jakob Driscoll		Changes for Linux Build env
#							Conditional on OS now
# ----------------------------------------------------------------------------


# Please set the staging directory for compilation below.
# We need to use escape characters if spaces are there in path
# Eg: If windows path is "C:\Program Files\CodeSourcery/staging",
# then set STAGING_DIR := C:/Program\ Files/CodeSourcery/staging

OTHER_CFLAG := -rdynamic
ifeq ($(OS),Windows_NT)
	ARCH := arm-verifone-linux-gnueabi-
	STAGING_DIR := "C:\Program Files\CodeSourcery\staging"
	SYSROOT_CFLAGS := --sysroot=$(STAGING_DIR)
	CC := $(ARCH)g++.exe $(SYSROOT_CFLAGS) $(OTHER_CFLAG)
else
	ARCH := arm-none-linux-gnueabi-
	CODE_SOURCERY_BIN := /u00/toolchains/CodeSourcery/Sourcery_G++/bin/
	STAGING_DIR := $(WORKSPACE)/VOS-SDK-1.2.7-DTK400.250300
	SYSROOT_CFLAGS := --sysroot=$(STAGING_DIR)
	CC := $(CODE_SOURCERY_BIN)$(ARCH)g++ $(SYSROOT_CFLAGS) $(OTHER_CFLAG)
endif
SDK := $(STAGING_DIR)


#Modification
#Object directory
OBJDIR=./Objects
#Source directory
SOURCEDIR=src/ 
#Release directory
RELEASE=./Release/
#Output directory
OUTPUT=./Output/
#Lib directory
LIBDIR=./lib/
#Include directory
INCDIR=./include/

#Mx Model 
#Add the correct Mx Model below as compiler symbol
# Mx870 - for the Mx8xx platform
# MX915 - for the Mx9xx platform 
MXMODEL=-DMX915

#Features
#Add the correct symbols to the FFLAGS below
# EMV - for the EMV feature
# NPC - for the NPC Mobile Payment Support
# LIFTSUPPORT - for the Lift Support
# SSLSUPPORT - for the SSL Support
# VTP - for the VCL Support
# FA - FormAgent for speedpass support
FFLAGS= -DNPC -DLIFTSUPPORT -DEMV -DFA -DLOG4CPP_FIX_ERROR_COLLISION -DSSLSUPPORT -DDEBUG -DVTP

#Remove Command
RM = rm -rf

INC_PATHS = -I$(SDK)/usr/include \
			-I$(SDK)/usr/include/libxml2 \
			-I$(SDK)/usr/local/include/ \
			-I$(SDK)/usr/local/include/svcmgr \
			-I$(SDK)/usr/local/include/fancypants \
			-I$(SDK)/usr/local/include/fancypants/libfp \
			-I$(SDK)/usr/include/openssl \
			-I./src \
			-I$(INCDIR)

LIB_PATHS = -L$(SDK)/usr/lib \
			-L$(SDK)/usr/local/lib \
			-L$(SDK)/usr/local/lib/svcmgr \
			-L$(LIBDIR)/ 
			
LDFLAGS			:= -Bsymbolic-functions					 

						 
#CFLAGS = $(INC_PATHS) $(CUST_INCPATHS) -O2 -g -Wall -c -fmessage-length=0 -fcommon
CFLAGS = $(INC_PATHS) -O0 -ggdb -Wall -fmessage-length=0 -Wno-write-strings -fPIC -fno-common -rdynamic
CFLAGS += $(MXMODEL)
CFLAGS += -D_REENTRANT -D__USE_GNU -D__cplusplus -DIDLE_TIMER_FIX  -DXPILIB
CFLAGS += $(FFLAGS)         

#FOR DEBUG COMPILATION
CFLAGSDB =  $(CFLAGS) -DVPAY_DEBUG_ENABLED -DLOG4CPP_FIX_ERROR_COLLISION
			
LIBS      = $(LIB_PATHS) -lsvc_utility -lCOMMComunication_Mx9xx -lsvc_event -lsvc_netloader \
			 -lsvc_sysinfo -lsvc_net -ltcpipdbg9xx -lvfirfcr9xxD -lssl \
			 -lvfwidgets9xx -lvfisvc -lguimgr9xx -lvfivoy  -lxpiapp -lBINTable9xx -lACIEncryptlib9xx \
			 -lADEEncryptlib9xx -lmxstub -lRSAEncrypt9xx -lSTB -lVCL9xxD -lVSDlib9xx -lINI9xx -lsqlite \
			 -lvfirfid9xx  -lvficardslot -lsvc_tms -l:libpng.a

LIBSDB     = $(LIB_PATHS) -lsvc_utility -lCOMMComunication_Mx9xx -lsvc_event -lsvc_netloader \
			 -lsvc_sysinfo -lsvc_net -ltcpipdbg9xx -lvfirfcr9xxD -lssl \
			 -lvfwidgets9xxD -lvfisvc -lguimgr9xxD -lvfivoy  -lxpiappD -lBINTable9xxD -lACIEncryptlib9xxD \
			 -lADEEncryptlib9xxD -lmxstub -lRSAEncrypt9xxD -lSTB -lVCL9xxD -lVSDlib9xxD -lINI9xxD -lsqlite \
			 -lvfirfid9xxD  -lvficardslot -lsvc_tms 

LIBSEXTRA = $(LIB_PATHS) -lNX11 -lpng  -lfreetype  -ljpeg
#-l:x11 -lpng -ljpeg -lnano-X -lfreetype

# Binary file name
TARGET =$(OUTPUT)VPAY9xx
TARGETDB =$(OUTPUT)x11.exe

#For source files selection(.cpp files)
SRCS    := $(shell find $(SOURCEDIR) -name '*.c' -o -name '*.cpp' -type f)
#For object files
OBJS    := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

OBJSDB    := $(patsubst %.cpp,$(OBJDIR)/%D.o,$(SRCS))
# Compile rule

$(OBJDIR)/%.o: $(SOURCEDIR)%.cpp
	@-mkdir -p $(OBJDIR)
	@-mkdir -p $(OBJDIR)/src/agent
	@-mkdir -p $(OBJDIR)/src/ObjectThread
	@-mkdir -p $(OBJDIR)/src/screen
	@-mkdir -p $(OBJDIR)/src/vhq
	@echo "==================================================="
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%D.o: %.cpp
	@-mkdir -p $(OBJDIR)
	@-mkdir -p $(OBJDIR)/src/agent
	@-mkdir -p $(OBJDIR)/src/ObjectThread
	@-mkdir -p $(OBJDIR)/src/screen
	@-mkdir -p $(OBJDIR)/src/vhq
	@echo "==================================================="
	$(CC) $(CFLAGSDB) -c $^ -o $@

.PHONY: all clean clean-custom post-build

debug: $(TARGETDB) post-build
all: $(TARGET) $(TARGETDB) post-build

$(TARGET): $(OBJS) 
	@echo ""
	@echo ""
	@echo "==================================================="
	@echo "    GENERATING APPLICATION FILE - VPAY9xx          "
	@echo "==================================================="
	# TODO: we can write a postMake file to create tgz in Release directory
	mkdir -p $(RELEASE)
	mkdir -p $(OUTPUT)
	$(CC) $(OBJS) -o $(TARGET)   $(LDFLAGS)
	@echo Generated Non-Debug Application File  \'"... $(TARGET)"\'  
	@echo "==================================================="
	@echo "==================================================="
	@echo ""
	@echo ""
	
$(TARGETDB): $(OBJSDB) 
	@echo ""
	@echo ""
	@echo "==================================================="
	@echo "    GENERATING DEBUG APPLICATION FILE - VPAY9xxD   "
	@echo "==================================================="
	# TODO: we can write a postMake file to create tgz in Release directory
	mkdir -p $(RELEASE)
	mkdir -p $(OUTPUT)
	$(CC)  $(OBJSDB) -o $(TARGETDB) $(LDFLAGS) $(LIBSEXTRA)
	@echo Generated Application File  \'"... $(TARGETDB)"\'  
	@echo "==================================================="
	@echo "==================================================="
	@echo ""
	@echo ""

clean: clean-custom
	@echo ""
	@echo ""
	@echo "==================================================="
	@echo "    CLEANING PROJECT FOLDER ..  		              "
	@echo "==================================================="
	@echo ""
	@echo "Cleaning Binary file........"
	@echo ""
	${RM} $(TARGET)
	${RM} $(TARGETDB)
	@echo ""
	@echo "Cleaning Object files........"
	@echo ""
	mkdir -p $(OBJDIR)
	rm -R $(OBJDIR)
	@echo ""
	@echo "==================================================="
	@echo "==================================================="
	@echo ""
	@echo ""
	
post-build:
	@echo ""
	@echo ""
	@echo "==================================================="
	@echo "    		CHANGING FILE PERMISSIONS ..		      "
	@echo "==================================================="
	@echo ""
	@chmod 755 $(TARGETDB)
	@chmod 755 $(LIBDIR)*
	@echo ""
	@echo "==================================================="
	@echo "==================================================="
	@echo ""
	@echo ""
		

