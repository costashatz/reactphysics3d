/****************************************************************************
* Copyright (C) 2009      Daniel Chappuis                                  *
****************************************************************************
* This file is part of ReactPhysics3D.                                     *
*                                                                          *
* ReactPhysics3D is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation, either version 3 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* ReactPhysics3D is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with ReactPhysics3D. If not, see <http://www.gnu.org/licenses/>.   *
***************************************************************************/

// Libraries
#include "RigidBody.h"

 // We want to use the ReactPhysics3D namespace
 using namespace reactphysics3d;

 // Constructor
 RigidBody::RigidBody(const Vector3D& position, const Kilogram& mass, const Matrix3x3& inertiaTensor)
           : Body(mass), inertiaTensor(inertiaTensor),
             currentBodyState(position, inertiaTensor.getInverse(),Kilogram(1.0/mass.getValue())),
             previousBodyState(position, inertiaTensor.getInverse(), Kilogram(1.0/mass.getValue())) {
        isMotionEnabled = true;
        isCollisionEnabled = true;
        interpolationFactor = 0.0;
 }

 // Copy-constructor
 RigidBody::RigidBody(const RigidBody& rigidBody) : Body(rigidBody), inertiaTensor(rigidBody.inertiaTensor),
            currentBodyState(rigidBody.currentBodyState), previousBodyState(rigidBody.previousBodyState) {
    this->isMotionEnabled = rigidBody.isMotionEnabled;
    this->isCollisionEnabled = rigidBody.isCollisionEnabled;
    interpolationFactor = rigidBody.interpolationFactor;
 }

 // Destructor
 RigidBody::~RigidBody() {

 };

 // Compute the linear interpolation state between the previous body state and the current body state
// This is used to avoid visual stuttering when the display and physics framerates are out of synchronization
BodyState RigidBody::getInterpolatedState() const {

    // Get the interpolation factor
    double alpha = interpolationFactor;

    // Compute the linear interpolation state
    BodyState interpolatedState = currentBodyState;     // Used to take massInverse, inertiaTensorInverse
    interpolatedState.setPosition(previousBodyState.getPosition() * (1-alpha) + currentBodyState.getPosition() * alpha);
    interpolatedState.setLinearMomentum(previousBodyState.getLinearMomentum() * (1-alpha) + currentBodyState.getLinearMomentum() * alpha);
    interpolatedState.setOrientation(Quaternion::slerp(previousBodyState.getOrientation(), currentBodyState.getOrientation(), alpha));
    interpolatedState.setAngularMomentum(previousBodyState.getAngularMomentum() * (1-alpha) + currentBodyState.getAngularMomentum() * alpha);

    // Recalculate the secondary state values
    interpolatedState.recalculate();

    // Return the interpolated state
    return interpolatedState;
}