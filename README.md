# CMAC-
A C/C++ implementation of a Cerebellar Model Articulation Controller (CMAC). 

The C++ implementation contains an interface file inherited by a basic CMAC class, which 
is inherited by the rest of the CMAC implementations. The interface CMAC file ICmac includes
virtual methods: 
- Calculate : calculates the output of the CMAC
- Train : trains the CMAC weights by adjusting them using a leakage and e-modification method
- GetRmsWeights : grabs the RMS of all the weights of a CMAC

Currently, two implementations of a CMAC types are created:
- Cmac : very basic CMAC implementation
- SupervisoryCmac : inherits Cmac with supervisory bias values as an addition


