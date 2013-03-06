SOURCES = CSVReader.cpp \
		  Parameters.cpp \
		  Calendar.cpp \
          Activity.cpp \
          TransmissionAlgorithm.cpp \
          DiseaseStatusUpdater.cpp \
          Person.cpp \
          HospitalStayManager.cpp \
          PersonsCreator.cpp \
          Place.cpp \
          SelectiveTransmissionPlace.cpp \
          AbstractPlace.cpp \
          School.cpp \
          Household.cpp \
          Workplace.cpp \
          Hospital.cpp \
          DefaultPlace.cpp \
          PlaceCreator.cpp \
          ActivityCreator.cpp \
          Statistics.cpp \
          MRSAObserver.cpp 
          
          
         
local_dir := src
local_src := $(addprefix $(local_dir)/, $(SOURCES))

sources += $(local_src)



