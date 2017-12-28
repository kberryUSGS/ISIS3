#include <iostream>

#include <boost/numeric/ublas/symmetric.hpp>

#include <QDebug>

#include "ControlMeasure.h"
#include "ControlNet.h"
#include "ControlPoint.h"
#include "IException.h"
#include "Latitude.h"
#include "Longitude.h"
#include "Preference.h"
#include "SurfacePoint.h"

using namespace std;
using namespace boost::numeric::ublas;
using namespace Isis;

void printPoint(ControlPoint &p);

/**
  * @brief Test ControlPoint object for accuracy and correct behavior.
  *
  * @history 2010-06-30  Tracie Sucharski, Updated for binary control net and
  *                         new keywords.
  * @history 2010-08-12  Tracie Sucharski,  Keywords changed AGAIN.. Added many
  *                         more tests for conversions between lat/lon/radius
  *                         and x/y/z and between simgas and covariance matrices.
  * @history 2011-06-07 Debbie A. Cook and Tracie Sucharski - Modified point types
  *                         Ground ------> Fixed
  *                         Tie----------> Free
  * @history 2015-02-17  Andrew Stebenne, changed a reference to a local filesystem to a dummy file
  *                         (dummy.cub) to make it clearer that the .cub file being referenced
  *                         wasn't necessary.
  * @history 2017-12-21 Kristin Berry - Added tests for newly added accessor methods.
  */
