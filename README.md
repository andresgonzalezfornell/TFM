+---------------------------------------------------------------------------------------------------+
| SAC EFFECTS PLATFORM                                                                              |
| Command line application to apply effects to a encoded file as Spatial Audio Coding (SAC) format. |
| Created by Andrés González Fornell                                                                |
| Escuela Ténica Superior de Ingenieros de Telecomunicación (Universidad Politécnica de Madrid)     |
+---------------------------------------------------------------------------------------------------+

The source code files include two applications:
	- commandline application is run through terminal
	- interface application includes an user interface

PREREQUISITES

	- Linux or Mac OS X
	- 32bits system architecture or higher
	- C++11 compiler or higher
	- Qt libraries (only for interface)

HOW TO BUILD

	- commandline
		In files folder, execute the following commands on the terminal:
			./configure
			make
		It creates a binary application in bin/commandline/saceffects that you can run.

	- interface
		You need to download Qt libraries and Qt Creator in:
			https://info.qt.io/download-qt-for-application-development
		Then, you can build the interface application through Qt Creator.

HOW TO ADD NEW EFFECTS

	- commandline
		1. Copy the new effect source file (.cpp) and header file (.h) to src/effects.
		2. Add the following line on the top of file src/effects/Effect.h:
			#include “HeaderFile.h”
		3. Add the following line in file src/effects/Effect.h according to description 2. LIST:
			EFFECT(EffectClass, “EffectClass”)
		4. Add the following line in file src/effects/Effect.h according to description 3. INHERITANCE OF CLASSES:
			public EffectClass
		5. Add the following lines to the switch-case statement in funtion Effect::apply in file src/effects/Effect.cpp:
			case effectID::EffectClass:
				ClaseEfecto::apply(input, output, samples, channels);
			break;
		6. Add the following lines to the switch-case statement in funtion Effect::plot in file src/effects/Effect.cpp:
			case effectID::EffectClass:
				return ClaseEfecto::plot(chart);
		7. Build the application with the following commands on terminal:
			./configure
			make

	- interface
		1. Copy the new effect source file (.cpp) and header file (.h) to src/effects.		2. Copy the user interface control description file (.xml) to src/interface.
		3. Open Qt project (interface.pro) with Qt Creator and configure project if it is required.
		4. On Qt Creator, add the three copied files to the project by right-clicking on the project folder and clicking on Add new files.		5. On Qt Creator, add the user interface control description file to media.qrc by right-clicking on media.qrc file and clicking on Open with editor. Then select prefix folder /imports and click on Add and Add files.		6. Add the following line on the top of file src/effects/Effect.h:
			#include “HeaderFile.h”
		7. Add the following line in file src/effects/Effect.h according to description 2. LIST:
			EFFECT(EffectClass, “EffectClass”)
		8. Add the following line in file src/effects/Effect.h according to description 3. INHERITANCE OF CLASSES:
			public EffectClass
		9. Add the following lines to the switch-case statement in funtion Effect::apply in file src/effects/Effect.cpp:
			case effectID::EffectClass:
				ClaseEfecto::apply(input, output, samples, channels);
			break;
		10. Add the following lines to the switch-case statement in funtion Effect::plot in file src/effects/Effect.cpp:
			case effectID::EffectClass:
				return ClaseEfecto::plot(chart);
		11. Build the application with Qt Creator
        
        