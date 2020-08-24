MODE = debug
QMAKE = qmake
RTAI = 

all: pdae pdif pdppt pdme bin/lcd bin/knob bin/rtview pdppt-lib docs

default: pdae pdif pdppt pdme bin/lcd bin/knob bin/rtview pdppt-lib

bin/lcd:src/atomics/lcd/* 
	cd ./src/atomics/lcd && $(QMAKE) CONFIG+=$(MODE) 
	make -C ./src/atomics/lcd 

bin/knob:src/atomics/knob/* 
	cd ./src/atomics/knob && $(QMAKE) CONFIG+=$(MODE) 
	make -C ./src/atomics/knob 

bin/rtview:src/atomics/rtview
	cd ./src/atomics/rtview && $(QMAKE) CONFIG+=$(MODE) 
	make -C ./src/atomics/rtview 

pdae:
	cd ./src/pdae && $(QMAKE) CONFIG+=$(MODE) 
	make -C ./src/pdae 

pdppt:
	cd ./src/pdppt && $(QMAKE) CONFIG+=$(MODE) 
	make -C ./src/pdppt 

pdppt-lib:
	cd ./src/lib/pdppt && $(QMAKE) CONFIG+=$(MODE) 
	make -C ./src/pdppt 

install-lib: pdppt-lib
	make -C ./build install
	make -C ./src/lib/pdppt  install



pdif:
ifeq ($(RTAI),yes)
	cd ./src/pdif && $(QMAKE) CONFIG+=$(MODE) $(QMAKE)_CXXFLAGS+=-DRTAIOS 
else
	cd ./src/pdif && $(QMAKE) CONFIG+=$(MODE) 
endif
	make -C ./src/pdif 

docs: src/doc/commands.tex src/doc/pd_userguide.kilepr src/doc/chap_intro.tex src/doc/images src/doc/images/pdevsmodel.jpg src/doc/images/pdmodeling.jpg src/doc/images/pdsimulation.jpg src/doc/chap_get_start.tex src/doc/PD_UserGuide.tex
	cd ./src/doc && pdflatex PD_UserGuide.tex -output-directory ../../doc
	cp ./src/doc/PD_UserGuide.pdf ./doc


pdme:
	cd ./src/pdme_v2 && $(QMAKE) CONFIG+=$(MODE) 
	make -C ./src/pdme_v2 

clean:
	make -C ./src/pdae clean
	make -C ./src/pdif clean
	make -C ./src/pdppt clean
	make -C ./src/lib/pdppt clean
	make -C ./src/pdme_v2 clean
	make -C ./src/pdme_v2 clean
	make -C ./src/atomics/lcd clean
	make -C ./src/atomics/knob clean
	make -C ./src/atomics/rtview clean
	rm -rf bin/pdae bin/pdppt bin/pdif bin/pdme bin/lcd bin/knob bin/rtview doc/*