int main() {
  Preference::Preferences(true);

  qDebug() << "ControlPoint unitTest";

  ControlPoint cp("C151");

  cp.SetType(ControlPoint::Fixed);
  cp.SetIgnored(true);
  cp.SetAprioriSurfacePointSource(ControlPoint::SurfacePointSource::Basemap);
  // dummy.cub does not exist: reference provided for testing.
  cp.SetAprioriSurfacePointSourceFile("./dummy.cub");
  cp.SetAprioriRadiusSource(ControlPoint::RadiusSource::DEM);
  cp.SetAprioriRadiusSourceFile("$base/dems/molaMarsPlanetaryRadius0003.cub");

  SurfacePoint point(Displacement(-424.024048, Displacement::Meters),
      Displacement(734.4311949, Displacement::Meters),
      Displacement(529.919264, Displacement::Meters),
      Distance(10, Distance::Meters),
      Distance(50, Distance::Meters),
      Distance(20, Distance::Meters));
  cp.SetAdjustedSurfacePoint(point);
  cp.SetAprioriSurfacePoint(point);
  cp.SetEditLock(true);

  ControlMeasure *cm1 = new ControlMeasure;
  cm1->SetCubeSerialNumber("Test1");
  cm1->SetIgnored(true);
  cm1->SetCoordinate(1.0, 2.0);
  cm1->SetResidual(-3.0, 4.0);
  cm1->SetDiameter(15.0);
  cm1->SetAprioriSample(2.0);
  cm1->SetAprioriLine(5.0);
  cm1->SetSampleSigma(.01);
  cm1->SetLineSigma(.21);
  cm1->SetChooserName("seedgrid");
  cm1->SetDateTime("2005-05-03T00:00:00");

  qDebug() << "Adding ControlMeasure with cube serial number [" << cm1->GetCubeSerialNumber() << "]"; // Cube Serial Number "Test1"
  cp.Add(cm1);

  printPoint(cp);

  ControlMeasure *cm2 = new ControlMeasure;
  cm2->SetCubeSerialNumber("Test2");
  cm2->SetIgnored(true);
  cm2->SetCoordinate(100.0, 200.0);
  cm2->SetDiameter(15.0);
  cm2->SetAprioriSample(2.0);
  cm2->SetAprioriLine(5.0);
  cm2->SetSampleSigma(.01);
  cm2->SetLineSigma(.21);
//  cm2->SetType(Isis::ControlMeasure::Reference);
  cm2->SetResidual(-2.0, 2.0);
  cm2->SetChooserName("seedgrid");
  cm2->SetDateTime("2005-05-03T00:00:00");
  qDebug() << "Adding ControlMeasure with cube serial number [" << cm2->GetCubeSerialNumber() << "]"; // Cube Serial Number "Test2"
  cp.Add(cm2);
  qDebug() << "Testing Edit Locking... ";
  cp.SetRefMeasure(cm2);
  if (cp.GetRefMeasure() != cm2) {
    cp.SetEditLock(false);
    cp.SetRefMeasure(cm2);
    if (cp.GetRefMeasure() == cm2) {
      qDebug() << "ok!\n";
    }
    else {
      qDebug() << "Failed!\n";
    }
  }
  else {
    qDebug() << "Failed!\n";
  }
  qDebug() << "\n";
  cp.SetEditLock(false);
  cp.SetRefMeasure(cm2);
  cp.SetEditLock(true);

  printPoint(cp);

  qDebug() << "Testing copy constructor...\n";
  ControlPoint copy(cp);
  qDebug() << "\t Also testing == operator";
  bool equal = (cp == cp);
  qDebug() << "\t\t original == its self (yes)? " << equal;
  copy.SetEditLock(false);
  copy.SetRefMeasure(0);
  equal = (cp == copy);
  qDebug() << "\t\t original == copy (no)? " << equal;
  printPoint(cp);
  qDebug() << "Testing assignment operator...\n";
  ControlPoint assignment = copy;
  printPoint(assignment);

  // Should be successful
  qDebug() << "Deleting ControlMeasure with cube serial number [" << cp.getCubeSerialNumbers().at(0).toLatin1().data() << "]";
  qDebug() << "Measure type: " << ControlMeasure::MeasureTypeToString(cp.GetMeasure(0)->GetType());
  cp.Delete(0);
  printPoint(cp);
//  qDebug() << "ReferenceIndex = " << cp.GetReferenceIndex();

//  qDebug() << "ReferenceIndex = " << cp.GetReferenceIndex();

  qDebug() << "";
  qDebug() << "Test adding control measures with identical serial numbers ...";
  try {
    cp.Add(cm2);
  }
  catch (IException &e) {
    e.print();
  }

  qDebug() << "";
  qDebug() << "Test SetAdjustedSurfacePoint ... ";
  SurfacePoint surfPt(cp.GetAdjustedSurfacePoint());
  qDebug() << "X = " << surfPt.GetX().meters();
  qDebug() << "Y = " << surfPt.GetY().meters();
  qDebug() << "Z = " << surfPt.GetZ().meters();
  qDebug() << "Latitude = " << surfPt.GetLatitude().degrees();
  qDebug() << "Longitude = " << surfPt.GetLongitude().degrees();
  qDebug() << "Radius = " << surfPt.GetLocalRadius().meters();
  surfPt.SetSpherical(Latitude(32, Angle::Degrees),
      Longitude(120, Angle::Degrees),
      Distance(1000, Distance::Meters));
  cp.SetAdjustedSurfacePoint(surfPt);
  surfPt = cp.GetAdjustedSurfacePoint();
  qDebug() << "X = " << surfPt.GetX().meters();
  qDebug() << "Y = " << surfPt.GetY().meters();
  qDebug() << "Z = " << surfPt.GetZ().meters();
  qDebug() << "Latitude = " << surfPt.GetLatitude().degrees();
  qDebug() << "Longitude = " << surfPt.GetLongitude().degrees();
  qDebug() << "Radius = " << surfPt.GetLocalRadius().meters();

  qDebug() << "";
  qDebug() << "Test conversions for apriori/adjusted covariance matrices ... ";

  symmetric_matrix<double, upper> covar;
  covar.resize(3);
  covar.clear();
  covar(0, 0) = 100.;
  covar(0, 1) = 3.55271368e-15;
  covar(0, 2) = -1.81188398e-13;
  covar(1, 0) = 3.55271368e-15;
  covar(1, 1) = 2500.;
  covar(1, 2) = 3.41060513e-13;
  covar(2, 0) = -1.81188398e-13;
  covar(2, 1) = 3.41060513e-13;
  covar(2, 2) = 400.;

  point.SetRectangularMatrix(covar);
  cp.SetAprioriSurfacePoint(point);

  //c.SetAprioriCovariance();
  point = cp.GetAprioriSurfacePoint();
  qDebug() << "Apriori Sigma X = " << point.GetXSigma().meters();
  qDebug() << "Apriori Sigma Y = " << point.GetYSigma().meters();
  qDebug() << "Apriori Sigma Z = " << point.GetZSigma().meters();

  point = cp.GetAdjustedSurfacePoint();
  qDebug() << "Adjusted Sigma X = " << point.GetXSigma().meters();
  qDebug() << "Adjusted Sigma Y = " << point.GetYSigma().meters();
  qDebug() << "Adjusted Sigma Z = " << point.GetZSigma().meters();

  qDebug();

  qDebug() << "Testing IsReferenceExplicit...";
  qDebug() << "cp:                    " << cp.IsReferenceExplicit();

  ControlMeasure *cm3 = new ControlMeasure;
  cm3->SetCubeSerialNumber("Test1");
  cm3->SetIgnored(true);
  cm3->SetCoordinate(1.0, 2.0);
  cm3->SetResidual(-3.0, 4.0);
  cm3->SetDiameter(15.0);
  cm3->SetAprioriSample(2.0);
  cm3->SetAprioriLine(5.0);
  cm3->SetSampleSigma(.01);
  cm3->SetLineSigma(.21);
  cm3->SetChooserName("seedgrid");
  cm3->SetDateTime("2005-05-03T00:00:00");

  ControlMeasure *cm4 = new ControlMeasure;
  cm4->SetCubeSerialNumber("Test2");
  cm4->SetIgnored(true);
  cm4->SetCoordinate(1.0, 2.0);
  cm4->SetResidual(-3.0, 4.0);
  cm4->SetDiameter(15.0);
  cm4->SetAprioriSample(2.0);
  cm4->SetAprioriLine(5.0);
  cm4->SetSampleSigma(.01);
  cm4->SetLineSigma(.21);
  cm4->SetChooserName("seedgrid");
  cm4->SetDateTime("2005-05-03T00:00:00");

  ControlPoint newCp;
  qDebug() << "newCp:                 " << newCp.IsReferenceExplicit();
  newCp.Add(cm3);
  qDebug() << "newCp with implicit:   " << newCp.IsReferenceExplicit();
  newCp.Add(cm4);
  newCp.SetRefMeasure(cm3);
  qDebug() << "newCp with explicit:   " << newCp.IsReferenceExplicit();
  newCp.Delete(cm3);
  qDebug() << "newCp reverted to implicit:   " << newCp.IsReferenceExplicit();

  qDebug() << "\ntesting getMeasures method...\n";
  ControlMeasure * alpha = new ControlMeasure;
  alpha->SetCubeSerialNumber("alpha");
  ControlMeasure * beta = new ControlMeasure;
  beta->SetCubeSerialNumber("beta");
  ControlPoint getMeasuresTestPoint;
  getMeasuresTestPoint.Add(alpha);
  getMeasuresTestPoint.Add(beta);
  QList< ControlMeasure * > measures = getMeasuresTestPoint.getMeasures();
  foreach (ControlMeasure * measure, measures) {
    qDebug() << measure->GetCubeSerialNumber() << "\n";
  }
  beta->SetIgnored(true);
  measures = getMeasuresTestPoint.getMeasures(true);
  foreach (ControlMeasure * measure, measures) {
    qDebug() << measure->GetCubeSerialNumber() << "\n";
  }


  qDebug() << "Testing various accessor methods... ";

//  if (cp.HasAprioriSurfacePointSourceFile()) {
//    qDebug() << "AprioriSurfacePointSourceFile: " << cp.GetAprioriSurfacePointSourceFile();
//  }
//
//  if (cp.HasAprioriRadiusSourceFile()) {
//    qDebug() << "AprioriRadiusSourceFile: " << cp.GetAprioriRadiusSourceFile();
//  }
//
//  if (cp.HasAprioriCoordinates()) {
//    qDebug() << "AprioriCoordinates: (" << cp.GetAprioriX().meters() << ", "
//                                    << cp.GetAprioriY().meters() << ", "
//                                    << cp.GetAprioriZ().meters() << ")";
//  }
//
//  if (cp.HasAdjustedCoordinates()) {
//    qDebug() << "AdjustedCoordinates: (" << cp.GetAdjustedX().meters() << ", "
//                                     << cp.GetAdjustedY().meters() << ", "
//                                     << cp.GetAdjustedZ().meters() << ")";
//  }
//
//  if (cp.HasRefMeasure()) {
//    qDebug() << "ReferenceMeasure index: " << cp.IndexOfRefMeasure();
//  }
//
//  if (cp.IsRejected()) {
//    qDebug() << "Point was rejected by jigsaw.";
//  }
//  else {
//    qDebug() << "Point was not rejected by jigsaw.";
//  }

  qDebug() << "\ntesting error handling for StringToPointType...\n";
  try {
    ControlPoint::StringToPointType("aoeu");
  }
  catch (IException &e) {
    qDebug() << "  " << e.toString() << "\n";
  }
}

