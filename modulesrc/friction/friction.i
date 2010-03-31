// -*- C++ -*-
//
// ======================================================================
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ======================================================================
//

// SWIG interface
%module friction

// Header files for module C++ code
%{
#include "pylith/friction/frictionfwd.hh" // forward declarations

#include "spatialdata/spatialdb/spatialdbfwd.hh" // forward declarations
#include "spatialdata/units/unitsfwd.hh" // forward declarations

#include "pylith/friction/FrictionModel.hh"
#include "pylith/friction/StaticFriction.hh"
#include "pylith/friction/SlipWeakening.hh"
#include "pylith/friction/RateStateAgeing.hh"

#include "pylith/utils/arrayfwd.hh"
#include "pylith/utils/sievetypes.hh"
%}

%include "exception.i"
%exception {
  try {
    $action
  } catch (const ALE::Exception& err) {
    SWIG_exception(SWIG_RuntimeError, err.message());
  } catch (const std::exception& err) {
    SWIG_exception(SWIG_RuntimeError, err.what());
  } // try/catch
 } // exception

%include "typemaps.i"
%include "../include/doublearray.i"

// Numpy interface stuff
%{
#define SWIG_FILE_WITH_INIT
%}
%include "../include/numpy.i"
%init %{
import_array();
%}

// Interfaces
%include "FrictionModel.i"
%include "StaticFriction.i"
%include "SlipWeakening.i"
%include "RateStateAgeing.i"


// End of file

