all: clean-all build clean

build: "Clipboard History.exe"

"Clipboard History.exe": main.obj linked_list.obj buffer.obj guiparams.obj gui.obj global.obj listener.obj
	cl main.obj linked_list.obj buffer.obj guiparams.obj gui.obj global.obj listener.obj /link /out:"Clipboard History.exe"

main.obj: src/main.c
	cl /c /I include "src/main.c"

linked_list.obj: src/linked_list.c
	cl /c /I include "src/linked_list.c"

buffer.obj: src/buffer.c
	cl /c /I include "src/buffer.c"

guiparams.obj: src/guiparams.c
	cl /c /I include "src/guiparams.c"

gui.obj: src/gui.c
	cl /c /I include "src/gui.c"

global.obj: src/global.c
	cl /c /I include "src/global.c"

listener.obj: src/listener.c
	cl /c /I include "src/listener.c"

clean:
	del *.obj

clean-all:
	del *.obj *.exe