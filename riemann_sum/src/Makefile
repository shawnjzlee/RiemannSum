CC=g++-5
CFLAGS=-c -std=c++14
LDFLAGS=-pthread -lpthread
SOURCES=main.cpp riemann.cpp rbarrier.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=riemann
BUILD_DIR=./bin

all: $(SOURCES) $(EXECUTABLE)
	
debug: CFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) copy
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/$@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm *.o
	rm -rf $(BUILD_DIR)
	
copy:
	mkdir -p $(BUILD_DIR)
	cp -f ./Makefile $(BUILD_DIR)/Makefile
	cp -f ./input.txt $(BUILD_DIR)/input.txt
	
check:
	./$(EXECUTABLE) input.txt ${ARGS} 0 1
	
check-multiplier:
	./$(EXECUTABLE) input.txt ${ARGS1} ${ARGS2} 1

check-nosharing:
	./$(EXECUTABLE) input.txt ${ARGS} 0 0

check-multiplier-nosharing:
	./$(EXECUTABLE) input.txt ${ARGS1} ${ARGS2} 0
	
check_d:
	gdb --args $(EXECUTABLE) input.txt ${ARGS} 0 1

n ?= 1
check_r:
	n=$(n); \
	while [ $${n} -le ${ARGS} ] ; do \
		echo $$n ; \
		$(EXECUTABLE) input.txt $${n} 0 1; \
		echo ; \
		n=`expr $$n + 1`; \
	done; \
	true

n ?= 1
check_r-nosharing:
	n=$(n); \
	while [ $${n} -le ${ARGS} ] ; do \
		echo $$n ; \
		$(EXECUTABLE) input.txt $${n} 0 0; \
		echo ; \
		n=`expr $$n + 1`; \
	done; \
	true
