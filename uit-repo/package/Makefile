####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

uit.buildsystem.repo ?=$(UIT_REPO_DOMAIN_PREFIX)

ifeq ("x11","$(UIT_ARCH)")
	uit.buildsystem.blacklist := \
		uit-uid-kanzi
endif

ifeq ("x86_64","$(UIT_ARCH)")
	uit.buildsystem.blacklist := \
		uit-uid-kanzi
endif

uit.buildsystem.targets := \
		uit-3rdparty \
		uit-env \
		uit-lib-core \
		uit-fw-data \
		uit-fw-screen \
		uit-uid-kanzi \
		uit-uid-qt \
		uit-doc

include ../BuildSystem.mk

.PHONY: install
install:
