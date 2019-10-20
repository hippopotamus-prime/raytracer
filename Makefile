CC      = g++
CCFLAGS = -O3 -Wall

# reminder because you always forget
# ----------------------------------
# $@ refers to the file name of the target, e.g. "multiply"
# $< refers to the name of the first dependency, e.g. "multiply.c"
# $^ refers to a list of all dependencies that have appeared, e.g. "multiply.c multiply_spu.o common.h"
# $+ is the same as the above, but does not check for repeated items
# $% is the name of the archive member, when the target is an archive member and not a file name
#    (as for what that means, I have no idea)

# files and intermediate files we create
OBJS  = main.o Scene.o VectorMath.o Triangle.o Primitive.o Sphere.o Cone.o \
        Disk.o Polygon.o Surface.o Phong.o Minnaert.o BlinnPhong.o \
        BoundingBox.o SpacePartition.o
PROG  = raytracer

# rules for building -- ordered from final output to original .c for no
# particular reason other than that the first rule is the default

all: $(PROG)

debug: CCFLAGS = -g -Wall
debug: $(PROG)

# program from .o files
$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS) $(LDLIBS)

main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp

Scene.o: Scene.cpp Scene.h
	$(CC) $(CCFLAGS) -c Scene.cpp

VectorMath.o: VectorMath.cpp VectorMath.h
	$(CC) $(CCFLAGS) -c VectorMath.cpp

Triangle.o: Triangle.cpp Triangle.h Primitive.h
	$(CC) $(CCFLAGS) -c Triangle.cpp

Primitive.o: Primitive.cpp Primitive.h
	$(CC) $(CCFLAGS) -c Primitive.cpp

Sphere.o: Sphere.cpp Sphere.h Primitive.h
	$(CC) $(CCFLAGS) -c Sphere.cpp

Cone.o: Cone.cpp Cone.h Primitive.h
	$(CC) $(CCFLAGS) -c Cone.cpp

Disk.o: Disk.cpp Disk.h Primitive.h
	$(CC) $(CCFLAGS) -c Disk.cpp

Polygon.o: Polygon.cpp Polygon.h Primitive.h
	$(CC) $(CCFLAGS) -c Polygon.cpp

Surface.o: Surface.cpp Surface.h
	$(CC) $(CCFLAGS) -c Surface.cpp

Phong.o: Phong.cpp Phong.h Surface.h
	$(CC) $(CCFLAGS) -c Phong.cpp

Minnaert.o: Minnaert.cpp Minnaert.h Surface.h
	$(CC) $(CCFLAGS) -c Minnaert.cpp

BlinnPhong.o: BlinnPhong.cpp BlinnPhong.h Surface.h
	$(CC) $(CCFLAGS) -c BlinnPhong.cpp

# OBJLoader.o: OBJLoader.cpp OBJLoader.h
# 	$(CC) $(CCFLAGS) -c OBJLoader.cpp

BoundingBox.o: BoundingBox.cpp BoundingBox.h
	$(CC) $(CCFLAGS) -c $<

SpacePartition.o: SpacePartition.cpp SpacePartition.h BoundingBox.h
	$(CC) $(CCFLAGS) -c $<

# remove everything but the program
clean:
	rm -f *~ *.o core*

# remove everything including program
clobber: clean
	rm -f $(PROG)
