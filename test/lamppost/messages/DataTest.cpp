#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/messages/Data.h>


#pragma region Equality

TEST(Data, WhenTwoInstancesHaveTheSameTypeAndContent_ThenTheyAreEqual)
{
  // Arrange.
  lp::messages::Data<std::string> data1("Test");
  lp::messages::Data<std::string> data2("Test");

  // Act, Assert.
  EXPECT_EQ(data1, data2);
}

TEST(Data, WhenTwoAbstractDataBaseInstancesHaveTheSameTypeAndContent_ThenTheyAreEqual)
{
  // Arrange.
  std::shared_ptr<lp::messages::DataBase> dataBase1 = std::make_shared<lp::messages::Data<std::string>>("Test");
  std::shared_ptr<lp::messages::DataBase> dataBase2 = std::make_shared<lp::messages::Data<std::string>>("Test");

  // Act, Assert.
  EXPECT_TRUE(dataBase1->Equals(dataBase2));
}

#pragma endregion // Equality

#pragma region Inequality

TEST(Data, WhenTwoInstancesHaveTheSameTypeButDifferentContent_ThenTheyAreNotEqual)
{
  // Arrange.
  lp::messages::Data<std::string> data1("Test 1");
  lp::messages::Data<std::string> data2("Test 2");

  // Act, Assert.
  EXPECT_NE(data1, data2);
}

TEST(Data, WhenTwoAbstractDataBaseInstancesHaveTheSameTypeButDifferentContent_ThenTheyAreNotEqual)
{
  // Arrange.
  std::shared_ptr<lp::messages::DataBase> dataBase1 = std::make_shared<lp::messages::Data<std::string>>("Test 1");
  std::shared_ptr<lp::messages::DataBase> dataBase2 = std::make_shared<lp::messages::Data<std::string>>("Test 2");

  // Act, Assert.
  EXPECT_FALSE(dataBase1->Equals(dataBase2));
}

#pragma endregion // Inequality
