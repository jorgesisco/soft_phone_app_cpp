CC=g++
CFLAGS=-g -Wall -I/usr/local/include/pjsua2 -Iinclude
LDFLAGS=-L/usr/local/lib -lpjsua2 -lpj
SOURCES=main.cpp src/MyCall.cpp src/MyAccount.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	find . -name "*.o" -type f -delete
	rm -f $(EXECUTABLE)


bu:
	docker-compose up --build

down:
	docker-compose down


env:
	source load_env.sh