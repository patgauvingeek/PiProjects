PROJECTS := libSimOn PiAlarm PiMorse

.PHONY: all $(PROJECTS)

all: $(PROJECTS)

PiMorse: libSimOn
		@echo "=== PiMorse ==="
		@$(MAKE) -C PiMorse --no-print-directory

PiAlarm: libSimOn
		@echo "=== PiAlarm ==="
		@$(MAKE) -C PiAlarm --no-print-directory

libSimOn:
		@echo "=== libSimOn ==="
		@$(MAKE) -C libSimOn --no-print-directory

clean:
		@for subdir in $(PROJECTS); do \
			$(MAKE) -C $$subdir clean || exit 1; \
		done

rebuild: clean all

install: all
		@echo "not implemented yet"

remove:
		@echo "not implemented yet"

update: clean
		@echo "Updating pi..."
		@scp -r libSimOn PiAlarm PiMorse pi@192.168.0.8:/home/pi/PiProjects
