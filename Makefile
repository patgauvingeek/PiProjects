PROJECTS := libSimOn PiAlarm PiMorse music

.PHONY: all $(PROJECTS)

all: $(PROJECTS)

PiMorse: libSimOn
		@echo "=== PiMorse ==="
		@$(MAKE) $(OPT) -C PiMorse --no-print-directory

PiAlarm: libSimOn
		@echo "=== PiAlarm ==="
		@$(MAKE) $(OPT) -C PiAlarm --no-print-directory

music: libSimOn
		@echo "=== music ==="
		@$(MAKE) $(OPT) -C music --no-print-directory

libSimOn:
		@echo "=== libSimOn ==="
		@$(MAKE) $(OPT) -C libSimOn --no-print-directory

clean:
		@for subdir in $(PROJECTS); do \
			$(MAKE) $(OPT) -C $$subdir clean || exit 1; \
		done

rebuild: clean all

debug: OPT = debug
debug: all

install: all
		@echo "not implemented yet"

remove:
		@echo "not implemented yet"

tests: all
		@echo "Running tests"
		@cd ./PiAlarm/tests; ./livetest.sh

logs: all
		@echo "Logs"
		@cd ./PiAlarm/tests; ./showlog.sh

update: clean
		@echo "Updating pi..."
		@rsync --progress -avzhe ssh libSimOn PiAlarm PiMorse music Makefile 192.168.0.8:~/PiProjects
