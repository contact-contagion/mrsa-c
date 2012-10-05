program :=
sources :=
libraries :=
extra_clean :=
bin_dir :=

CXX = /usr/local/bin/mpicxx
BOOST_INCLUDE = /usr/local/include/boost/
LIBS = -lboost_mpi -lboost_serialization -lboost_filesystem -lboost_system -lnetcdf -lcurl -lnetcdf_c++ -lmpi -lrelogo-1.0.1 -lrepast_hpc-1.0.1 $(libraries)

objects = $(subst .cpp,.o,$(sources))
dependencies = $(subst .cpp,.d,$(sources))

include_dirs := $(BOOST_INCLUDE)  src

# un-opt flags
CPPFLAGS += $(addprefix -I, $(include_dirs)) -O0 -g3 -Wall -c -fmessage-length=0

# opt flags
#CPPFLAGS += $(addprefix -I, $(include_dirs)) -O3 -g0 -Wall -c -fmessage-length=0
#vpath %.h $(include_dirs)

RM := rm -f
SED := sed
MV := mv -f

#%.o : %.cpp
#	$(CC) $(CPPFLAGS) -o "$@" "$<"

include src/no_main_module.mk

ifeq "$(MAKECMDGOALS)" "unit_tests"
 include test/module.mk
	
endif

ifeq "$(MAKECMDGOALS)" "mrsa"
 include src/module.mk
endif


.PHONY: mrsa unit_tests

mrsa: $(objects)
	-mkdir -p $(bin_dir)
	$(CXX) -o $(bin_dir)/$(program) $^ $(LIBS)

unit_tests: $(objects)
	-mkdir -p $(bin_dir)
	$(CXX) -o $(bin_dir)/$(program) $^ $(LIBS)
	
	
.PHONY: clean
clean:
	$(RM) $(objects) $(bin_dir)/$(program) $(libraries) \
	$(dependencies) $(extra_clean)

ifeq "$(MAKECMDGOALS)" "clean"
 include src/no_main_module.mk
 include test/module.mk
 include src/module.mk
endif


%.d: %.cpp
	$(CXX) $(CPPFLAGS) -M $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@
	
#$(CXX) $(CPPFLAGS) -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  "$<"
