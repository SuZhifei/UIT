
# UIT Build System v3.0 Beta3

# $(1) build target: uit-xxx-xxx

# [uit.buildsystem]

uit.buildsystem.repo ?=

uit.buildsystem.cpu_cores = $(shell cat /proc/cpuinfo| grep "processor" | wc -l)

uit.buildsystem.jobs ?= $(uit.buildsystem.cpu_cores)

uit.buildsystem.git_jobs ?= $(uit.buildsystem.cpu_cores)

uit.buildsystem.build_dir ?= $(UIT_BUILD_DIR)

uit.buildsystem.targets ?=

uit.buildsystem.groups ?=

uit.buildsystem.blacklist ?=

uit.buildsystem.selfmake ?=

uit.buildsystem.baseline = $(UIT_BASELINE)
#$(info Baseline: $(uit.buildsystem.baseline))

# [uit.buildsystem.utils]

uit.buildsystem.utils.make ?= $(MAKE)

uit.buildsystem.utils.parallel_make = $(uit.buildsystem.utils.make) -j$(uit.buildsystem.jobs)

uit.buildsystem.utils.cmake ?= $(UIT_CMAKE) --no-warn-unused-cli

uit.buildsystem.utils.update ?= $(UIT_UPDATE)

uit.buildsystem.utils.astyle ?= $(SVP_ASTYLE)

uit.buildsystem.utils.cpplint ?= $(SVP_CPPLINT)

# [uit.buildsystem.core] --- Shouldn't be overided!
uit.buildsystem.real_targets = $(filter-out $(uit.buildsystem.blacklist),$(uit.buildsystem.targets))
#$(info Real Targets: $(uit.buildsystem.real_targets))

uit.buildsystem.real_groups = $(filter-out $(uit.buildsystem.blacklist),$(uit.buildsystem.groups))
#$(info Real Groups: $(uit.buildsystem.real_groups))

uit.buildsystem.core.init_groups := $(patsubst %,init_%, $(uit.buildsystem.real_groups))

uit.buildsystem.core.update_groups := $(patsubst %,update_%, $(uit.buildsystem.real_groups))

uit.buildsystem.core.build_groups := $(patsubst %,build_%, $(uit.buildsystem.real_groups))

uit.buildsystem.core.clean_groups := $(patsubst %,clean_%, $(uit.buildsystem.real_groups))

uit.buildsystem.core.astyle_groups := $(patsubst %,astyle_%, $(uit.buildsystem.real_groups))

uit.buildsystem.core.cpplint_groups := $(patsubst %,cpplint_%, $(uit.buildsystem.real_groups))

uit.buildsystem.core.init_targets := $(patsubst %,init_%, $(uit.buildsystem.real_targets))

uit.buildsystem.core.init_depends := $(patsubst %,init_depends_%, $(uit.buildsystem.real_targets))

uit.buildsystem.core.update_targets := $(patsubst %,update_%, $(uit.buildsystem.real_targets))

uit.buildsystem.core.update_depends := $(patsubst %,update_depends_%, $(uit.buildsystem.real_targets))

uit.buildsystem.core.build_targets := $(uit.buildsystem.real_targets)

uit.buildsystem.core.clean_targets := $(patsubst %,clean_%, $(uit.buildsystem.real_targets))

uit.buildsystem.core.astyle_targets := $(patsubst %,astyle_%, $(uit.buildsystem.real_targets))

uit.buildsystem.core.cpplint_targets := $(patsubst %,cpplint_%, $(uit.buildsystem.real_targets))

# [uit.buildsystem.<function> ]

define uit.buildsystem.env_validator
	@$(if $(uit.buildsystem.build_dir),,$(error 'uit.buildsystem.build_dir' not been set))
endef

define uit.buildsystem.depends
    $(shell test -f $(1)/BuildSystem.Depends && cat $(1)/BuildSystem.Depends)
endef

define uit.buildsystem.init
	@if [ ! -d $(1) ]; then \
		echo "[uit.buildsystem.init] target $(1)"; \
		git clone $(uit.buildsystem.repo)/$(1).git; \
		if [ `git --git-dir $(1)/.git branch --list -r origin/$(uit.buildsystem.baseline)` ]; then \
			git --git-dir=$(1)/.git --work-tree=$(1) checkout -q $(uit.buildsystem.baseline); \
		fi \
	fi
endef

define uit.buildsystem.init_depends
	@if [ -f $(1)/BuildSystem.Depends ] \
		&& [ ! -z "$(strip $(call uit.buildsystem.depends,$(1)))" ]; then \
		$(uit.buildsystem.utils.make) $(foreach target,$(call uit.buildsystem.depends,$(1)),init_$(target)); \
		$(uit.buildsystem.utils.make) $(foreach target,$(call uit.buildsystem.depends,$(1)),init_depends_$(target)); \
	fi
