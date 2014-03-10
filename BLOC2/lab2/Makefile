all:exercicis entrega

exercicis:tetera.cc teteraDoble.cc ninotNeu.cc ninotpers.cc entrega.cc
	g++ tetera.cc -o tetera.x -lGL -lglut
	g++ teteraDoble.cc -o teteraDoble.x -lGL -lglut
	g++ ninotNeu.cc -o ninotNeu.x -lGL -lglut
	g++ -o ninotpers ninotpers.cc model.o -lGLU -lGL -lglut
	g++ -o entrega entrega.cc model.o -lGLU -lGL -lglut
entrega: entrega.cc
	g++ -o entrega entrega.cc model.o -lGLU -lGL -lglut

clean:
	rm -f exercicis
