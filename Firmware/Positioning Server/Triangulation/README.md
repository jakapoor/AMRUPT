We plan to connect Raspberry Pi 3 base stations wirelessly at a frequency outside the bandwidth of the rest of our system to 
prevent radio frequency interference. The Raspberry Pi 3 will send timestamped and identified angle of arrival measurements to a 
central hub/laptop. The central hub will then triangulate the position of a radio tag (based on the AOA identifications) in a two 
dimensional space. 

The code that we will use to triangulate radio tag positions will be heavily based on of the python psuedocode 
ThreeReceiverTriangulate.py. Class coordinate has four methods _init_, intersect, centroid, and triangulated area. All the 
Raspberry Pi would need to do (in a seperate script) with this file is create a class Coordinate object and call function 
triangulated_area() using six parameters. Three of these parameters consist of three angle  of arrivals of type float at three 
receiver units (e.g. RTL-SDRS). The other three parameters correspond to the positions of recievers (of type coordinate). 
Positions of receivers are instantiated using function _init_, which takes two float values and assigns them to x and y values 
respectively. 

Function intersect uses a basic intersection formula of two parameterized lines (y = mx + b) two compute the intersections of 
lines across the two dimensional space. For example, if an angle of arrival is 30 degrees, the parameterized line would be a line 
30 degrees below the x axis in the third quadrant, and continued 30 degrees above the x axis in the first quadrant, treating the 
receiver position as the origin.

Function centroid computes the centroid of the triangulated area which would be the expected value of transmitter position in an 
area (determined by angle of arrival intersections) uniformly distributed (the transmitter is equally likely to be anywhere in the 
triangulated area).

Function triangulated area computes the parameterized lines based off of angle of arrival measurements, computes the intersection 
of these lines (by calling intersect()), and calculates a position by taking the centroid of the triangulated area with 
intersections as vertices (by calling centroid()).
