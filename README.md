***************************************************************
**
**	GMU - 
**	Granular Synthesis Advanced Object Set for Max/MSP
**
**
**  Copyright 2004-2022 GMEM - Marseille
**			Centre National de Création Musicale 
**
**			

GMU is a flexible granular synthesis environnement for Max/MSP. It includes a set of synthesis and control DSP objects, advanced control subpatches and UI interfaces patches examples. 

THIS SOFTWARE IS NO MORE MAINTAINED AND NO SUPPORT WILL BE PROVIDED.

This software is released under special license terms.
Please refer to the License file "LICENSE.md" for further details.


______________________
  DESCRIPTION

- externals 			
		- bufGranul~ 		: multi buffer granulation object
		- synGranul~ 		: sinusoidal waveform granulation
		- liveGranul~		: live sound source granulation object
		- rand_dist_list~ 	: random generator

- abstraction 				
		- tranche_ctrl5 		: random signal generator based on probability curves
		- trig_ctrl5			: random pulse generator based on probability curves

- editors					
		- Envelope_Editor	: a block chain based envelope editor ( for grain envelope or ... )
			

_______________________
  INSTALLATION  


This release is for Max up to 8.0.2 on was tested on MacOS up to 10.13 & Windows 10.

Just put the whole package in a max search path.

you can start with the help files in help folders.

Examples folder contains more advanced example patch (see tranche_ctrl5.maxhelp and trig_ctrl5.maxhelp for help on random generators used in the patches). 

Some patches needs the [iana~] external (for sinusoidal decomposition) by Todor Todoroff.

________________________
 CONTRIBUTIONS


GMU was developped at GMEM - Marseille - Centre National de Création Musicale by :
	Charles Bascou		
	Leopold Frey			
	Laurent Pottier
and also contributions from :
	Jean-François Oliver
	Loïc Kessous