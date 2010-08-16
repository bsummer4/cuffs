clean:V:
	cd sdl; mk clean; cd ..

test:V:
	cd sdl; mk test; cd ..
	cd game; mk test; cd ..
