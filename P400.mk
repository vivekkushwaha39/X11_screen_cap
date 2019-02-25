#################################################################
#  Global Viperpay build                                        #
#                                                               #
#  Author: Raju Kapparamata, Michael Frankel                    #
#                                                               #
#  make [all|clean|rebuild]                                     #
#                                                               #
#  Important: Run make from the directory containing this file. #
#                                                               #
#################################################################

# Executable name and location
TARGET_DIR :=  output
TARGET     :=  $(TARGET_DIR)/Evpay.exe

# ADK and SDK versions
ADK_VERSION  :=  adk-full-4.4.10-376
# adk-full-4.4.9-370
# adk-full-4.5.2-281
SDK_VERSION  :=  vos2-sdk-winx86-release-30811500
# vos2-sdk-winx86-release-30811400
# vos2-sdk-winx86-release-30920300

#ADK and SDK versions for VIPA 6.6
#ADK_VERSION :=	adk-full-ext-4.4.9-370
#SDK_VERSION  :=  vos2-sdk-winx86-release-30811400

# Build environment
ifeq ($(OS),Windows_NT)
	# Local Build
	
	# Optional env variable: VFI_STAGING_DIR
	STAGING_DIR  :=  ./imports
	ifneq ($(VFI_STAGING_DIR),)
	  STAGING_DIR  :=  $(VFI_STAGING_DIR)
	endif
	
	
	# Optional env variable: VFI_ADK_VERSION
	ifneq ($(VFI_ADK_VERSION),)
	  ADK_VERSION  :=  $(VFI_ADK_VERSION)
	endif
	
	# Optional env variable: VFI_SDK_VERSION
	ifneq ($(VFI_SDK_VERSION),)
	  SDK_VERSION  :=  $(VFI_SDK_VERSION)
	endif    
	
    SDK_PATH     :=  $(STAGING_DIR)/SDKs/$(SDK_VERSION)/vos2
    ADK_PATH     :=  $(STAGING_DIR)/ADKs/$(ADK_VERSION)/vos2
    TOOL_CHAIN   :=  $(STAGING_DIR)/toolchains/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03
else
    # Jenkins Build
    STAGING_DIR  :=  $(WORKSPACE)
    SDK_PATH     :=  $(STAGING_DIR)/imports/SDKs/vos2
    ADK_PATH     :=  $(STAGING_DIR)/imports/ADKs/$(ADK_VERSION)/vos2
    TOOL_CHAIN   :=  /u00/toolchains/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux
endif

# Compiler
COMPILER := arm-linux-gnueabihf-g++

# Build command
CC := $(TOOL_CHAIN)/bin/$(COMPILER)

# Build options
CFLAGS  :=  -O2 -Wall -fmessage-length=0 -Wno-write-strings -fPIC -fno-common -rdynamic -std=gnu++0x --sysroot=$(SDK_PATH)
DEFINES :=  -D_REENTRANT -D__USE_GNU
LDFLAGS :=  -Bsymbolic-functions

# ReleaseMode or DebugMode
ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG_ON
else
	CFLAGS += 
endif

# Include paths: Application
INC_DIR_AGENTS  :=  Sources/Agents/include
INC_DIR_CFG     :=  Sources/Configuration/include
INC_DIR_COMMON  :=  include
INC_DIR_EPSC    :=  Sources/EPSCommunicator/include
INC_DIR_EXCEPTIONS :=  Sources/Exceptions/include
INC_DIR_IFSF    :=  Sources/IFSF/include
INC_DIR_MAIN    :=  Sources/Main/include
INC_DIR_MM      :=  Sources/MessageManager/include
INC_DIR_PPSM    :=  Sources/PPSM/include
INC_DIR_STATES  :=  Sources/States/include
INC_DIR_VIPA    :=  Sources/VIPAInterface/include
INC_DIR_LOGGING :=  Sources/GVPLogging/include
INC_DIR_UTILS   :=  Sources/Utils/include
INC_DIR_SCREENS := 	Sources/Screens/include

