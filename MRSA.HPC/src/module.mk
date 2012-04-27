SOURCES = main.cpp
         
local_dir := src
local_src := $(addprefix $(local_dir)/, $(SOURCES))

sources += $(local_src)
bin_dir = "bin"
program = "mrsa_model"



