# Genetic-neural-network

![Imgur](https://imgur.com/0sF5JTG.png)

## Explanation

The program displays 48 boxes. In each one of them is a blue and a green circle. The green circle is controlled by a neural network (separate for each one) and the blue one is static “food”. Every time a green circle touches a blue circle, the network responsible is rewarded with a point, and the blue circle is relocated. After a chosen time period 10 networks with the highest number of points are chosen, and copied with small mutations. Thus, a new generation of 48 networks is created. The process repeats. 

After each generation, the terminal outputs the score achieved by the best network within the generation. Example:
```
Generation number: 112 best score: 7
Generation number: 113 best score: 10
Generation number: 114 best score: 6
Generation number: 115 best score: 9
Generation number: 116 best score: 15
Generation number: 117 best score: 18
Generation number: 118 best score: 20
Generation number: 119 best score: 17
Generation number: 120 best score: 21
```

## Compilation
In order to perform compilation, a sfml library is needed, which can be downloaded from this site: 

https://www.sfml-dev.org/download/sfml/2.5.1/

On linux the command for installation is `sudo apt-get install libsfml-dev`.

For compilation on linux the script compile.sh can be used `./compile.sh`

## Control

#### n - switch display mode
 
By clicking n you can cycle between 60fps, 120fps, unlimited fps and no display. Turning off the display increases simulation speed dramatically.

#### p - pause

Pauses the simulation, while pausing the simulation, networks can also be saved/loaded to/from the file.

#### s - saving network to the file

Saves network marked with cursor to a file. The name of the file to which the network 
is saved needs to be written in the terminal.

#### l - load network from the file

Loads network marked with cursor to a file. The name of the file from which the network is 
loaded needs to be written in the terminal.

## Implementation
 
The algorithm is very time and memory efficient. It doesn't use any allocation on the heap as all data structures needed in order to build a population of neural networks are written using templates and their size is known during compilation.
