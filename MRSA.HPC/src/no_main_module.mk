SOURCES = CSVReader.cpp \
          Activity.cpp \
          TransmissionAlgorithm.cpp \
          DiseaseStatusUpdater.cpp \
          Person.cpp \
          PersonsCreator.cpp \
          SummaryInfectionStats.cpp \
          Place.cpp \
          SelectiveTransmissionPlace.cpp \
          AbstractPlace.cpp \
          School.cpp \
          Household.cpp \
          Workplace.cpp \
          DefaultPlace.cpp \
          PlaceCreator.cpp \
          ActivityCreator.cpp \
          PersonStats.cpp \
          MRSAObserver.cpp 
          
          
         
local_dir := src
local_src := $(addprefix $(local_dir)/, $(SOURCES))

sources += $(local_src)



