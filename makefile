OSFLAG :=
MS     :=

ifndef PAHO_HOME
  $(error PAHO_HOME must be set to the location of your cloned and installed paho.mqtt.c repository)
endif

PAHO_LIB=$(PAHO_HOME)/lib
PAHO_SRC=$(PAHO_HOME)/include

MS      = $(shell getconf LONG_BIT)
W_OPTS  = -Wall -Wno-strict-aliasing -Wno-parentheses -Wextra -Werror -Wsign-compare
OPTS    = -DKXVER=3 -shared -fPIC $(W_OPTS)
LDOPTS  = -L$(PAHO_LIB) -lpaho-mqtt3a -lpaho-mqtt3as -lpaho-mqtt3c -lpaho-mqtt3cs
INCLUDES= -I$(PAHO_SRC)
TGT     = mqtt.so

ifeq ($(shell uname),Linux)
 LNK     = -lrt
 OSFLAG  = l
 OSXOPTS:=
 RP      = -Wl,-rpath=$(PAHO_LIB)
else ifeq ($(shell uname),Darwin)
 OSFLAG  = m
 LNK:=
 OSXOPTS = -undefined dynamic_lookup  -mmacosx-version-min=10.12
 RP:=
endif

QARCH = $(OSFLAG)$(MS)
Q     = $(QHOME)/$(QARCH)

all:src/k.h
	$(CC) src/mqtt.c $(OPTS) $(LDOPTS) $(INCLUDES) $(RP) -o $(TGT) $(OSXOPTS)

install:
	install $(TGT) $(Q)
	install mqtt.q $(QHOME)

clean:
	rm -f *.so
	rm -f src/k.h

src/k.h:
	curl -s -O -J -L https://github.com/KxSystems/kdb/raw/master/c/c/k.h
	mv k.h src
