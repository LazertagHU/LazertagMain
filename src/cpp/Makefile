#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES +=  ButtonTaak.cpp InputControlTaak.cpp KeypadTaak.cpp PlayerInfo.cpp runGameTaak.cpp SendTask.cpp pause_detector.cpp DisplayTaak.cpp msg_decoder.cpp SpeakerTaak.cpp

# header files in this project

HEADERS += ButtonTaak.hpp hit.hpp InputControlTaak.hpp InputListener.hpp KeypadListener.hpp KeypadTaak.hpp msg_listener.hpp msg_decoder.hpp pause_listener.hpp pause_detector.hpp PlayerInfo.hpp runGameTaak.hpp SendTask.hpp weapon.hpp DisplayTaak.hpp SpeakerTaak.hpp

# other places to look for files for this project
SEARCH  := ../../lib

# set REATIVE to the next higher directory 
# and defer to the Mak defile.due there
RELATIVE := $(RELATIVE)../
include $(RELATIVE)Makefile.due
