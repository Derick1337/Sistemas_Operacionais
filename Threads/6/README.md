Para compilar o exercício 6, primeiro baixe o opencv, usando: 
sudo apt update
sudo apt install libopencv-dev

Depois compile com:
g++ -o 6 6.cpp `pkg-config --cflags --libs opencv4` -pthread
