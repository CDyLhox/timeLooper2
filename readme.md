# yello, 

whats going on, here is the first version of the timelooper available as a vst in your daw
keep in mind, its buggy as hell also it requires a few things to work 

## requirements

* JUCE,
* CMAKE,
* Clang support probably also

## INSTALLATION
(terminal install, if you want a gui installer you figure it out yourself)
- first of all clone the repo
- then add juce
- then make build folder 
- enter build folder 
- build 

> git clone https://github.com/CDyLhox/timeLooper2.git
>
> cd timelooper2 
>
> git submodule add https://github.com/juce-framework/JUCE.git
>
> CMake -S . -B build
>
> cd build 
>
> make 

congratulations! you have now built the timelooper
youll find your vst in "/timelooper2/build/timelooper2/CdlStretchLooper_artefacts"

***ps, could be i accidentally update the cmakelists ***line 19** and comment it out cause i personally dont need it over there. you do though so if i comment it out you can edit CMakeLists.txtto uncomment it. itll say something like "unknown command JUCE init when building"****

## Usage
so right now the machine has no knobs so if you want to change parameters youll have to do so in my code. 
the 2 paramiters i find most interesting to change are the 
> m_maxNumZeroCrossings -- on line 35 of timestretcher.h
>
> - this is the amount of times the audio has to cross 0 for it to fill the loop and repeat, basically the ***LOOPSIZE***
>
> then another interesting parameter is the m_threshold on line 31 of timestretcher.h
>
> - this controls is how loud the input volume has to be for the timestretcher to activate. 

have fun with it for real 


automatic vs manual
random length
pitch 
interpolation 


