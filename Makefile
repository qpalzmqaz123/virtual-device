all: a.o
	gcc a.o vdev_lcd.o `sdl2-config --cflags` `sdl2-config --libs` -lpthread

a.o: a.c vdev_lcd.c vdev_lcd.h vdev_types.h vdev.h
	gcc vdev_lcd.c `sdl2-config --cflags` `sdl2-config --libs` -o vdev_lcd.o -c
	gcc a.c `sdl2-config --cflags` `sdl2-config --libs` -o a.o -c


clean:
	rm -rvf *.o a.out
