.PHONY: all

SUBDIRS := $(wildcard day*/)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: all clean $(SUBDIRS)

clean:
	@for dir in $(SUBDIRS); do \
		echo "Cleaning in $$dir"; \
		$(MAKE) -C $$dir clean; \
	done
	@echo "Cleanup completed."

