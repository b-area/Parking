NAME = camshiftdemo
NAME2 = watershed
NAME3 = segment
CC = g++
CFLAGS = -Wall -g -O0 `pkg-config --cflags opencv`
LIBS = -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

all:
	$(CC) $(NAME).cpp $(CFLAGS) $(LIBS) -o $(NAME)
	$(CC) $(NAME2).cpp $(CFLAGS) $(LIBS) -o $(NAME2)
	$(CC) $(NAME3).cpp $(CFLAGS) $(LIBS) -o $(NAME3)