void printPoint(Isis::ControlPoint &p) {
  bool wasLocked = p.IsEditLocked();
  p.SetEditLock(false);
  p.SetChooserName("cnetref");
  p.SetDateTime("2005-05-03T00:00:00");
  p.SetEditLock(wasLocked);

  ControlNet net;

  ControlPoint *copyPoint = new ControlPoint(p);
  wasLocked = copyPoint->IsEditLocked();
  copyPoint->SetEditLock(false);
  for (int i = 0; i < copyPoint->GetNumMeasures(); i++) {
    ControlMeasure *measure = copyPoint->GetMeasure(i);
    bool explicitLock = measure->IsEditLocked();
    measure->SetEditLock(false);
    measure->SetChooserName("seedgrid");
    measure->SetDateTime("2005-05-03T00:00:00");
    measure->SetEditLock(explicitLock);
  }
  copyPoint->SetEditLock(wasLocked);

  net.AddPoint(copyPoint);
  net.SetNetworkId("Identifier");
  net.SetCreatedDate("Yesterday");
  net.SetModifiedDate("Yesterday");
  net.Write("./tmp.net", true);
  Pvl tmp("./tmp.net");
  qDebug() << "Printing point:\n" << tmp << "\nDone printing point.";
  qDebug() << "";
  remove("./tmp.net");
}
