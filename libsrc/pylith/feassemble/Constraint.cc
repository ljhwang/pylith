// -*- C++ -*-
//
// ----------------------------------------------------------------------
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2016 University of California, Davis
//
// See COPYING for license information.
//
// ----------------------------------------------------------------------
//

#include <portinfo>

#include "pylith/feassemble/Constraint.hh" // implementation of object methods

#include "pylith/topology/Mesh.hh" // USES Mesh
#include "pylith/topology/Field.hh" // USES Field
#include "pylith/problems/ObserversPhysics.hh" // USES ObserversPhysics
#include "pylith/problems/Physics.hh" // USES Physics

#include "pylith/utils/EventLogger.hh" // USES EventLogger
#include "pylith/utils/journals.hh" // USES PYLITH_JOURNAL_*

#include <cassert> // USES assert()
#include <typeinfo> // USES typeid()
#include <stdexcept> // USES std::runtime_error

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
pylith::feassemble::Constraint::Constraint(pylith::problems::Physics* const physics) :
    PhysicsImplementation(physics),
    _constraintLabel(""),
    _subfieldName("")
{}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor.
pylith::feassemble::Constraint::~Constraint(void) {
    deallocate();
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Set indices of constrained degrees of freedom at each location.
void
pylith::feassemble::Constraint::setConstrainedDOF(const int* flags,
                                                  const int size) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("setConstrainedDOF(flags="<<flags<<", size="<<size<<")");

    assert((size > 0 && flags) || (!size && !flags));

    _constrainedDOF.resize(size);
    for (int i = 0; i < size; ++i) {
        if (flags[i] < 0) {
            std::ostringstream msg;
            assert(_physics);
            msg << "Constrained DOF '" << flags[i] << "' must be nonnegative in constraint component '" << _physics->getIdentifier() << "'.";
            throw std::runtime_error(msg.str());
        } // if
        _constrainedDOF[i] = flags[i];
    } // for

    PYLITH_METHOD_END;
} // setConstrainedDOF


// ---------------------------------------------------------------------------------------------------------------------
// Get indices of constrained degrees of freedom.
const pylith::int_array&
pylith::feassemble::Constraint::getConstrainedDOF(void) const {
    return _constrainedDOF;
} // getConstrainedDOF


// ---------------------------------------------------------------------------------------------------------------------
// Set label marking constrained degrees of freedom.
void
pylith::feassemble::Constraint::setMarkerLabel(const char* value) {
    PYLITH_JOURNAL_DEBUG("setMarkerLabel(value="<<value<<")");

    if (strlen(value) == 0) {
        throw std::runtime_error("Empty string given for boundary condition constraint label.");
    } // if

    _constraintLabel = value;
} // setMarkerLabel


// ---------------------------------------------------------------------------------------------------------------------
// Get label marking constrained degrees of freedom.
const char*
pylith::feassemble::Constraint::getMarkerLabel(void) const {
    return _constraintLabel.c_str();
} // getMarkerLabel


// ---------------------------------------------------------------------------------------------------------------------
// Set name of constrained solution subfield.
void
pylith::feassemble::Constraint::setSubfieldName(const char* value) {
    PYLITH_JOURNAL_DEBUG("setSubfieldName(value="<<value<<")");

    if (!value || (0 == strlen(value))) {
        std::ostringstream msg;
        assert(_physics);
        msg << "Empty string given for name of solution subfield for constraint '" << _physics->getIdentifier()
            <<"'.";
        throw std::runtime_error(msg.str());
    } // if
    _subfieldName = value;
} // setSubfieldName


// ---------------------------------------------------------------------------------------------------------------------
// Get name of constrained solution subfield.
const char*
pylith::feassemble::Constraint::getSubfieldName(void) const {
    return _subfieldName.c_str();
} // getSubfieldName


// ---------------------------------------------------------------------------------------------------------------------
// Set constraint kernel.
void
pylith::feassemble::Constraint::setKernelConstraint(const PetscPointFunc kernel) {
    _kernelConstraint = kernel;
} // setKernelConstraint


// ---------------------------------------------------------------------------------------------------------------------
// Update at beginning of time step.
void
pylith::feassemble::Constraint::prestep(const double t,
                                        const double dt) { // prestep
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("prestep(t="<<t<<", dt="<<dt<<") empty method");

    // Default is to do nothing.

    PYLITH_METHOD_END;
} // prestep


// ---------------------------------------------------------------------------------------------------------------------
// Update at end of time step.
void
pylith::feassemble::Constraint::poststep(const PylithReal t,
                                         const PylithInt tindex,
                                         const PylithReal dt,
                                         const pylith::topology::Field& solution) {
    PYLITH_METHOD_BEGIN;
    PYLITH_JOURNAL_DEBUG("poststep(t="<<t<<", dt="<<dt<<")");

    const bool infoOnly = false;
    assert(_observers);
    _observers->notifyObservers(t, tindex, solution, infoOnly);

    PYLITH_METHOD_END;
} // poststep


// End of file
