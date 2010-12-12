#==============================================================================#
#                                                                              #
#                          Diatom Build Framework v1.2                         #
#                                      .                                       #
#==============================================================================#

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                                                                              #
#  This build framework has been designed to support quite large projects, as  #
#  well as a variety of languages. (C, C++, Java, as well as Lex/Yacc files.)  #
#                                                                              #
#  For full documentation on this Makefile, see the document README.diatom in  #
#  this directory, or online at: http://diatom.com/build                       #
#                                                                              #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

# ------------------------------------ #
#  (0) Zinc specific globals           #
# ------------------------------------ #

NAMESPACE := zinc
C_STD     := gnu99
# MULTIPROJECT := 1 not for zinc!

# ------------------------------------ #
#  (1) Supported Platforms.            #
# ------------------------------------ #

APPLE := APPLE
LINUX := linux

PLATFORM := $(strip                                                          \
	 $(findstring $(APPLE),                                              \
	     $(shell cpp -dM < /dev/null | awk '/__APPLE__/ {print $$2}' ) ) \
	 $(findstring $(LINUX),                                              \
	     $(shell cpp -dM < /dev/null | awk '/__LINUX__/ {print $$2}' ) ) )

# ------------------------------------ #
#  (2) Directory Configration          #
# ------------------------------------ #

SHELL        := /bin/bash
MODULE       := $(shell basename `pwd`)
SRC_DIR      := $(shell pwd)

TMP_PAR      := $(SRC_DIR:%/$(MODULE)=%)
FAMILY       := $(shell basename $(TMP_PAR))
TMP_SRC      := $(TMP_PAR:%/$(FAMILY)=%)
SRC_ROOT     := $(shell basename $(TMP_SRC))
ROOT_DIR     := $(TMP_SRC:%/$(SRC_ROOT)=%)

BUILD_ROOT   := $(ROOT_DIR)/build
BUILD_DIR    := $(BUILD_ROOT)/$(MODULE)
ARCHIVE_DIR  := $(ROOT_DIR)/lib/
INCLUDE_DIR  := $(ROOT_DIR)/include
ifdef NAMESPACE 
  EXPORT_DIR   := $(INCLUDE_DIR)/$(NAMESPACE)
else
  EXPORT_DIR   := $(INCLUDE_DIR)
endif  
BIN_DIR      := $(ROOT_DIR)/bin
INSTALL_BASE := /opt/bin

CLASSPATH    := $(BUILD_ROOT)

ifdef JAVA_PKG
  JAVA_PATH := $(subst .,/,$(JAVA_PKG))
endif

ifdef MULTIPROJECT
  APP_DIR      := $(BIN_DIR)/$(PROJECT)
  INSTALL_DIR  := $(INSTALL_BASE)/$(PROJECT)
else
  APP_DIR      := $(BIN_DIR)
  INSTALL_DIR  := $(INSTALL_BASE)
endif


# ------------------------------------ #
#  (3) Application Configuration       #
# ------------------------------------ #

JXX  := javac -classpath $(CLASSPATH)
CC   := gcc
CXX  := g++
AR   := ar
JAR  := jar
LEX  := flex
YACC := bison


# ------------------------------------ #
#  (4) Target Conversion               #
# ------------------------------------ #

ifdef LYOBJ
      OBJECT_SOURCES := $(OBJECT_SOURCES) $(LYOBJ).tab.c $(LYOBJ).lex.c 
endif

OBJECTS     := $(patsubst %.cpp,%.o,$(filter %.cpp,$(OBJECT_SOURCES)))     \
               $(patsubst %.c,%.o,$(filter %.c,$(OBJECT_SOURCES)))         

JCLASSES    := $(patsubst %.java,%.class,$(filter %.java,$(OBJECT_SOURCES)))
 
APPS        := $(patsubst %.cpp,%.cppapp,$(filter %.cpp,$(APP_SOURCES)))   \
               $(patsubst %.c,%.capp,$(filter %.c,$(APP_SOURCES)))         \
               $(patsubst %.java,%.jbin,$(filter %.java,$(APP_SOURCES)))

ifdef LYAPP
      APPS := $(APPS) $(LYAPP).lyapp
endif


TEST_APPS   := $(patsubst %.cpp,%.cpptapp,$(filter %.cpp,$(TEST_SOURCES))) \
               $(patsubst %.c,%.ctapp,$(filter %.c,$(TEST_SOURCES)))       

