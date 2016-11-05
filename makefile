program := achilles
cli := cli

.PHONY: all $(program) ${cli}

all: $(program) $(cli) 

$(program):
	$(MAKE) --directory=$@ $(TARGET)

$(cli):
	$(MAKE) --directory=$@ $(TARGET)

clean:
#for d in $(program) $(utest);
	for d in $(program);                    \
	do                                      \
		$(MAKE) --directory=$$d clean;  \
	done
