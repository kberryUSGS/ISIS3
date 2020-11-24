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
    "isd=/scratch/csm2020-3/jesse_test_data/test_data/F02_036648_2021_XN_22N022W.cal.json"};

  UserInterface options(APP_XML, args);

  csminit(options);

  // get a model and a state string
/*  FileName file("/scratch/csm2020-3/jesse_test_data/test_data/F02_036648_2021_XN_22N022W.cub");
  StringBlob stateString("CSMState", "String", file.expanded());
  EXPECT_EQ(stateString.Name().toStdString(), "CSMState");
  EXPECT_EQ(stateString.Type().toStdString(), "String"); 
  EXPECT_EQ(stateString.string(), "String");*/

  // run csminit twice
  csminit(options);

  // can do this after instantiation of model etc...
  TestCsmPlugin plugin;
  EXPECT_EQ(plugin.getPluginName(), "");
  EXPECT_EQ(plugin.getManufacturer(), "");
  EXPECT_EQ(plugin.getReleaseDate(), "");
}

