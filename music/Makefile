CXXFLAGS = -std=c++14 -Idb -I../libSimOn/src -Wall -fexceptions -Wno-deprecated-declarations
LDFLAGS  = -L../lib/$(TARGET_ARCH) -lSimOn

TARGET_DIR=../bin/$(TARGET_ARCH)
EXECUTABLE=music
TARGET=$(TARGET_DIR)/$(EXECUTABLE)

MKDIR_P = mkdir -p

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) 
SOURCES := $(GEN) $(call rwildcard,src/,*.cpp)
OBJDIRS := $(sort $(dir $(patsubst %.cpp,obj/$(TARGET_ARCH)/%,$(SOURCES))))
OBJECTS := $(patsubst %.cpp,obj/$(TARGET_ARCH)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: directories

all: directories $(TARGET)

directories: $(OBJDIRS)

$(OBJDIRS):
		@$(MKDIR_P) $(OBJDIRS)

$(TARGET): $(OBJECTS)
		@$(MKDIR_P) $(TARGET_DIR)
		@echo "=== Linking ===" $@
		@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

obj/$(TARGET_ARCH)/%.o: %.cpp
		@echo "== Compiling ==" $<
		@$(CXX) -MD -c $< -o $@ $(CXXFLAGS)

-include $(DEPS)

clean:
		@echo "=== Cleaning ==="
		@rm -Rf db
		@rm -Rf obj

rebuild: clean all

debug: CXXFLAGS += -DDEBUG -g
debug: clean all