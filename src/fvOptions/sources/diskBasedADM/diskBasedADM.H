/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::fv::diskBasedADM

Description
    Modification of OpenFOAM's Actuation disk source to work with
    disk based velocity

    Constant values for momentum source for actuation disk
    \f[
        T = 0.5 \rho A U_{o}^2 Ct
    \f]

    where:
    \vartable
        A   | disk area
        U_o | upstream or disk velocity
    \endvartable

Usage
    Example usage:
    \verbatim
    fields          (U);        // names of fields to apply source
    diskDir         (-1 0 0);   // disk direction
    Ct              0.5;        // thrust coefficient
    diskArea        5.0;        // disk area
    upstreamPoint   (0 0 0);    // upstream point
    \endverbatim


SourceFiles
    diskBasedADM.C
    diskBasedADMTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef diskBasedADM_H
#define diskBasedADM_H

#include "cellSetOption.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                     Class diskBasedADM Declaration
\*---------------------------------------------------------------------------*/

class diskBasedADM
:
    public cellSetOption
{

protected:

    // Protected data

        //- Disk area normal
        vector diskDir_;

        //- Thrust coefficient
        scalar Ct_;

        //- Disk area
        scalar diskArea_;

        //- Upstream point sample
        point upstreamPoint_;

        //- Upstream cell ID
        label upstreamCellId_;


private:

    // Private Member Functions

        //- Check data
        void checkData() const;

        //- Add resistance to the UEqn
        template<class RhoFieldType>
        void addActuationDiskAxialInertialResistance
        (
            vectorField& Usource,
            const labelList& cells,
            const scalarField& V,
            const RhoFieldType& rho,
            const vectorField& U
        ) const;

        //- Disallow default bitwise copy construct
        diskBasedADM(const diskBasedADM&);

        //- Disallow default bitwise assignment
        void operator=(const diskBasedADM&);


public:

    //- Runtime type information
    TypeName("diskBasedADM");


    // Constructors

        //- Construct from components
        diskBasedADM
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~diskBasedADM()
    {}


    // Member Functions

        // Access

            //- Return Ct
            scalar Ct() const
            {
                return Ct_;
            }

            //- Normal disk direction
            const vector& diskDir() const
            {
                return diskDir_;
            }

            //- Disk area
            scalar diskArea() const
            {
                return diskArea_;
            }


         // Add explicit and implicit contributions

            //- Source term to momentum equation
            virtual void addSup
            (
                fvMatrix<vector>& eqn,
                const label fieldi
            );

            //- Source term to compressible momentum equation
            virtual void addSup
            (
                const volScalarField& rho,
                fvMatrix<vector>& eqn,
                const label fieldi
            );


        // IO

            //- Read dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "diskBasedADMTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
