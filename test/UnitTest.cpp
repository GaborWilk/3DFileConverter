#define CATCH_CONFIG_MAIN  // This tells Catch2 to provide a main() - only do this in one cpp file

#include "catch.hpp"

#include "FileConverter.h"


using namespace conv;

TEST_CASE("Read and Write converters are set", "[converter]") {
  auto& fc = FileConverter::getInstance();
  InputType input = InputType::INPUT_TYPE_OBJ;
  OutputType output = OutputType::OUTPUT_TYPE_STL;

  SECTION("Testing read converter") {
    CHECK(input == InputType::INPUT_TYPE_OBJ);
    CHECK_NOTHROW(fc.setInputFormat(input));

    input = InputType::INPUT_TYPE_UNKNOWN;
    CHECK(input == InputType::INPUT_TYPE_UNKNOWN);
    CHECK_THROWS(fc.setInputFormat(input));
  }

  SECTION("Testing write converter") {
    CHECK(output == OutputType::OUTPUT_TYPE_STL);
    CHECK_NOTHROW(fc.setOutputFormat(output));

    output = OutputType::OUTPUT_TYPE_UNKNOWN;
    CHECK(output == OutputType::OUTPUT_TYPE_UNKNOWN);
    CHECK_THROWS(fc.setOutputFormat(output));
  }
}

TEST_CASE("Read from file", "[file reader]") {
  auto& fc = FileConverter::getInstance();
  std::string input = "";

  SECTION("Testing invalid input file") {
    REQUIRE(input.empty());
    REQUIRE_THROWS(fc.read(input));
  }

  SECTION("Testing valid input file") {
    input = "../../3dfc/res/cube.obj";
    REQUIRE_FALSE(input.empty());
    REQUIRE_NOTHROW(fc.read(input));
  }
}

TEST_CASE("Write to file", "[file writer]") {
  auto& fc = FileConverter::getInstance();
  std::string output = "";

  SECTION("Testing invalid output file") {
    REQUIRE(output.empty());
    REQUIRE_THROWS(fc.read(output));
  }

  SECTION("Testing valid output file") {
    output = "../../3dfc/res/cube.stl";
    REQUIRE_FALSE(output.empty());
    REQUIRE_NOTHROW(fc.read(output));
  }
}

TEST_CASE("Rotate mesh", "[rotate]") {
  auto& fc = FileConverter::getInstance();
  glm::dvec3 rotate = {1.1, 2.2, -4.4};

  SECTION("Testing rotate functionality") {
    REQUIRE_NOTHROW(fc.rotate(rotate));
  }
}

TEST_CASE("Scale mesh", "[scale]") {
  auto& fc = FileConverter::getInstance();
  glm::dvec3 scale = {7.0, -10.6, 41.72};

  SECTION("Testing scale functionality") {
    REQUIRE_NOTHROW(fc.scale(scale));
  }
}

TEST_CASE("Translate mesh", "[translate]") {
  auto& fc = FileConverter::getInstance();
  glm::dvec3 translate = {-0.3, 28.201, 104.045};

  SECTION("Testing translate functionality") {
    REQUIRE_NOTHROW(fc.translate(translate));
  }
}

TEST_CASE("Is point inside", "[point]") {
  auto& fc = FileConverter::getInstance();
  const std::string input = "../../3dfc/res/cube.obj";
  glm::dvec3 point = {1.9, 1.9, 1.0};

  SECTION("Testing if point is inside the mesh") {
    REQUIRE_FALSE(input.empty());
    REQUIRE_NOTHROW(fc.read(input));
    REQUIRE(fc.isPointInside(point));
  }

  SECTION("Testing if point is outside the mesh") {
    point.y = 2.0;
    REQUIRE_FALSE(input.empty());
    REQUIRE_NOTHROW(fc.read(input));
    REQUIRE_FALSE(fc.isPointInside(point));
  }
}

TEST_CASE("Volume of mesh", "[volume]") {
  auto& fc = FileConverter::getInstance();
  const std::string input = "../../3dfc/res/cube.obj";

  SECTION("Testing volume functionality") {
    REQUIRE_FALSE(input.empty());
    REQUIRE_NOTHROW(fc.read(input));
    REQUIRE(fc.volume() == Approx(8.0));
  }
}

TEST_CASE("Surface of mesh", "[surface]") {
  auto& fc = FileConverter::getInstance();
  const std::string input = "../../3dfc/res/cube.obj";

  SECTION("Testing surface functionality") {
    REQUIRE_FALSE(input.empty());
    REQUIRE_NOTHROW(fc.read(input));
    REQUIRE(fc.surface() == Approx(24.0));
  }
}
