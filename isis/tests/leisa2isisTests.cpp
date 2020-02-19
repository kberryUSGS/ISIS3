#include "leisa2isis.h"

#include <QVector>
#include <QString>

#include "UserInterface.h"
#include "FileName.h"
#include "TestUtilities.h"

#include "gmock/gmock.h"

using namespace Isis;

TEST(leisa2isisTest, leisa2isisTestDefault) {
  QString APP_XML = FileName("$ISISROOT/bin/xml/leisa2isis.xml").expanded();
  // update to use QTemporaryfFile
  QVector<QString> args = {"from=/usgs/cpkgs/isis3/testData/isis/src/newhorizons/apps/leisa2isis/tsts/qualityReplacement/input/lsb_0034931099_0x53c_sci_1.fit", "to=/home/kberry/TEMP.cub"}; 
  UserInterface options(APP_XML, args);
  leisa2isis(options);
}

