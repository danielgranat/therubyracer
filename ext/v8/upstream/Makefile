
SCONS=build/scons/install/bin/scons
SCONSSRC=build/scons
V8SRC=build/v8
V8VERSION=3.1.8
LIBV8=build/v8/libv8.a
LIBV8_G=build/v8/libv8_g.a
GCC_VERSION=$(shell ruby -e 'puts %x{gcc --version} =~ /(\d)\.(\d)\.\d/ ? $$1 + $$2 : "UNKNOWN"')
ARCH=$(shell ruby detect_cpu.rb)

all: $(LIBV8)

debug: $(LIBV8_G)
	cp $(LIBV8_G) $(LIBV8)

$(LIBV8): $(SCONS) $(V8SRC)
	cd build/v8 && GCC_VERSION=$(GCC_VERSION) ../scons/install/bin/scons arch=$(ARCH)

$(LIBV8_G): $(SCONS) $(V8SRC)
	cd build/v8 && GCC_VERSION=$(GCC_VERSION) ../scons/install/bin/scons arch=$(ARCH) mode=debug

$(SCONS): $(SCONSSRC)
	mkdir -p $(SCONSSRC)/install
	python build/scons/setup.py install --prefix=install

$(V8SRC): build
	cp -r $(V8VERSION) build/v8
	patch -td build/v8 -i ../../fpic-on-linux-amd64.patch
	patch -td build/v8/src -i ../../../platform-thread-missing-destructor.patch

$(SCONSSRC): build
	cp -r scons build

build:
	mkdir -p build

scons: $(SCONS)

clean:
	rm -rf build
