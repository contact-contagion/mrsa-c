SOURCES = main.cpp \
		creators_tests.cpp \
		transmission_tests.cpp \
#		data_tests.cpp 
          
          
         
local_dir := test
local_src := $(addprefix $(local_dir)/, $(SOURCES))

libraries += -lboost_unit_test_framework

sources += $(local_src)
bin_dir = "test_bin"
program = "unit_tests"