endef

define uit.buildsystem.update
	@if [ -d $(1) ]; then \
		echo "[uit.buildsystem.update] target $(1)"; \
		$(uit.buildsystem.utils.update) $(1); \
	fi
endef

define uit.buildsystem.update_depends
	@if [ -f "$(1)/BuildSystem.Depends" ] \
		&& [ ! -z "$(strip $(shell cat $(1)/BuildSystem.Depends))" ]; then \
		$(uit.buildsystem.utils.make) $(foreach target,$(call uit.buildsystem.depends,$(1)),update_$(target)); \
		$(uit.buildsystem.utils.make) $(foreach target,$(call uit.buildsystem.depends,$(1)),update_depends_$(target)); \
	fi
endef

define uit.buildsystem.build
	@if [ ! -z $(findstring $(1),$(uit.buildsystem.selfmake)) ]; then \
		echo "[uit.buildsystem.build] target $(1) in selfmake, jobs = $(uit.buildsystem.jobs)"; \
		cd $(1) && $(uit.buildsystem.utils.parallel_make) && $(uit.buildsystem.utils.parallel_make) install; \
	else \
		echo "[uit.buildsystem.build] target $(1), jobs = $(uit.buildsystem.jobs)"; \
		mkdir -p $(1)/$(uit.buildsystem.build_dir) && cd $(1)/$(uit.buildsystem.build_dir) && \
		$(uit.buildsystem.utils.cmake) ../.. && $(uit.buildsystem.utils.parallel_make) && $(uit.buildsystem.utils.parallel_make) install; \
	fi
endef

define uit.buildsystem.clean
	@if [ -d $(1) ]; then \
		if [ ! -z $(findstring $(1),$(uit.buildsystem.selfmake)) ]; then \
			echo "[uit.buildsystem.clean] target $(1) in selfmake"; \
			cd $(1) && $(uit.buildsystem.utils.parallel_make) clean; \
		else \
			echo "[uit.buildsystem.clean] target $(1)"; \
			rm -rf $(1)/$(uit.buildsystem.build_dir); \
		fi \
    fi
endef

define uit.buildsystem.astyle
	@if [ -d $(1) ]; then \
		echo "[uit.buildsystem.astyle] target $(1)"; \
		$(uit.buildsystem.utils.astyle) $(1); \
	fi
endef

define uit.buildsystem.cpplint
	@if [ -d $(1) ]; then \
		echo "[uit.buildsystem.cpplint] target $(1)"; \
		$(uit.buildsystem.utils.cpplint) $(1); \
	fi
endef

# [uit.buildsystem.core.<common> ]

# Make sure 'all' is the 1st target, 'make' use 1st target as the default target
.PHONY: all env_validator init update build clean astyle cpplint

all: build

env_validator:
	$(call uit.buildsystem.env_validator)

init:

update:

build:

clean:

astyle:

cpplint:

# [uit.buildsystem.core.<targets> ]

.PHONY: $(uit.buildsystem.core.init_targets)

$(uit.buildsystem.core.init_targets):
	$(call uit.buildsystem.init,$(subst init_,,$@))

.PHONY: $(uit.buildsystem.core.init_depends)

$(uit.buildsystem.core.init_depends):
	$(call uit.buildsystem.init_depends,$(subst init_depends_,,$@))

.PHONY: $(uit.buildsystem.core.update_targets)

$(uit.buildsystem.core.update_targets):
	$(call uit.buildsystem.update,$(subst update_,,$@))

.PHONY: $(uit.buildsystem.core.update_depends)

$(uit.buildsystem.core.update_depends):
	$(call uit.buildsystem.update_depends,$(subst update_depends_,,$@))

.PHONY: $(uit.buildsystem.core.build_targets)

$(uit.buildsystem.core.build_targets):
	$(call uit.buildsystem.build,$@)

.PHONY: $(uit.buildsystem.core.clean_targets)

$(uit.buildsystem.core.clean_targets):
	$(call uit.buildsystem.clean,$(subst clean_,,$@))

.PHONY: $(uit.buildsystem.core.astyle_targets)

$(uit.buildsystem.core.astyle_targets):
	$(call uit.buildsystem.astyle,$(subst astyle_,,$@))

.PHONY: $(uit.buildsystem.core.cpplint_targets)

$(uit.buildsystem.core.cpplint_targets):
	$(call uit.buildsystem.cpplint,$(subst cpplint_,,$@))

# [uit.buildsystem.core.<targets> ] update common depends

