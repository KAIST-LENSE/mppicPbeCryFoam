/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "mathematicalConstants.H"

using namespace Foam::constant::mathematical;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
inline
Foam::KinematicParcel<ParcelType>::constantProperties::constantProperties()
:
    dict_(dictionary::null),
    parcelTypeId_(dict_, -1),
    rhoMin_(dict_, 0.0),
    rho0_(dict_, 0.0),
    minParcelMass_(dict_, 0.0)
{}


template<class ParcelType>
inline Foam::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
    const constantProperties& cp
)
:
    dict_(cp.dict_),
    parcelTypeId_(cp.parcelTypeId_),
    rhoMin_(cp.rhoMin_),
    rho0_(cp.rho0_),
    minParcelMass_(cp.minParcelMass_)
{}


template<class ParcelType>
inline Foam::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
    const dictionary& parentDict
)
:
    dict_(parentDict.subOrEmptyDict("constantProperties")),
    parcelTypeId_(dict_, "parcelTypeId", -1),
    rhoMin_(dict_, "rhoMin", 1e-15),
    rho0_(dict_, "rho0"),
    minParcelMass_(dict_, "minParcelMass", 1e-15)
{}


template<class ParcelType>
inline Foam::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& owner,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti
)
:
    ParcelType(owner, coordinates, celli, tetFacei, tetPti),
    active_(true),
    typeId_(-1),
    nParticle_(0),
    d_(0.0),
    dTarget_(0.0),
    U_(Zero),
    rho_(0.0),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(Zero),
    kv_(0.0),
    dr_(0.0),
    noNode_(0.0),
    minmodeTheta_(0.0), 
    totalSource_(0.0),
    rhoc_(0.0),
    Uc_(Zero),
    muc_(0.0),
    supersatc_(0.0),
    R_growthc_(0.0),
    R_nucleationc_(0.0)
{
    for(int i=0 ; i<40 ; i++)
    {
        editF(i) = 0 ;
    }
}


template<class ParcelType>
inline Foam::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& owner,
    const vector& position,
    const label celli
)
:
    ParcelType(owner, position, celli),
    active_(true),
    typeId_(-1),
    nParticle_(0),
    d_(0.0),
    dTarget_(0.0),
    U_(Zero),
    rho_(0.0),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(Zero),
    kv_(0),
    dr_(0),
    noNode_(0),
    minmodeTheta_(0),
    totalSource_(0),
    rhoc_(0.0),
    Uc_(Zero),
    muc_(0.0),
    supersatc_(0.0),
    R_growthc_(0.0),
    R_nucleationc_(0.0)
{
    for(int i=0 ; i<40 ; i++)
    {
        editF(i) = 0 ;
    }
}


template<class ParcelType>
inline Foam::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& owner,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti,
    const label typeId,
    const scalar nParticle0,
    const scalar d0,
    const scalar dTarget0,
    const vector& U0,
    const constantProperties& constProps
)
:
    ParcelType(owner, coordinates, celli, tetFacei, tetPti),
    active_(true),
    typeId_(typeId),
    nParticle_(0),
    d_(d0),
    dTarget_(dTarget0),
    U_(U0),
    rho_(constProps.rho0()),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(Zero),
    kv_(0),
    dr_(0),
    noNode_(0),
    minmodeTheta_(0),
    totalSource_(0),
    rhoc_(0.0),
    Uc_(Zero),
    muc_(0.0),
    supersatc_(0.0),
    R_growthc_(0.0),
    R_nucleationc_(0.0)
{
    for(int i=0 ; i<40 ; i++)
    {
        editF(i) = 0 ;
    }
}


// * * * * * * * * * constantProperties Member Functions * * * * * * * * * * //

template<class ParcelType>
inline const Foam::dictionary&
Foam::KinematicParcel<ParcelType>::constantProperties::dict() const
{
    return dict_;
}


template<class ParcelType>
inline Foam::label
Foam::KinematicParcel<ParcelType>::constantProperties::parcelTypeId() const
{
    return parcelTypeId_.value();
}


template<class ParcelType>
inline Foam::scalar
Foam::KinematicParcel<ParcelType>::constantProperties::rhoMin() const
{
    return rhoMin_.value();
}


template<class ParcelType>
inline Foam::scalar
Foam::KinematicParcel<ParcelType>::constantProperties::rho0() const
{
    return rho0_.value();
}


