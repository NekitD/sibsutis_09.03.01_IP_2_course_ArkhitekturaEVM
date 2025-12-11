#Файл управления

.PHONY: all run-pr01 clean run-console

all:
	$(MAKE) -C ./console all

clean:
	$(MAKE) -C ./console clean

run-pr01:
	$(MAKE) -C ./console run-pr01

run-console:
	$(MAKE) -C ./console run-console

sat:
	$(MAKE) -C ./console sat $(word 2, $(MAKECMDGOALS)) $(word 3, $(MAKECMDGOALS))

%:
	@: