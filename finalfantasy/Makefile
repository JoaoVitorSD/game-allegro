ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: finalfantasy.exe 


finalfantasy.exe: finalfantasy.o
	gcc -o finalfantasy.exe finalfantasy.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

finalfantasy.o: finalfantasy.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c finalfantasy.c

	
clean:
	del finalfantasy.o 
	del finalfantasy.exe
	

