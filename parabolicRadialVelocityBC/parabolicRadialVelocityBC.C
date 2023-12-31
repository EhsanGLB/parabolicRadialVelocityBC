/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.1
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "parabolicRadialVelocityBC.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

parabolicRadialVelocityBC::parabolicRadialVelocityBC
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(p, iF),
    meanValue_(0),
    lenDir_(1, 0, 0),
    radius_(0.0)
{}


parabolicRadialVelocityBC::parabolicRadialVelocityBC
(
    const parabolicRadialVelocityBC& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchVectorField(ptf, p, iF, mapper),
    meanValue_(ptf.meanValue_),
    lenDir_(ptf.lenDir_),
    radius_(ptf.radius_)
{}


parabolicRadialVelocityBC::parabolicRadialVelocityBC
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchVectorField(p, iF),
    meanValue_(readScalar(dict.lookup("meanValue"))),
    lenDir_(dict.lookup("lenDir")),
    radius_(readScalar(dict.lookup("radius")))
{
    lenDir_ /= mag(lenDir_);

    evaluate();
}


parabolicRadialVelocityBC::parabolicRadialVelocityBC
(
    const parabolicRadialVelocityBC& fcvpvf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(fcvpvf, iF),
    meanValue_(fcvpvf.meanValue_),
    lenDir_(fcvpvf.lenDir_),
    radius_(fcvpvf.radius_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void parabolicRadialVelocityBC::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    boundBox bb_(patch().patch().localPoints(), true);
    vector ctr_ = 0.5*(bb_.max() + bb_.min());
    const vectorField& c_ = patch().Cf();
    scalarField rp_ = mag(c_ - ctr_);

    vectorField::operator=(lenDir_*2.0*meanValue_*(1.0 - pow(rp_/radius_, 2.0)));
    fixedValueFvPatchVectorField::updateCoeffs();
}


// Write
void parabolicRadialVelocityBC::write(Ostream& os) const
{
    fvPatchVectorField::write(os);
    os.writeKeyword("meanValue") << meanValue_ << token::END_STATEMENT << nl;
    os.writeKeyword("lenDir") << lenDir_ << token::END_STATEMENT << nl;
    os.writeKeyword("radius") << radius_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField(fvPatchVectorField, parabolicRadialVelocityBC);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
