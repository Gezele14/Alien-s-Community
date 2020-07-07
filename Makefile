all: code

libs:
		(cd lib && make)

code:
		(cd src && make)