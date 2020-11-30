#include "csminit.h"
#include "spiceinit.h"

#include <QString>
#include <iostream>

#include "TestCsmPlugin.h"
#include "Fixtures.h"
#include "TestUtilities.h"
#include "StringBlob.h"
#include "FileName.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "gmock/gmock.h"

using namespace Isis;

static QString APP_XML = FileName("$ISISROOT/bin/xml/csminit.xml").expanded();

//class CSMPluginFixture : public ::testing::Test {
  
//};

TEST_F(SmallCube, CSMTest1) { //-> update to use fixture later
  QVector<QString> args = {
    "from=/scratch/csm2020-3/jesse_test_data/test_data/F02_036648_2021_XN_22N022W.cub",
    "isd=/scratch/csm2020-3/jesse_test_data/test_data/F02_036648_2021_XN_22N022W.cal.json",
  "modelname=TestModelName",
  "pluginname=TestCsmPlugin"};

  UserInterface options(APP_XML, args);

  TestCsmPlugin plugin; 

  csminit(options);

  // Get a model and a state string
  StringBlob stateString;
  Cube cub("/scratch/csm2020-3/jesse_test_data/test_data/F02_036648_2021_XN_22N022W.cub");
  cub.read(stateString);

  PvlObject blobPvl = stateString.Label(); 

  EXPECT_EQ(stateString.Name().toStdString(), "CSMState");
  EXPECT_EQ(stateString.Type().toStdString(), "String"); 
  // Better test for state string
  EXPECT_GT(stateString.string().length(), 1000);
  // check blob label ModelName and Plugin Name 
  EXPECT_EQ(QString(blobPvl.findKeyword("PluginName")), "");
  EXPECT_EQ(QString(blobPvl.findKeyword("ModelName")), "");
}

TEST_F(SmallCube, CSMinitRunTwice) {
  // Run csminit twice, make sure it runs without error
  QVector<QString> args = {
    "from=/scratch/csm2020-3/jesse_test_data/test_data/F02_036648_2021_XN_22N022W.cub",
    "isd=/scratch/csm2020-3/jesse_test_data/test_data/F02_036648_2021_XN_22N022W.cal.json"};

  UserInterface options(APP_XML, args);

  csminit(options);
  csminit(options);

  StringBlob stateString;
  Cube cub("/scratch/csm2020-3/jesse_test_data/test_data/F02_036648_2021_XN_22N022W.cub");
  cub.read(stateString);

  EXPECT_EQ(stateString.Name().toStdString(), "CSMState");
  EXPECT_EQ(stateString.Type().toStdString(), "String"); 
  EXPECT_GT(stateString.string().length(), 1000);
}