# Includes: Application
INC_PATHS_APP := -I$(INC_DIR_AGENTS) \
                 -I$(INC_DIR_CFG) \
                 -I$(INC_DIR_COMMON) \
                 -I$(INC_DIR_EPSC) \
                 -I$(INC_DIR_EXCEPTIONS) \
                 -I$(INC_DIR_IFSF) \
                 -I$(INC_DIR_MAIN) \
                 -I$(INC_DIR_MM) \
                 -I$(INC_DIR_PPSM) \
                 -I$(INC_DIR_STATES) \
                 -I$(INC_DIR_VIPA) \
                 -I$(INC_DIR_LOGGING) \
                 -I$(INC_DIR_UTILS) \
                 -I$(INC_DIR_SCREENS)

# Include paths: SDK
INC_PATHS_SDK := -I$(SDK_PATH)/usr/include \
                 -I$(SDK_PATH)/usr/include/libxml2 \
                 -I$(SDK_PATH)/usr/include/openssl \
                 -I$(SDK_PATH)/usr/local/include \
                 -I$(SDK_PATH)/usr/local/include/svcmgr

# Include paths: ADK
INC_PATHS_ADK := -I$(ADK_PATH)/include \
                 -I$(ADK_PATH)/include/com \
                 -I$(ADK_PATH)/include/agt \
                 -I$(ADK_PATH)/include/sound

# Include paths: tool chain
INC_PATHS_TOOL_CHAIN := -I$(TOOL_CHAIN)/arm-linux-gnueabihf/include/c++/4.7.3

# Include paths: Concatenated
INC_PATHS := $(INC_PATHS_APP)\
             $(INC_PATHS_SDK) \
             $(INC_PATHS_ADK) \
             $(INC_PATHS_TOOL_CHAIN)

# Library paths
LIB_PATHS := -L$(SDK_PATH)/lib \
             -L$(SDK_PATH)/usr/lib \
             -L$(SDK_PATH)/usr/lib/ssl \
             -L$(SDK_PATH)/usr/local/lib \
             -L$(SDK_PATH)/usr/local/lib/svcmgr \
             -L$(ADK_PATH)/lib

# Libraries
LIBS :=  $(LIB_PATHS) \
        -lsvc_utility -lsvc_event -lxml2 -lsvcmgrSvcInf -lsvcmgrSvcInfXml -lvfisysinfo \
        -lsvcmgr -lvfisvc -lvficore -liniparser -lvfirtc -lvfiplatforminfo  \
        -ldl -lsvc_netloader -lsvc_sysinfo -ludev -lz -lcjson -lsecins -lvfiled \
        -lvfirkl -lcap -lsvc_net -lssl -lvfisec -lvfileg -lvfiutils -lresolv  \
        -lvfimsr -lsvc_bluetooth -lrt -lber-tlv -lcrypto -lvfiVaultapi \
        -lattr -lvfimodem -lvfigsm -lcom -lexpat -linf-static -lsqlite -lvfiipc \
        -llog -lsvc_logmgr -lpthread -lvfirfcr -lvfictlsinterface \
        -lsvc_powermngt -lsvc_security -lsvc_sound -lsvc_led -lvfiguiprt \
        -lsvc_tms \
        -lEMV_CT_Framework -lEMV_CT_Link -lTLV_Util -lsound -lmpg123 -lout123 \
		-lpng -lNX11 -lx11 -lfreetype

# Application sources
SRCFILES := $(wildcard ./src/*.cpp)

# Object file list
OBJS = $(patsubst %.cpp, raptor_objs/%.o, $(SRCFILES) )

# -------- Rules ---------

all: prerequisites $(TARGET)

rebuild: clean $(TARGET)

prerequisites:
	@find src/ -name '*' -type d -exec mkdir -p "raptor_objs/{}" \;
	@echo "Creating P400 objects directory"

clean:
	@echo
	@echo -------------------------------------------------------------------------------
	@echo Cleaning
	@echo -------------------------------------------------------------------------------
	@rm -fv $(OBJS)
	@rm -frv $(TARGET_DIR)
	@echo done
	@echo

$(TARGET): $(OBJS)
	@echo -------------------------------------------------------------------------------
	@echo Generating  target: $(TARGET)
	@echo -------------------------------------------------------------------------------
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@ -lEMV_CT_Client
	@echo done
	@echo

raptor_objs/%.o: %.cpp
	@echo -------------------------------------------------------------------------------
	@echo Compiling: $<
	@echo -------------------------------------------------------------------------------
	$(CC) $(CFLAGS) $(INC_PATHS) -c  $< -o $@
	@echo done
	@echo

# -------------------------------------------------------------------------------
# End of File
# -------------------------------------------------------------------------------
