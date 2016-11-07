#created by kamuszhou@tencent.com

root := ../
public_dir := ../public/
boost_home := /data1/home/kamuszhou/local/
#boost_home2 := /data/home/rubinwang/local/
#lib := $(HOME)/lib
lib := `echo ~`/lib
mysql_lib := /data/home/kamuszhou/lib/ /usr/local/mysql/lib/
bin := $(root)bin/

other_source_dirs += 

apppath := $(bin)$(projname)
sources := $(shell find . -name "*.cpp") $(wildcard $(public_dir)*.cpp) $(filter-out %main.cpp, $(wildcard $(addsuffix *.cpp,$(other_source_dirs))))
objects := $(subst .cpp,.o,$(sources))
dependencies := $(subst .cpp,.d,$(sources))
#lib_inc_dirs := $(notdir $(patsubst %/,%,$(shell ls -d ../../lib/*/)))
#include_dirs += . $(public_dir) $(addprefix $(root)inc/,$(lib_inc_dirs))
include_dirs += . $(public_dir) ../gleaner

#CXXFLAGS += --std=c++11 
CXXFLAGS += --std=c++0x 
#CPPFLAGS += $(addprefix -I,$(include_dirs)) -I$(boost_home)include -I$(boost_home2)include
CPPFLAGS += $(addprefix -I,$(include_dirs)) -I$(boost_home)include -DVERSION_MAJOR=1 -DVERSION_MINOR=0
LINKFLAGS += -static -L$(lib) $(addprefix -L,$(mysql_lib)) -lmysqlcppconn-static -lmysqlclient -lseclog -lsecneocomm -L$(boost_home)lib -lboost_filesystem -lboost_system -lboost_regex -lboost_thread -lboost_program_options -lboost_date_time -lpthread -lrt -ldl -lz -lcrypto -ldl -lsodium
#LINKFLAGS += -Wl,-Bstatic -L$(lib) $(addprefix -L,$(mysql_lib)) -lmysqlcppconn-static -lmysqlclient -lseclog -lbon2 -lsecneocomm -L$(boost_home)lib -lboost_filesystem -lboost_system -lboost_regex -lboost_thread -lboost_program_options -lboost_date_time -lpthread -lrt -ldl -lz -lcrypto

vpath %.cpp .
vpath %.h $(include_dirs)

.PHONY: clean

$(apppath): $(objects)
	test -d $(bin) || mkdir -p $(bin)
	g++ -o $@ $(objects) $(LINKFLAGS)
	md5sum $@
	
ifneq "$(MAKECMDGOAL)" "clean"
  -include $(dependencies)
endif

#$(call make-depend,source-file,object-file,depend-file) 
# -MM option causes g++ to omit "system" headers from the prerequisites list.
# This option implies -E which will stop after the preprocessing stage; do not
# run the compiler proper. 
# -MF option specifies the dependecy filename.
# -MT target change the target of the rule emitted by dependency generation.
define make-depend
   g++ -MM -MF $3 -MT $2 $(CPPFLAGS) $(TARGET_ARCH) $1
endef

%.o : %.cpp
	$(call make-depend, $<,$@,$(subst .cpp,.d,$<))
	$(COMPILE.C) $(OUTPUT_OPTION) $<
#	g++ $(CXXFLAGS) $(CPPFLAGS) $< # lacks -c -o 

clean :
	rm -rf *.o *.d $(apppath) *.log $(public_dir)*.o $(public_dir)*.d $(dependencies) $(objects)
	-find . -name *.o -delete
	-find . -name *.d -delete

