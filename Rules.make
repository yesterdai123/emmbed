# Define target platform.
PLATFORM=omapl138

# The installation directory of the SDK.
SDK_INSTALL_DIR=$(HOME)/ti-dvsdk_omapl138-evm_4_02_00_06

# For backwards compatibility
DVEVM_INSTALL_DIR=$(SDK_INSTALL_DIR)

# The directory that points to your kernel source directory.
LINUXKERNEL_INSTALL_DIR=$(HOME)/Omapl138kba/src/linux-headers-generic
# Where the development headers and libs are installed.
LINUXLIBS_INSTALL_DIR=$(SDK_INSTALL_DIR)/linux-devkit/arm-none-linux-gnueabi/usr

# The prefix to be added before the GNU compiler tools (optionally including # path), i.e. "arm_v5t_le-" or "/opt/bin/arm_v5t_le-".
CSTOOL_DIR=$(HOME)/Omapl138kba/arm-2009q1
CSTOOL_PREFIX=$(CSTOOL_DIR)/bin/arm-none-linux-gnueabi-

MVTOOL_DIR=$(CSTOOL_DIR)
MVTOOL_PREFIX=$(CSTOOL_PREFIX)

# Where to copy the resulting executables
EXEC_DIR=/opt/nfs