template<class ParcelType>
inline Foam::scalar
Foam::KinematicParcel<ParcelType>::constantProperties::minParcelMass() const
{
    return minParcelMass_.value();
}



// * * * * * * * KinematicParcel Member Functions  * * * * * * * //

template<class ParcelType>
inline bool Foam::KinematicParcel<ParcelType>::active() const
{
    return active_;
}


template<class ParcelType>
inline Foam::label Foam::KinematicParcel<ParcelType>::typeId() const
{
    return typeId_;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::nParticle() const
{
    return nParticle_;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::d() const
{
    return d_;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::dTarget() const
{
    return dTarget_;
}


template<class ParcelType>
inline const Foam::vector& Foam::KinematicParcel<ParcelType>::U() const
{
    return U_;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::rho() const
{
    return rho_;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::age() const
{
    return age_;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::tTurb() const
{
    return tTurb_;
}


template<class ParcelType>
inline const Foam::vector& Foam::KinematicParcel<ParcelType>::UTurb() const
{
    return UTurb_;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::rhoc() const
{
    return rhoc_;
}


template<class ParcelType>
inline const Foam::vector& Foam::KinematicParcel<ParcelType>::Uc() const
{
    return Uc_;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::muc() const
{
    return muc_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::supersatc() const
{
    return supersatc_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::R_growthc() const
{
    return R_growthc_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::R_nucleationc() const
{
    return R_nucleationc_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::F(int i) const
{
    return F_[i];
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::kv() const
{
    return kv_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::dr() const
{
    return dr_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::noNode() const
{
    return noNode_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::minmodeTheta() const
{
    return minmodeTheta_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::totalSource() const
{
    return totalSource_;
}

template<class ParcelType>
inline bool& Foam::KinematicParcel<ParcelType>::active()
{
    return active_;
}


template<class ParcelType>
inline Foam::label& Foam::KinematicParcel<ParcelType>::typeId()
{
    return typeId_;
}


template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::nParticle()
{
    return nParticle_;
}


template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::d()
{
    return d_;
}


template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::dTarget()
{
    return dTarget_;
}


template<class ParcelType>
inline Foam::vector& Foam::KinematicParcel<ParcelType>::U()
{
    return U_;
}


template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::rho()
{
    return rho_;
}


template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::age()
{
    return age_;
}

template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::tTurb()
{
    return tTurb_;
}

template<class ParcelType>
inline Foam::vector& Foam::KinematicParcel<ParcelType>::UTurb()
{
    return UTurb_;
}

template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::editF(int i)
{
    return F_[i];
}

template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::kv()
{
    return kv_;
}

template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::dr()
{
    return dr_;
}

template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::noNode()
{
    return noNode_;
}

template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::minmodeTheta()
{
    return minmodeTheta_;
}

template<class ParcelType>
inline Foam::scalar& Foam::KinematicParcel<ParcelType>::totalSource()
{
    return totalSource_;
}

template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::massCell
(
    const label celli
) const
{
    return rhoc_*this->mesh().cellVolumes()[celli];
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::mass() const
{
    return rho_*volume();
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::momentOfInertia() const
{
    return 0.1*mass()*sqr(d_);
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::volume() const
{
    return volume(d_);
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::volume(const scalar d)
{
    return pi/6.0*pow3(d);
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::areaP() const
{
    return areaP(d_);
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::areaP(const scalar d)
{
    return 0.25*areaS(d);
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::areaS() const
{
    return areaS(d_);
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::areaS(const scalar d)
{
    return pi*d*d;
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::Re
(
    const vector& U,
    const scalar d,
    const scalar rhoc,
    const scalar muc
) const
{
    return rhoc*mag(U - Uc_)*d/(muc + ROOTVSMALL);
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::We
(
    const vector& U,
    const scalar d,
    const scalar rhoc,
    const scalar sigma
) const
{
    return rhoc*magSqr(U - Uc_)*d/(sigma + ROOTVSMALL);
}


template<class ParcelType>
inline Foam::scalar Foam::KinematicParcel<ParcelType>::Eo
(
    const vector& a,
    const scalar d,
    const scalar sigma
) const
{
    vector dir = U_/(mag(U_) + ROOTVSMALL);
    return mag(a & dir)*(rho_ - rhoc_)*sqr(d)/(sigma + ROOTVSMALL);
}


// ************************************************************************* //
