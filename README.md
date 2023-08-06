# GMU - GMEM Microsound Universe	

![GMU_logo](https://cdn.artishoc.coop/73fdf21c-7f78-49a8-a3b9-14025fe01361/v1/medias/eyJfcmFpbHMiOnsibWVzc2FnZSI6Ik1qTTFOalU9IiwiZXhwIjpudWxsLCJwdXIiOiJtZWRpYS9tZWRpYV9pZCJ9fQ==--945665266434b7eb57a069a86cecce719f64dbdb3f109985639e45c540057957/c3a4489ca7b2/test-interstellar3-1000x500-e1489077177439.png)

GMU is a flexible granular synthesis environnement for Max. It includes a set of synthesis and control DSP objects, advanced control subpatches and UI interfaces patches examples. 

**THIS SOFTWARE IS _NO MORE MAINTAINED_ AND _NO SUPPORT_ WILL BE PROVIDED.**

This software is released under special license terms.
Please refer to the License file [LICENSE.md](LICENSE.md) for further details.


## Description

- externals 			
	- bufGranul~ 		: multi buffer granulation object
	- synGranul~ 		: sinusoidal waveform granulation
	- liveGranul~		: live sound source granulation object
	- rand_dist_list~ 	: random generator

- abstraction 				
	- tranche_ctrl5 		: random signal generator based on probability curves
	- trig_ctrl5			: random pulse generator based on probability curves

- editors					
	- Envelope_Editor	: a block chain based envelope editor (for grain envelope or ...)
			

## Installation  

This release is for Max up to 8.0.2 and was tested on MacOS up to 10.13 & Windows 10.

Just put the whole package in a max search path.

you can start with the help files in help folders.

Examples folder contains more advanced example patch (see *tranche_ctrl5.maxhelp* and *trig_ctrl5.maxhelp* for help on random generators used in the patches). 

Some patches needs the [iana~](https://forum.ircam.fr/projects/detail/max-sound-box/) external (for sinusoidal decomposition) by Todor Todoroff.

## Build

If you want to build the externals from source, you will need to install [CMake](https://cmake.org/) and [Max SDK](https://github.com/Cycling74/max-sdk).

Go to the `build` folder and run:

``` 
cmake -DC74_SDK=</path/to/sdk/source> ..
cmake --build .
```
The externals will be built in the `externals` folder.

## Contributions

GMU was developped at [GMEM - Marseille - Centre National de Création Musicale](https://gmem.org/) by :
- Charles Bascou		
- Leopold Frey			
- Laurent Pottier

And also contributions from :
- Jean-François Oliver
- Loïc Kessous
- Arthus Touzet
- Matéo Fayet

More info : [GMU Website](https://gmem.org/recherche/gmu-granular-synthesis-environment/)
