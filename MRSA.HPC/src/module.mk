SOURCES = main.cpp \
          CSVReader.cpp \
          Activity.cpp \
          Person.cpp \
          PersonsCreator.cpp \
          Place.cpp \
          PlaceCreator.cpp \
          ActivityCreator.cpp \
          PersonStats.cpp \
          MRSAObserver.cpp \
          
          
         
local_dir := src
local_src := $(addprefix $(local_dir)/, $(SOURCES))

sources += $(local_src)
bin_dir = "bin"
program = "mrsa_model"



