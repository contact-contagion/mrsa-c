#-------------------------------------------------------------------------------
# MRSA Model
# 
# Copyright (c) 2012 University of Chicago and Argonne National Laboratory
#    All rights reserved.
#   
#    Redistribution and use in source and binary forms, with 
#    or without modification, are permitted provided that the following 
#    conditions are met:
#   
#   	 Redistributions of source code must retain the above copyright notice,
#   	 this list of conditions and the following disclaimer.
#   
#   	 Redistributions in binary form must reproduce the above copyright notice,
#   	 this list of conditions and the following disclaimer in the documentation
#   	 and/or other materials provided with the distribution.
#   
#   	 Neither the name of the Argonne National Laboratory nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#   
#    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#    ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
#    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
#    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#-------------------------------------------------------------------------------
SOURCES = CSVReader.cpp \
		  Parameters.cpp \
		  RegionMap.cpp \
		  FileOutput.cpp \
		  utility.cpp \
		  Calendar.cpp \
          Activity.cpp \
          TransmissionAlgorithm.cpp \
          DiseaseStatusUpdater.cpp \
          Person.cpp \
          AbstractStayManager.cpp \
          HospitalStayManager.cpp \
          PrisonStayManager.cpp \
          PersonsCreator.cpp \
          Place.cpp \
          SelectiveTransmissionPlace.cpp \
          CompositePlace.cpp \
          AbstractPlace.cpp \
          School.cpp \
          Household.cpp \
          Workplace.cpp \
          Hospital.cpp \
          DefaultPlace.cpp \
          Gym.cpp \
          GeneralQuarters.cpp \
          Prison.cpp \
          PlaceCreator.cpp \
          TransmissionEventRecorder.cpp \
          ActivityCreator.cpp \
          PlaceStats.cpp \
          DataSourceAdapters.cpp \
          Statistics.cpp \
          MRSAObserver.cpp 
          
          
         
local_dir := src
local_src := $(addprefix $(local_dir)/, $(SOURCES))

sources += $(local_src)