JTEST_APPS  := $(patsubst %.java,%.jtbin,$(filter %.java,$(TEST_SOURCES))) 

BUILT_OBJS  := $(OBJECTS:%=$(BUILD_DIR)/%)
HEADER_TARG := $(PUBLIC_HEADERS:%.h=$(EXPORT_DIR)/%.h)


# ------------------------------------ #
#  (4) Compile Mode Configuration      #
# ------------------------------------ #

ifeq ($(COMPILE_MODE), lib)
	AR_TARG   := $(if $(strip $(OBJECTS)),lib$(MODULE).a)
	SO_TARG   := $(if $(strip $(OBJECTS)),lib$(MODULE).so)
	JAR_TARG  := $(if $(strip $(JCLASSES)),$(MODULE).jar)
	UNITTEST  := $(MODULE)-test
	ifeq ($(PLATFORM),$(APPLE))
    	    LIB_FLAGS := -shared -undefined suppress -flat_namespace
	else 
    	    LIB_FLAGS := -shared
	endif	    
endif

ifdef AR_TARG
	OBJECT_ARCHIVE := $(BUILD_DIR)/$(AR_TARG)
endif
ifdef SO_TARG
	SHARED_LIBRARY := $(BUILD_DIR)/$(SO_TARG)
endif
ifdef JAR_TARG	
	JARCHIVE := $(ARCHIVE_DIR)/$(JAR_TARG)
endif


# ------------------------------------ #
#  (5) Convenience Replacements        #
# ------------------------------------ #

CPPAPP  = $(patsubst %.cppapp,%,$@)
CAPP    = $(patsubst %.capp,%,$@)
CPPTAPP = $(patsubst %.cpptapp,%,$@)
CTAPP   = $(patsubst %.ctapp,%,$@)
JAVAPP  = $(patsubst %.jbin,%,$@)
JTAPP   = $(patsubst %.jtbin,%,$@)


# ------------------------------------ #
#  (6) Flags and other Makefile        #
# ------------------------------------ #

ifndef THIRD_PARTY_INCLUDES
  THIRD_PARTY_INCLUDES :=
else
  THIRD_PARTY_INCLUDES := $(THIRD_PARTY_INCLUDES) 
endif

ifndef THIRD_PARTY_LIBS
  THIRD_PARTY_LIBS := -lpthread
else
  THIRD_PARTY_LIBS := -lpthread $(THIRD_PARTY_LIBS)
endif

# Some platforms want -lpthread as a comp flag.
ifndef COMP_FLAGS
  COMP_FLAGS := -W -Wall -Wshadow -Wpointer-arith -Wcast-qual $(ADDTL_FLAGS)
endif

ifndef CPP_FLAGS
  CPP_FLAGS  := -Wreorder -Woverloaded-virtual 
endif

ifndef C_FLAGS
  C_FLAGS    := $(strip -fPIC -std=$(C_STD) -funsigned-char \
                        -Wbad-function-cast -Wmissing-prototypes )
endif

ifndef LINK_DYNAMIC
  LINK_LOCAL := 
  LINK_LOCAL_END := 
endif

# Debugging memory? dmalloc isn't bad...
# Use for mem debugging: -DDMALLOC

ifdef DEBUG
  DBG_FLAGS := -ggdb 
  D_FLAGS := $(D_FLAGS) -D DEBUG 
else
  COMP_FLAGS := $(COMP_FLAGS) -O2
  D_FLAGS := $(D_FLAGS) -D NDEBUG 
endif

# Need any additional common libraries?  -lc? -lz?
STD_LIBS := -lc

AR_FLAGS := -rcs
VPATH    := $(SRC_DIR):$(BUILD_DIR)


# ------------------------------------ #
#  (7) Pre-assemble command lines      #
# ------------------------------------ #

APP_FILES := $(APP_SOURCES) $(BUILT_OBJS)
PREP_LINE := $(D_FLAGS) -I$(INCLUDE_DIR) $(THIRD_PARTY_INCLUDES)
LINK_LINE := $(LINK_FLAGS) $(LINK_LOCAL) -L$(ARCHIVE_DIR) $(LOCAL_LIBS) \
             $(EXTRA_LINK_DIRS)   $(THIRD_PARTY_LIBS)  $(LINK_LOCAL_END) \
	     $(STD_LIBS)
