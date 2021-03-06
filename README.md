# mppicPbeCryFoam
This is an open-source software which is called the multiphase particle in cell coupled with the population balance equation method implemented in OpenFOAM.
The software is an extension of mppicFoam, and a tutorial is antisolvent crystallization which is attached together.

# Copyright Information
Copyright (C) 2019 Shin-Hyuk Kim, Jay H. Lee and Richard D. Braatz

# License
mppicPbeCryFoam is free software.
Users can redistribute it or modify it under the terms of the GNU general public license.

# Requirements
OpenFOAM 5.x is required to compile the software.

# References
Users are referred to the below reference for details on the MP-PIC-PBE method.

S. H. Kim, R. D. Braatz, and Jay H Lee, “Multi-Phase Particle-In-Cell Coupled with Population Balance Equation (MP-PIC-PBE) Method for Multiscale Computational Fluid Dynamics Simulation,” Comput. Chem. Eng., 134, 106686, 2020.

# Notice
When working 'wmake', some errors can occur regarding link files in the lnInclude folder at intermediate folder. In this case, replace the existing link files with the link files in openfoam5/scr/lagrangian/intermediate/lnInclude.
