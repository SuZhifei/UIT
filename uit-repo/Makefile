####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

uit.buildsystem.build_dir := $(UIT_BUILD_DIR)

ifeq ("on","$(UIT_DISTRIBUTED)")
	uit.buildsystem.jobs := 12
endif

uit.buildsystem.groups := \
		package

include ./BuildSystem.mk

dist_clean:
	$(if $(UIT_INSTALL_DIR),,$(error 'UIT_INSTALL_DIR' not been set))
	rm -rf $(UIT_INSTALL_DIR)
