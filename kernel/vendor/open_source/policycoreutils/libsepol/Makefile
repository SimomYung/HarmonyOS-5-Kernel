DISABLE_CIL ?= n
ENABLE_SEHARMONY_POLICY_SPLIT ?= n
ENABLE_SEHARMONY ?= n
ENABLE_HKIP_SEHARMONY_PROT ?= n

export DISABLE_CIL
export ENABLE_SEHARMONY_POLICY_SPLIT
export ENABLE_SEHARMONY
export ENABLE_HKIP_SEHARMONY_PROT

all: 
	$(MAKE) -C src 
	$(MAKE) -C utils

install: 
	$(MAKE) -C include install
	$(MAKE) -C src install
	$(MAKE) -C utils install
	$(MAKE) -C man install

relabel:
	$(MAKE) -C src relabel

clean:
	$(MAKE) -C src clean
	$(MAKE) -C utils clean
	$(MAKE) -C tests clean

indent:
	$(MAKE) -C src $@
	$(MAKE) -C include $@
	$(MAKE) -C utils $@

test:
	$(MAKE) -C tests test

