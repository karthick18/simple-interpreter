TARGETS = evaluate

.PHONY: $(TARGETS)

all: all_objs $(TARGETS)

all_objs:
	$(foreach dir, $(ALL_DIRS), make -C $(dir); )

evaluate:$(ALL_OBJS)
	$(CC) $(DEBUG_FLAGS) $(LINKER_FLAGS) -o $@ $(ALL_OBJS)

clean:
	$(foreach dir, $(ALL_DIRS), make -C $(dir) clean;) \
	rm -f $(TARGETS)

include Rules.inc