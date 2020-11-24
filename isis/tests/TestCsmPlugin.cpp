#include "TestCsmPlugin.h"
#include "TestCsmModel.h"

TestCsmPlugin::TestCsmPlugin() {
  // deliberately blank
}

TestCsmPlugin::~TestCsmPlugin() {}

std::string TestCsmPlugin::getPluginName() const {
  return "TestCsmPlugin";
}

std::string TestCsmPlugin::getManufacturer() const {
  return "TestCsmPluginCreator";
}

std::string TestCsmPlugin::getReleaseDate() const {
  return "TestDate";
}

csm::Version TestCsmPlugin::getCsmVersion() const {
  return csm::Version(3,0,3);
}

size_t TestCsmPlugin::getNumModels() const {
  return 2;
}

std::string TestCsmPlugin::getModelName(size_t modelIndex) const {
  return "TestModelName";
}

std::string TestCsmPlugin::getModelFamily(size_t modelIndex) const {
  return "TestModelFamily";
}

csm::Version TestCsmPlugin::getModelVersion(const std::string& modelName) const { 
  return csm::Version(3,0,3);
}

bool TestCsmPlugin::canModelBeConstructedFromState(const std::string& modelName,
                                    const std::string& modelState,
                                    csm::WarningList* warnings) const {
  return true;
}

bool TestCsmPlugin::canModelBeConstructedFromISD(
    const csm::Isd& imageSupportData,
    const std::string& modelName,
    csm::WarningList* warnings) const {
  return true;
}

bool TestCsmPlugin::canISDBeConvertedToModelState(
    const csm::Isd& imageSupportData,
    const std::string& modelName,
    csm::WarningList* warnings) const{
  return true;
}

std::string TestCsmPlugin::convertISDToModelState(
    const csm::Isd& imageSupportData,
    const std::string& modelName,
    csm::WarningList* warnings) const {
  return "Hello";
}

std::string TestCsmPlugin::getModelNameFromModelState(
    const std::string& modelState,
    csm::WarningList* warnings) const {
  return "TestModelNameFromModelState";
}

csm::Model* TestCsmPlugin::constructModelFromState(
      const std::string& modelState,
      csm::WarningList* warnings) const {
  TestCsmModel model;
  return &model;
}

csm::Model* TestCsmPlugin::constructModelFromISD(
    const csm::Isd& imageSupportData,
    const std::string& modelName,
    csm::WarningList* ) const {
  TestCsmModel model;
  return &model;
}

// need to create a virtual model or models now, too.
