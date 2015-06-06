all: 
	g++ ./src/app.cpp -L ./include -lredispp -lcurl -o app

clean:
	rm -f app
