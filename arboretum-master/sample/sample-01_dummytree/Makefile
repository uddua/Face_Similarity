# Makefile for GCC
#
# Author: Marcos Rodrigues Vieira
# Author: Fabio Jun Takada Chino
#
INCLUDEPATH=../../arboretum
LIBPATH=-L../..
INCLUDE=-I$(INCLUDEPATH)
LIBS=-lstdc++ -lm -larboretum
SRC= app.cpp Cities.cpp city.cpp
OBJS=$(subst .cpp,.o,$(SRC))

# Implicit Rules
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

Cities: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o Cities $(INCLUDE) $(LIBPATH) $(LIBS)

clean:
	-rm Cities *.o