CC_EXEC   := $(CC) $(COMP_FLAGS) $(DBG_FLAGS) $(C_FLAGS) 
CXX_EXEC  := $(CXX) $(COMP_FLAGS) $(DBG_FLAGS) $(CPP_FLAGS)
JXX_EXEC  := $(JXX)

ifdef REQUIRE_C
TARG_EXEC := $(CC_EXEC)
else
TARG_EXEC := $(CXX_EXEC)
endif

# ------------------------------------ #
#  (8) Targets                         #
# ------------------------------------ #

.PHONY:   all quiet env libs apps scripts export unittest install clean br

all:      env export libs apps scripts unittest post

quiet:    env export libs apps scripts post

apps:     $(OBJECTS) $(JCLASSES) $(APPS)

libs:     $(OBJECT_ARCHIVE) $(SHARED_LIBRARY) $(JARCHIVE)

unittest: $(OBJECTS) $(JCLASSES) $(OBJECT_ARCHIVE) $(SHARED_LIBRARY) $(JARCHIVE) $(TEST_APPS) $(JTEST_APPS)

post:
	@echo


# ------------------------------------ #
#  (9) Set Up Environment              #
# ------------------------------------ #

env:
	@if [ ! -d $(INCLUDE_DIR) ]; then mkdir -p $(INCLUDE_DIR); fi;
	@if [ ! -d $(BIN_DIR)     ]; then mkdir -p $(BIN_DIR);     fi;
	@if [ ! -d $(BUILD_ROOT)  ]; then mkdir -p $(BUILD_ROOT);  fi;
	@if [ ! -d $(ARCHIVE_DIR) ]; then mkdir -p $(ARCHIVE_DIR); fi;
	@if [ ! -d $(BUILD_DIR)   ]; then mkdir -p $(BUILD_DIR);   fi;
ifdef PUBLIC_HEADERS
	@if [ ! -d $(EXPORT_DIR)  ]; then mkdir -p $(EXPORT_DIR);  fi;
endif
ifdef APP_DIR
	@if [ ! -d $(APP_DIR)     ]; then mkdir -p $(APP_DIR);     fi;
endif
ifdef JAVA_PATH	
	@if [ ! -d $(BUILD_ROOT)/$(JAVA_PATH) ]; \
		then mkdir -p $(BUILD_ROOT)/$(JAVA_PATH); fi;
endif


# ------------------------------------ #
#  (10) Export Headers                 #
# ------------------------------------ #

export: env $(PUBLIC_HEADERS)
ifdef PUBLIC_HEADERS
	@echo
	@echo ... EXPORTING: Public Header Files to $(EXPORT_DIR) : $(NAMESPACE) :
	cp $(PUBLIC_HEADERS) $(EXPORT_DIR)
endif


# ------------------------------------ #
#  (11) Installation                   #
# ------------------------------------ #

install:
	@echo
