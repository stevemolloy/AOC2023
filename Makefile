.PHONY: all

SUBDIRS := $(wildcard day*/)

all: $(SUBDIRS)

$(SUBDIRS):
	@echo "Running make in $@"
	@$(MAKE) -C $@
	@echo "Make in $@ completed."

.PHONY: all clean $(SUBDIRS)

clean:
	@for dir in $(SUBDIRS); do \
		echo "Cleaning in $$dir"; \
		$(MAKE) -C $$dir clean; \
	done
	@echo "Cleanup completed."

