READY_DIR = ready
DEBUG_EXE = ./bin/lab
RELEASE_EXE = ./bin/lab


build:
	@mkdir -p ./build
	@cd ./build && cmake .. && make


debug:
	@mkdir -p ./build
	@cd ./build && cmake .. && make


clean:
	@rm -rf ./build ./bin ./CMakeFiles


$(READY_DIR)/stud-unit-test-report-prev.json:
	

$(READY_DIR)/stud-unit-test-report.json: $(READY_DIR)
	cp test/stud-unit-test-report-prev.json $@

$(READY_DIR)/app-cli-debug: debug $(READY_DIR)
	mv $(DEBUG_EXE) $@

$(READY_DIR)/report.pdf: $(READY_DIR)
	cp docs/report.pdf ready/report.pdf

$(READY_DIR):
	@mkdir -p ./ready