ifdef APP_DIR
	@echo ... INSTALLING APPLICATIONS AND SCRIPTS
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi;
	cp $(APP_DIR)/* $(INSTALL_DIR)
else
	@echo Nothing to Install!
endif


# ------------------------------------ #
#  (12) Scripts                        #
# ------------------------------------ #

scripts:
ifdef SCRIPTS
	@echo
	@echo ... PUBLISHING SCRIPTS 
	cp $(SCRIPTS) $(APP_DIR) 
endif



# ------------------------------------ #
#  (13) Clean                          #
# ------------------------------------ #

clean:
	@rm -f *.tab.c;
	@rm -f *.tab.h;
	@rm -f *.lex.c;
	@rm -f *~;
	@rm -f \#*;
	@if [ -d $(BUILD_DIR) ]; then rm -r $(BUILD_DIR); fi;
ifdef APPS
	@for h in $(APPS); do if [ -f $(APP_DIR)/$$h ]; then rm $$h; fi; done
	
endif
ifdef AR_TARG
	@if [ -f $(ARCHIVE_DIR)/$(AR_TARG) ]; \
		then rm $(ARCHIVE_DIR)/$(AR_TARG); fi;
endif
ifdef SO_TARG
	@if [ -f $(ARCHIVE_DIR)/$(SO_TARG) ]; \
		then rm $(ARCHIVE_DIR)/$(SO_TARG); fi;
endif
ifdef JAR_TARG
	@if [ -f $(ARCHIVE_DIR)/$(JAR_TARG) ]; \
		then rm $(ARCHIVE_DIR)/$(JAR_TARG); fi;
endif
ifdef JAVA_PATH
	@if [ -d $(BUILD_ROOT)/$(JAVA_PATH) ]; \
		then rm -r $(BUILD_ROOT)/$(JAVA_PATH); fi;
endif
ifdef HEADER_TARG
	@for h in $(HEADER_TARG) ; do if [ -f $$h ]; then rm $$h; fi; done;
endif
	@printf "       :   cleaned: %-25s :\n" $(MODULE)


# ------------------------------------ #
#  (14) File Rules                     #
# ------------------------------------ #

## Pretty print for bash:
## @printf "\033[40m\033[1;37m%-4s\033[1;32m%-76s\033[0m\n" '...' '$@'

%.o : %.cpp	%.h
	@echo
	@printf "%-4s  %-76s" '...' '$@'
	$(strip $(CXX_EXEC) -c $(PREP_LINE) $< -o $(BUILD_DIR)/$@ )

%.o : %.cpp
	@echo
	@printf "%-4s  %-76s" '...' '$@'c
	$(strip $(CXX_EXEC) -c $(PREP_LINE) $< -o $(BUILD_DIR)/$@ )

%.o : %.c %.h
	@echo
	@printf "%-4s  %-76s" '...' '$@'
	$(strip $(CC_EXEC) -c $(PREP_LINE)  $< -o $(BUILD_DIR)/$@ )

%.o : %.c
	@echo
	@printf "%-4s  %-76s" '...' '$@'
	$(strip $(CC_EXEC) -c $(PREP_LINE)  $< -o $(BUILD_DIR)/$@ )

%.class : %.java
	@echo
	@printf "%-4s  %-76s" '...' '$@'
	$(strip $(JXX_EXEC) $(patsubst %.class,%.java,$@) -d $(BUILD_ROOT) )


%.lex.o : %.l
	@echo
	@printf "%-4s  %-76s" '...' '$@'
	$(LEX) -o$(LYOBJ).lex.c $(LYOBJ).l
	$(strip $(CC_EXEC) -c $(PREP_LINE)  $(LYOBJ).lex.c -o $(BUILD_DIR)/$@ )
	

%.tab.o : %.y
	@echo
	@printf "%-4s  %-76s" '...' '$@'
	$(YACC) -d $(LYOBJ).y
	$(strip $(CC_EXEC) -c $(PREP_LINE)  $(LYOBJ).tab.c -o $(BUILD_DIR)/$@ )


# ------------------------------------ #
#  (15) Application & Test Rules       #
# ------------------------------------ #

%.capp : %.c
	@echo
	@printf "%-4s%-22s%-51s\n" '...' 'BUILDING C Application' '< $(CAPP) >'
	$(strip $(CC_EXEC) $(patsubst %.capp,%.c,$@) $(BUILT_OBJS)        \
	        $(PREP_LINE) $(LINK_LINE) -o $(BUILD_DIR)/$(CAPP) )
	cp $(BUILD_DIR)/$(CAPP) $(APP_DIR)/$(CAPP)

%.jbin : %.java
	@echo
	@printf "\033[40m\033[1;37m%-4s\033[0;35m\033[40m%-22s \033[40;37m %-51s \033[0m\n" '...' 'BUILDING Java Application' '< $(JAVAPP) >'
	$(strip $(JXX_EXEC) $(patsubst %.jbin,%.java,$@) -d $(BUILD_ROOT) )	
	$(JAR) cf $(APP_DIR)/$(JAVAPP).jar -C $(BUILD_ROOT) $(JAVA_PATH)/$(JAVAPP).class
#	touch $@

%.cppapp : %.cpp
	@echo
	@printf "\033[40m\033[1;37m%-4s\033[0;35m\033[40m%-22s \033[40;37m %-51s \033[0m\n" '...' 'BUILDING C++ Application' '< $(CPPAPP) >'
	$(strip $(CXX_EXEC) $(patsubst %.cppapp,%.cpp,$@) $(BUILT_OBJS)   \
            $(PREP_LINE) $(LINK_LINE) -o                              \
            $(BUILD_DIR)/$(patsubst %.cppapp,%,$@) )
	cp $(BUILD_DIR)/$(CPPAPP) $(APP_DIR)/$(CPPAPP)

%.ctapp : %.c
	@echo
	@printf "\033[40m\033[1;37m%-4s\033[0;35m\033[40m%-19s \033[40;37m %-54s \033[0m\n" '...' 'BUILDING: Unit Test' '< $(CTAPP) >'
	$(strip $(CC_EXEC) $(patsubst %.ctapp,%.c,$@) $(BUILT_OBJS)       \
            $(PREP_LINE) $(LINK_LINE) -o $(BUILD_DIR)/$(CTAPP) )
	@echo
	@printf "\033[40m\033[1;37m... \033[1;34m\033[40m%-76s\033[0m\n" 'EXECUTING TEST'
	@$(BUILD_DIR)/$(CTAPP)
	@echo

%.cpptapp : %.cpp
	@echo
	@printf "\033[40m\033[1;37m%-4s\033[0;35m\033[40m%-19s \033[40;37m %-54s \033[0m\n" '...' 'BUILDING: Unit Test' '< $(CPPTAPP) >'
	$(strip $(CXX_EXEC) $(patsubst %.cpptapp,%.cpp,$@) $(BUILT_OBJS)  \
            $(PREP_LINE) $(LINK_LINE) -o $(BUILD_DIR)/$(CPPTAPP) )
	@echo
	@echo "... \033[1;37mEXECUTING TEST\033[0m ..."
	$(BUILD_DIR)/$(CPPTAPP)

%.jtbin : %.java
	@echo
	@printf "\033[40m\033[1;37m%-4s\033[0;35m\033[40m%-22s \033[40;37m %-51s \033[0m\n" '...' 'BUILDING Unit Test' '< $(JTAPP) >'
	$(strip $(JXX_EXEC) $(patsubst %.jtbin,%.java,$@) -d $(BUILD_ROOT) )
	@echo
	@echo "... \033[1;37mEXECUTING TEST\033[0m ..."
	java $(BUILD_DIR)/$(JAVA_PKG)/$(JTAPP)
	touch $@



# ------------------------------------ #
#  (16) Library Rules                  #
# ------------------------------------ #

$(SHARED_LIBRARY): $(OBJECTS)
ifdef SO_TARG
	@echo
	@printf "%s%-66s\n" 'BUILDING:' '$(SO_TARG)'
	$(strip $(TARG_EXEC) $(LIB_FLAGS) $(BUILT_OBJS) $(PREP_LINE) -o $@ ) 
	@if [ ! -d $(ARCHIVE_DIR) ]; then mkdir -p $(ARCHIVE_DIR); fi;
	@cp $(SHARED_LIBRARY) $(ARCHIVE_DIR)/$(SO_TARG)
endif

$(OBJECT_ARCHIVE): $(OBJECTS)
ifdef AR_TARG
	@echo
	@printf "%s%-66s\n" 'BUILDING:' '$(AR_TARG)'
	@if [ ! -d $(ARCHIVE_DIR) ]; then mkdir -p $(ARCHIVE_DIR); fi;
	ar $(AR_FLAGS) $(ARCHIVE_DIR)/$(AR_TARG) $(filter %.o,$(BUILT_OBJS))
endif

$(JARCHIVE): $(JCLASSES)
ifdef JAR_TARG
	@echo
	@printf "\033[40;1;37m... \033[0;36m\033[40m%s \033[1;36m%-66s\033[0m\n" 'BUILDING:' '$(JAR_TARG)'
	@if [ ! -d $(ARCHIVE_DIR) ]; then mkdir -p $(ARCHIVE_DIR); fi;
	$(JAR) cvf $(ARCHIVE_DIR)/$(JAR_TARG) $(addprefix -C $(BUILD_ROOT) $(JAVA_PATH)/,$(filter %.class,$(JCLASSES)))
endif	

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                                                                              #
#  Honestly, this thing is a dinosaur--and this is only the beginning! Future  #
#  functionality such as dependency tracking and a better install system will  #
#  make it even more insane!                                                   #
#                                                                              #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

#                                      .                                       #



#  ooh and wouldn't it be cool if it could identify external dependencies and  #
#  install them locally like maven does? that way people wouldn't have to put  #
#  all kinds of crazy crap on their machines just to build. i like it...       #

