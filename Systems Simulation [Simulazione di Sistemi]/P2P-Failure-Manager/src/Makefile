#
# OMNeT++/OMNEST Makefile for P2P-Failure-Manager_-_SDS
#
# This file was generated with the command:
#  opp_makemake -f --deep -L/home/gyankos/omnetpp-4.4/samples/queueinglib/out/$$\(CONFIGNAME\) -lqueueinglib -DQUEUEING_IMPORT -KALOHA_PROJ=/home/gyankos/omnetpp-4.4/samples/aloha -KQUEUEINGLIB_PROJ=/home/gyankos/omnetpp-4.4/samples/queueinglib -- $$O/../../../graphml.o
#

# Name of target to be created (-o option)
TARGET = P2P-Failure-Manager_-_SDS$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I. -Imessages -Imodules -Iwrapper

# Additional object and library files to link with
EXTRA_OBJS = $O/../../../graphml.o

# Additional libraries (-L, -l options)
LIBS = -L$(QUEUEINGLIB_PROJ)/out/$(CONFIGNAME)  -lqueueinglib
LIBS += -Wl,-rpath,`abspath $(QUEUEINGLIB_PROJ)/out/$(CONFIGNAME)`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/modules/NodeManager.o \
    $O/modules/Peer.o \
    $O/wrapper/Graph.o \
    $O/wrapper/Vertex.o \
    $O/wrapper/Edge.o \
    $O/messages/ProtocolMessage_m.o \
    $O/messages/LinkCreationAnswer_m.o \
    $O/messages/LinkCreatedAdvice_m.o \
    $O/messages/NeighbourCrashedAdvice_m.o \
    $O/messages/LinkCreationRequest_m.o

# Message files
MSGFILES = \
    messages/ProtocolMessage.msg \
    messages/LinkCreationAnswer.msg \
    messages/LinkCreatedAdvice.msg \
    messages/NeighbourCrashedAdvice.msg \
    messages/LinkCreationRequest.msg

# Other makefile variables (-K)
ALOHA_PROJ=/home/gyankos/omnetpp-4.4/samples/aloha
QUEUEINGLIB_PROJ=/home/gyankos/omnetpp-4.4/samples/queueinglib

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) -DQUEUEING_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f P2P-Failure-Manager_-_SDS P2P-Failure-Manager_-_SDS.exe libP2P-Failure-Manager_-_SDS.so libP2P-Failure-Manager_-_SDS.a libP2P-Failure-Manager_-_SDS.dll libP2P-Failure-Manager_-_SDS.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f messages/*_m.cc messages/*_m.h
	$(Q)-rm -f modules/*_m.cc modules/*_m.h
	$(Q)-rm -f wrapper/*_m.cc wrapper/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc messages/*.cc modules/*.cc wrapper/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/messages/LinkCreatedAdvice_m.o: messages/LinkCreatedAdvice_m.cc \
	messages/LinkCreatedAdvice_m.h \
	messages/ProtocolMessage_m.h
$O/messages/LinkCreationAnswer_m.o: messages/LinkCreationAnswer_m.cc \
	messages/LinkCreationAnswer_m.h \
	messages/ProtocolMessageUtils.h \
	messages/ProtocolMessage_m.h \
	modules/NodeManager.h \
	modules/Peer.h \
	wrapper/EProp.h \
	wrapper/Graph.h \
	wrapper/VProp.h \
	wrapper/Vertex.h \
	wrapper/graph_t.h
$O/messages/LinkCreationRequest_m.o: messages/LinkCreationRequest_m.cc \
	messages/LinkCreationRequest_m.h \
	messages/ProtocolMessage_m.h
$O/messages/NeighbourCrashedAdvice_m.o: messages/NeighbourCrashedAdvice_m.cc \
	messages/NeighbourCrashedAdvice_m.h \
	messages/ProtocolMessage_m.h
$O/messages/ProtocolMessage_m.o: messages/ProtocolMessage_m.cc \
	messages/ProtocolMessage_m.h
$O/modules/NodeManager.o: modules/NodeManager.cc \
	messages/NeighbourCrashedAdvice_m.h \
	messages/ProtocolMessage_m.h \
	modules/NodeManager.h \
	modules/Peer.h \
	wrapper/EProp.h \
	wrapper/Edge.h \
	wrapper/Graph.h \
	wrapper/VProp.h \
	wrapper/Vertex.h \
	wrapper/graph_t.h
$O/modules/Peer.o: modules/Peer.cc \
	messages/LinkCreatedAdvice_m.h \
	messages/LinkCreationAnswer_m.h \
	messages/LinkCreationRequest_m.h \
	messages/NeighbourCrashedAdvice_m.h \
	messages/ProtocolMessageUtils.h \
	messages/ProtocolMessage_m.h \
	modules/NodeManager.h \
	modules/Peer.h \
	utils.h \
	wrapper/EProp.h \
	wrapper/Graph.h \
	wrapper/VProp.h \
	wrapper/Vertex.h \
	wrapper/graph_t.h
$O/wrapper/Edge.o: wrapper/Edge.cc \
	wrapper/EProp.h \
	wrapper/Edge.h \
	wrapper/Graph.h \
	wrapper/VProp.h \
	wrapper/graph_t.h
$O/wrapper/Graph.o: wrapper/Graph.cc \
	modules/Peer.h \
	wrapper/EProp.h \
	wrapper/Edge.h \
	wrapper/Graph.h \
	wrapper/VProp.h \
	wrapper/Vertex.h \
	wrapper/graph_t.h
$O/wrapper/Vertex.o: wrapper/Vertex.cc \
	modules/Peer.h \
	wrapper/EProp.h \
	wrapper/Edge.h \
	wrapper/Graph.h \
	wrapper/VProp.h \
	wrapper/Vertex.h \
	wrapper/graph_t.h

