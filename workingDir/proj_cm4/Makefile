include ../common.mk

APPNAME=proj_cm4
VERBOSE=

COMPONENTS+=FREERTOS

DISABLE_COMPONENTS=CM0P_SLEEP CM0P_SECURE CM0P_CRYPTO CM0P_BLESS

CORE=CM4
CORE_NAME=CM4_0

SOURCES=
INCLUDES=
DEFINES=

VFP_SELECT=

CFLAGS=
CXXFLAGS=
ASFLAGS=
LDFLAGS=
LDLIBS=
LINKER_SCRIPT=
PREBUILD=
POSTBUILD=

# ============================================================
# Adam app wiring
# ============================================================
FREERTOS_ROOT=../../mtb_shared/freertos/release-v10.6.202

COMPONENTS+=FREERTOS
DEFINES+=USE_FREERTOS

INCLUDES+=.
INCLUDES+=src
INCLUDES+=src/tasks
INCLUDES+=src/drivers
INCLUDES+=src/config
INCLUDES+=src/vendor

INCLUDES+=$(FREERTOS_ROOT)/Source/include
INCLUDES+=$(FREERTOS_ROOT)/Source/portable/COMPONENT_CM4/TOOLCHAIN_GCC_ARM

SOURCES+=$(wildcard $(FREERTOS_ROOT)/Source/*.c)
SOURCES+=$(FREERTOS_ROOT)/Source/portable/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/port.c
SOURCES+=$(FREERTOS_ROOT)/Source/portable/MemMang/heap_4.c

RETARGET_IO_ROOT=../../mtb_shared/retarget-io/release-v1.9.0

INCLUDES+=$(RETARGET_IO_ROOT)/include
SOURCES+=$(wildcard $(RETARGET_IO_ROOT)/source/*.c)


CY_APP_PATH=

CY_GETLIBS_SHARED_PATH=../..
CY_GETLIBS_SHARED_NAME=mtb_shared

include $(CY_TOOLS_DIR)/make/start.mk