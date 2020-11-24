#ifndef TestCsmPlugin_h
#define TestCsmPlugin_h

#include <string>

#include "csm/Plugin.h"
#include "csm/Model.h"
#include "csm/Version.h"

#include <nlohmann/json.hpp>

class TestCsmPlugin : public csm::Plugin {
 public:
  TestCsmPlugin();
  ~TestCsmPlugin();

  std::string getPluginName() const; // return "TestCsmPlugin"

  std::string getManufacturer() const; // return "TestCsmPluginCreator"

  std::string getReleaseDate() const; // return beginning of unix epoch

  csm::Version getCsmVersion() const; //return 3.0.3

  size_t getNumModels() const; // return 1-2?

  std::string getModelName(size_t modelIndex) const; //return whatever

  std::string getModelFamily(size_t modelIndex) const; // look up what makes sense here

  csm::Version getModelVersion(const std::string& modelName) const; // blah

  bool canModelBeConstructedFromState(const std::string& modelName,
      const std::string& modelState,
      csm::WarningList* warnings = NULL) const; // just return true for everything

  bool canModelBeConstructedFromISD(
      const csm::Isd& imageSupportData,
      const std::string& modelName,
      csm::WarningList* warnings = NULL) const; // return true;

  bool canISDBeConvertedToModelState(
      const csm::Isd& imageSupportData,
      const std::string& modelName,
      csm::WarningList* warnings = NULL) const;

  std::string convertISDToModelState(
      const csm::Isd& imageSupportData,
      const std::string& modelName,
      csm::WarningList* warnings = NULL) const;

  csm::Model* constructModelFromState(
      const std::string& modelState,
      csm::WarningList* warnings = NULL) const;

  csm::Model* constructModelFromISD(
      const csm::Isd& imageSupportData,
      const std::string& modelName,
      csm::WarningList* warnings = NULL) const;

  std::string getModelNameFromModelState(
      const std::string& modelState,
      csm::WarningList* warnings = NULL) const;
};

#endif 
