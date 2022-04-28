ifndef OS # linux
EXECUTABLE_EXTENSION=
RM=rm -f $(1)
else ifeq ($(OS), Windows_NT) # windows
EXECUTABLE_EXTENSION=.exe
RM=if exist $(1) del $(1)
else
$(error os not supported)
endif

file-to-c$(EXECUTABLE_EXTENSION): main.o
	gcc -o file-to-c$(EXECUTABLE_EXTENSION) main.o

ifndef OS # linux
file-to-c.AppImage: file-to-c
	cp file-to-c file-to-c.AppDir/usr/bin/file-to-c
	ARCH=x86_64 ./appimagetool-x86_64.AppImage file-to-c.AppDir
	mv file-to-c-x86_64.AppImage file-to-c.AppImage
	chmod +x file-to-c.AppImage
endif

main.o: main.c
	gcc -c main.c

clean:
	$(call RM,main.o)
	$(call RM,file-to-c)
	$(call RM,file-to-c.exe)
	$(call RM,file-to-c.AppImage)