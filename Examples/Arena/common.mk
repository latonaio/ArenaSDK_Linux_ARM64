ARCH_TYPE = $(shell dpkg --print-architecture)

ifeq ($(ARCH_TYPE), amd64)

LDFLAGS = -L../../../lib64 \
          -L../../../GenICam/library/lib/Linux64_x64 \
          -L../../../ffmpeg
GENICAMLIBS = -lGCBase_gcc421_v3_0 \
              -lGenApi_gcc421_v3_0 \
              -lLog_gcc421_v3_0 \
              -llog4cpp_gcc421_v3_0 \
              -lMathParser_gcc421_v3_0 \
              -lNodeMapData_gcc421_v3_0 \
              -lXmlParser_gcc421_v3_0

OUTDIR = ../../../OutputDirectory/Linux/x64Release/

else ifeq ($(ARCH_TYPE), armhf)

LDFLAGS = -L../../../lib \
          -L../../../GenICam/library/lib/Linux32_ARMhf \
          -L../../../ffmpeg

GENICAMLIBS = -lGCBase_gcc46_v3_0 \
              -lGenApi_gcc46_v3_0 \
              -lLog_gcc46_v3_0 \
              -llog4cpp_gcc46_v3_0 \
              -lMathParser_gcc46_v3_0 \
              -lNodeMapData_gcc46_v3_0 \
              -lXmlParser_gcc46_v3_0


OUTDIR = ../../../OutputDirectory/armhf/x32Release/

else ifeq ($(ARCH_TYPE), arm64)

LDFLAGS = -L../../../lib \
          -L../../../GenICam/library/lib/Linux64_ARM \
          -L../../../ffmpeg

GENICAMLIBS = -lGCBase_gcc48_v3_1 \
              -lGenApi_gcc48_v3_1 \
              -lLog_gcc48_v3_1 \
              -llog4cpp_gcc48_v3_1 \
              -lMathParser_gcc48_v3_1 \
              -lNodeMapData_gcc48_v3_1 \
              -lXmlParser_gcc48_v3_1


OUTDIR = ../../../OutputDirectory/arm64/x64Release/
endif

CC=g++

INCLUDE= -I../../../include/Arena \
         -I../../../include/Save \
         -I../../../include/GenTL \
         -I../../../GenICam/library/CPP/include

CFLAGS=-Wall -g -O2 -std=c++11 -Wno-unknown-pragmas


FFMPEGLIBS = -lavcodec \
             -lavformat \
             -lavutil \
             -lswresample

LIBS= -larena -lsave -lgentl $(GENICAMLIBS) $(FFMPEGLIBS) -lpthread
RM = rm -f

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(OBJS:%.o=%.d)

.PHONY: all
all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} ${LDFLAGS} $^ -o $@ $(LIBS)
	-mkdir -p $(OUTDIR)
	-cp $(TARGET) $(OUTDIR)

%.o: %.cpp ${SRCS}
	${CC} ${INCLUDE}  ${LDFLAGS} -o $@ $< -c ${CFLAGS}

${DEPS}: %.cpp
	${CC} ${CLAGS} ${INCLUDE} -MM $< >$@

-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET} ${OBJS} ${DEPS}