init: $(uit.buildsystem.core.init_targets)

update: $(uit.buildsystem.core.update_targets)

build: $(uit.buildsystem.core.build_targets)

clean: $(uit.buildsystem.core.clean_targets)

astyle: $(uit.buildsystem.core.astyle_targets)

cpplint: $(uit.buildsystem.core.cpplint_targets)

# [uit.buildsystem.core.<groups> ]

.PHONY: $(uit.buildsystem.core.init_groups)

$(uit.buildsystem.core.init_groups):
	cd $(subst init_,,$@) && $(uit.buildsystem.utils.parallel_make) init;

.PHONY: $(uit.buildsystem.core.update_groups)

$(uit.buildsystem.core.update_groups):
	cd $(subst update_,,$@) && $(uit.buildsystem.utils.parallel_make) update;

$(uit.buildsystem.core.update_groups): uit.buildsystem.jobs = $(uit.buildsystem.git_jobs)

.PHONY: $(uit.buildsystem.core.build_groups)

$(uit.buildsystem.core.build_groups):
	cd $(subst build_,,$@) && $(uit.buildsystem.utils.parallel_make);

.PHONY: $(uit.buildsystem.core.clean_groups)

$(uit.buildsystem.core.clean_groups):
	cd $(subst clean_,,$@) && $(uit.buildsystem.utils.parallel_make) clean;

.PHONY: $(uit.buildsystem.core.astyle_groups)

$(uit.buildsystem.core.astyle_groups):
	cd $(subst astyle_,,$@) && $(uit.buildsystem.utils.parallel_make) astyle;

.PHONY: $(uit.buildsystem.core.cpplint_groups)

$(uit.buildsystem.core.cpplint_groups):
	cd $(subst cpplint_,,$@) && $(uit.buildsystem.utils.parallel_make) cpplint;

# [uit.buildsystem.core.<groups> ] update common depends

init: $(uit.buildsystem.core.init_groups)

update: $(uit.buildsystem.core.update_groups)

update: uit.buildsystem.jobs = $(uit.buildsystem.git_jobs)

build: $(uit.buildsystem.core.build_groups)

clean: $(uit.buildsystem.core.clean_groups)

astyle: $(uit.buildsystem.core.astyle_groups)

cpplint: $(uit.buildsystem.core.cpplint_groups)

# [uit.buildsystem.update_depends ]

# Update Depends refer to 'BuildSystem.Depends'
define uit.buildsystem.template.update_depends

init_$(1): env_validator

init_depends_$(1): env_validator

update_$(1): env_validator

update_depends_$(1): env_validator

update_depends_$(1): uit.buildsystem.jobs = 1

$(1): env_validator

clean_$(1): env_validator

astyle_$(1): env_validator

cpplint_$(1): env_validator

$(1): init_$(1)

update_$(1): init_$(1)

$(1): $$(call uit.buildsystem.depends,$(1))

endef

$(foreach target,$(uit.buildsystem.real_targets),$(eval $(call uit.buildsystem.template.update_depends,$(target))))

# [uit.buildsystem.export_blacklist ]

define uit.buildsystem.template.export_blacklist

init_$(1):
	@echo "[uit.buildsystem.init] target $(1) in blacklist, skip it!";

init_depends_$(1):
	@echo "[uit.buildsystem.init_depends] target $(1) in blacklist, skip it!";

update_$(1):
	@echo "[uit.buildsystem.update] target $(1) in blacklist, skip it!";

update_depends_$(1):
	@echo "[uit.buildsystem.update_depends] target $(1) in blacklist, skip it!";

$(1):
	@echo "[uit.buildsystem.build] target $(1) in blacklist, skip it!";

clean_$(1):
	@echo "[uit.buildsystem.clean] target $(1) in blacklist, skip it!";

astyle_$(1):
	@echo "[uit.buildsystem.astyle] target $(1) in blacklist, skip it!";

cpplint_$(1):
	@echo "[uit.buildsystem.cpplint] target $(1) in blacklist, skip it!";

endef

$(foreach target,$(uit.buildsystem.blacklist),$(eval $(call uit.buildsystem.template.export_blacklist,$(target))))

# Create group entries, $1: group; $2: depends
define uit.buildsystem.template.update_group_depends

$(1): $(2)

#init_$(1): init_$(2)

update_$(1): uit.buildsystem.jobs = $(uit.buildsystem.cpu_cores)

build_$(1): build_$(2)

#clean_$(1): clean_$(2)

#astyle_$(1): astyle_$(2)

#cpplint_$(1): cpplint_$(2)

endef

# Ignore self make install, do nothing
.PHONY: install
install:

# Var print targets
var-%:
	@echo '$(value  $*)'
