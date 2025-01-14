####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

PACKAGES = boost capicxx-core-runtime vsomeip capicxx-someip-runtime poco

clean_PACKAGES = $(patsubst %,clean_%, $(PACKAGES))

.PHONY: all clean $(PACKAGES) $(clean_PACKAGES)

all: $(PACKAGES)
clean: $(clean_PACKAGES)

dist=$(shell pwd)/../../dist/$(UIT_TARGET)/release
boost_exist=$(shell if [ -d boost ]; then echo "exist"; else echo "notexist"; fi;)
ccr_exist=$(shell if [ -d capicxx-core-runtime ]; then echo "exist"; else echo "notexist"; fi;)
vs_exist=$(shell if [ -d vsomeip ]; then echo "exist"; else echo "notexist"; fi;)
vsr_exist=$(shell if [ -d capicxx-someip-runtime ]; then echo "exist"; else echo "notexist"; fi;)
poco_exist=$(shell if [ -d poco ]; then echo "exist"; else echo "notexist"; fi;)

#boost 1.61.0 is not valid for vsomeip, don't known why:server can't not startup
boost_ver=1.61.0
boost_pkg=boost_$(subst .,_,$(boost_ver))
boost_tar=$(boost_pkg).tar.gz
#			https://ufpr.dl.sourceforge.net/project/boost/boost/1.61.0/boost_1_61_0.tar.gz -o boost_1_61_0.tar.gz
boost_url=https://ufpr.dl.sourceforge.net/project/boost/boost/$(boost_ver)/$(boost_tar)
boost:
ifneq ($(boost_exist), exist)
	#@curl --insecure $(boost_url) -o $(boost_tar) --retry 10000 && \
	tar xvf $(boost_tar) 1> /dev/null && \
	rm -rf $@ && mv $(boost_pkg) $@
endif
	@cd $@ && ./bootstrap.sh && \
	./b2 --buildtype=complete variant=release

capicxx-core-runtime:
ifneq ($(ccr_exist), exist)
	@git clone https://github.com/GENIVI/$@.git && cd $@ && \
	git checkout 2fd0625
endif
	@cd $@ && \
	mkdir -p $(UIT_BUILD_DIR) && cd $(UIT_BUILD_DIR) && \
	cmake ../.. -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=$(dist) && \
	make && make install

vsomeip:
ifneq ($(vs_exist), exist)
	@git clone https://github.com/GENIVI/$@.git && cd $@ && \
	git checkout 9fb9bee && git apply ../$@.patch
endif
	@cd $@ && \
	mkdir -p $(UIT_BUILD_DIR) && cd $(UIT_BUILD_DIR) && \
	cmake ../.. -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=$(dist) && \
	make && make install

capicxx-someip-runtime:
ifneq ($(vsr_exist), exist)
	@git clone https://github.com/GENIVI/$@.git && cd $@ && \
	git checkout 64f0cb0 && git apply ../$@.patch
endif
	@cd $@ && \
	mkdir -p $(UIT_BUILD_DIR) && cd $(UIT_BUILD_DIR) && \
	cmake ../.. -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=$(dist) && \
	make && make install

poco:
ifneq ($(poco_exist), exist)
	@git clone https://github.com/pocoproject/$@.git && cd $@ && \
	git checkout 13eda19
endif
#	@cd $@ && \
#	./configure --config=Linux --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --shared --no-tests --no-samples --prefix=$(dist) && \
#	make && make install
	@cd $@ && \
	mkdir -p $(UIT_BUILD_DIR) && cd $(UIT_BUILD_DIR) && \
	cmake ../.. -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=$(dist) && \
	make && make install

$(clean_PACKAGES):
	rm -rf $(subst clean_,,$@)



