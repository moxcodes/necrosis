necrosis_base: necrosis_base.cpp
	g++ -std=c++1y -g -o ./necrosis ./necrosis_base.cpp -lSDL2 -lSDL2_image -lSDL2_ttf 

necrosis_level_editor: necrosis_editor.cpp
	g++ -std=c++1y -g -o ./necrosis-edit ./necrosis_editor.cpp -lSDL2 -lSDL2_image -lSDL2_ttf
