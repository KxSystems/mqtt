OSFLAG :=
MS     :=

PAHO_HOME=./paho.mqtt.c
PAHO_LIB=$(PAHO_HOME)/build/output
PAHO_SRC=$(PAHO_HOME)/src
MS = $(shell getconf LONG_BIT)
OPTS = -DKXVER=3 -shared -fPIC
LDOPTS=-L$(PAHO_LIB) -lpaho-mqtt3a -lpaho-mqtt3as -lpaho-mqtt3c -lpaho-mqtt3cs
INCLUDES=-I$(PAHO_SRC)
TGT = mqtt.so

ifeq ($(shell uname),Linux)
 LNK     = -lrt
 OSFLAG  = l
 OSXOPTS:=
 RP = -Wl,-rpath=$(PAHO_LIB)
else ifeq ($(shell uname),Darwin)
 OSFLAG  = m
 LNK:=
 OSXOPTS = -undefined dynamic_lookup  -mmacosx-version-min=10.12
 RP:=
endif

QARCH = $(OSFLAG)$(MS)
Q = $(QHOME)/$(QARCH)

all:k.h
	$(CC) mqtt.c $(OPTS) $(LDOPTS) $(INCLUDES) $(RP) -o $(TGT) $(OSXOPTS)

install:
	install $(TGT) $(Q)

clean:
	rm -f mqtt.so

k.h:
	curl -s -O -J -L https://github.com/KxSystems/kdb/raw/master/c/c/k.h
