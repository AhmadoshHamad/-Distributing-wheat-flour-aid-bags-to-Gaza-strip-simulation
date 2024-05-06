plane idf test container main dist split collect: opengl.cpp plane.cpp idf.cpp test.cpp container.cpp main.cpp distribute.cpp split.cpp collect.cpp
	g++ plane.cpp -o plane
	g++ idf.cpp -o idf
	g++ test.cpp -o test
	g++ container.cpp -o container
	g++ main.cpp -o main
	g++ distribute.cpp -o dist
	g++ split.cpp -o split
	g++ collect.cpp -o collect
	g++ -o opengl opengl.cpp -lglut -lGL -lGLU