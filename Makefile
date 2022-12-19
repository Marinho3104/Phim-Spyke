
compiler_path := ./compiler/
parser_path := ./parser/
utils_path := ./utils/
entry_point := ./test/

output_name := ./output

all:

	$(MAKE) -C $(compiler_path)
	$(MAKE) -C $(parser_path)
	$(MAKE) -C $(entry_point)
	$(MAKE) -C $(utils_path)

	$(MAKE) compile
	$(MAKE) clean

compile:

	g++ -o $(output_name) ./*.cpp -g

clean:

	rm -f ./*.cpp
	rm -f ./*.h
	rm -f ./*.o