all: julia

# Make will also look in these dirs for dependencies (and targets!).
# Names found in these dirs get translated, so the 'julia'
# dependencies below get 'src/' prepended to their names, which makes
# the auto var '$<' do the right thing.
VPATH = src

julia: julia.cpp bitmapwriter.h functions.h tmp
	g++ -Wall -o $@ $<

tmp:
	mkdir -p tmp

clean:
	-rm -rf tmp
