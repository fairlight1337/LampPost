#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/bus/Bus.h>


#pragma region Constants

// The sample bus name.
const std::string cSampleBusName("TheBusName");

// The sample child bus name.
const std::string cSampleChildBusName("TheChildBusName");

// The sample topic.
const std::string cSampleTopic("/thesampletopic");

// The sample data message.
const std::string cSampleDataMessage("The sample data message.");

// The sample action topic namespace.
const std::string cSampleActionTopicNamespace("/thesampleaction");

// The bus runtime duration (in milliseconds).
const int cBusRuntimeDurationMs = 100;

#pragma endregion


#pragma region Constructor

// When the simple constructor is called, then the properties are set correctly.
TEST(Bus, WhenSimpleConstructorIsCalled_ThenThePropertiesAreSetCorrectly)
{
  // Arrange, Act.
  lp::bus::Bus bus(cSampleBusName);

  // Assert.
  EXPECT_EQ(cSampleBusName, bus.GetIdentifier());
}

// When the extended constructor is called, then the properties are set correctly.
// REMARK: Does not cover the publishing function; see the respective tests below.
TEST(Bus, WhenExtendedConstructorIsCalled_ThenThePropertiesAreSetCorrectly)
{
  // Arrange.
  std::function<void(std::shared_ptr<lp::messages::Message>)> publishingFunction =
    [](std::shared_ptr<lp::messages::Message> message) {};

  // Act.
  lp::bus::Bus bus(cSampleBusName, publishingFunction);

  // Assert.
  EXPECT_EQ(cSampleBusName, bus.GetIdentifier());
}

// When the simple constructor is called with invalid arguments, then an exception is thrown.
TEST(Bus, WhenSimpleConstructorIsCalledWithInvalidArguments_ThenAnExceptionIsThrown)
{
  // Arrange, Act, Assert.
  ASSERT_THROW(lp::bus::Bus bus(""), lp::exceptions::ArgumentNullException);
}

// When the extended constructor is called with invalid arguments, then an exception is thrown.
TEST(Bus, WhenExtendedConstructorIsCalledWithInvalidArguments_ThenAnExceptionIsThrown)
{
  // Arrange.
  std::function<void(std::shared_ptr<lp::messages::Message>)> publishingFunction =
    [](std::shared_ptr<lp::messages::Message> message) {};

  // Act, Assert.
  ASSERT_THROW(lp::bus::Bus bus("", publishingFunction), lp::exceptions::ArgumentNullException);
  ASSERT_THROW(lp::bus::Bus bus(cSampleBusName, nullptr), lp::exceptions::ArgumentNullException);
}

#pragma endregion // Constructor

#pragma region CreateChildBus

// When a second child bus with the same name is created, then an exception is thrown.
TEST(Bus, WhenSecondChildBusWithSameNameIsCreated_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);
  bus.CreateChildBus(cSampleChildBusName);

  // Act, Assert.
  ASSERT_THROW(bus.CreateChildBus(cSampleChildBusName), lp::exceptions::DuplicateKeyException);
}

// When a child bus is created with an empty name, then an exception is thrown.
TEST(Bus, WhenChildBusIsCreatedWithEmptyName_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);

  // Act, Assert.
  ASSERT_THROW(bus.CreateChildBus(""), lp::exceptions::ArgumentNullException);
}

#pragma endregion // CreateChildBus

#pragma region GetChildBus

// When a named child bus is referred to again, then that child bus is returned.
TEST(Bus, WhenNamedChildBusIsReferredToAgain_ThenThatChildBusIsReturned)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);

  // Act.
  std::shared_ptr<lp::bus::Bus> childBusCreate = bus.CreateChildBus(cSampleChildBusName);
  std::shared_ptr<lp::bus::Bus> childBusGet = bus.GetChildBus(cSampleChildBusName);

  // Act, Assert.
  EXPECT_EQ(childBusCreate, childBusGet);
}

// When a named child bus is referred to that does not exist, then an exception is thrown.
TEST(Bus, WhenNamedChildBusIsReferredToThatDoesNotExist_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);

  // Act, Assert.
  ASSERT_THROW(bus.GetChildBus(cSampleChildBusName), lp::exceptions::KeyNotFoundException);
}

#pragma endregion // GetChildBus

#pragma region Publish

// When a message is published, then the publishing function is called with the correct arguments.
TEST(Bus, WhenMessageIsPublished_ThenThePublishingFunctionIsCalledWithTheCorrectArguments)
{
  // Arrange.
  bool publishingFunctionWasCalledCorrectly = false;
  std::function<void(std::shared_ptr<lp::messages::Message>)> publishingFunction =
    [&](std::shared_ptr<lp::messages::Message> message)
    {
    if(message)
    {
      std::shared_ptr<lp::messages::Datagram> datagram = message->GetDatagram();

      if(datagram)
      {
        if(datagram->Get<std::string>() == cSampleDataMessage)
        {
          publishingFunctionWasCalledCorrectly = true;
        }
      }
    }
  };

  lp::bus::Bus bus(cSampleBusName, publishingFunction);
  std::shared_ptr<lp::bus::Publisher> publisher = bus.CreatePublisher(cSampleTopic);
  std::shared_ptr<lp::messages::Datagram> publishDatagram = std::make_shared<lp::messages::Datagram>();
  *publishDatagram = cSampleDataMessage;

  // Act.
  publisher->Publish(publishDatagram);

  // Assert.
  EXPECT_TRUE(publishingFunctionWasCalledCorrectly);
}

#pragma endregion // Publish

#pragma region Subscribe

TEST(Bus, WhenSubscribedToTopicAndMessageIsPublishedOnThatTopic_ThenTheMessageIsReceivedProperly)
{
  // Arrange.
  bool messageWasReceivedProperly = false;

  lp::bus::Bus bus(cSampleBusName);
  std::shared_ptr<lp::bus::Publisher> publisher = bus.CreatePublisher(cSampleTopic);
  std::shared_ptr<lp::messages::Datagram> publishDatagram = std::make_shared<lp::messages::Datagram>();
  *publishDatagram = cSampleDataMessage;

  std::shared_ptr<lp::bus::Subscriber> subscriber = bus.CreateSubscriber(
    cSampleTopic,
    [&](std::shared_ptr<lp::messages::Datagram> datagram)
    {
      if(datagram)
      {
        if(datagram->Get<std::string>() == cSampleDataMessage)
        {
          messageWasReceivedProperly = true;
        }
      }
    });

  std::thread busRunner(&lp::bus::Bus::Start, &bus);

  // Act.
  publisher->Publish(publishDatagram);
  std::this_thread::sleep_for(std::chrono::milliseconds(cBusRuntimeDurationMs));

  // Assert.
  EXPECT_TRUE(messageWasReceivedProperly);

  // Cleanup.
  bus.Stop();
  busRunner.join();
}

#pragma endregion // Subscribe

#pragma region ContainsSubscriber

TEST(Bus, WhenSubscriberIsCreatedInBus_ThenTheBusContainsTheSubscriber)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);

  // Act.
  std::shared_ptr<lp::bus::Subscriber> subscriber = bus.CreateSubscriber(
    cSampleTopic,
    [](std::shared_ptr<lp::messages::Datagram> datagram) {});

  // Assert.
  EXPECT_TRUE(bus.ContainsSubscriber(subscriber));
}

#pragma endregion // ContainsSubscriber

#pragma region ContainsPublisher

TEST(Bus, WhenPublisherIsCreatedInBus_ThenTheBusContainsThePublisher)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);

  // Act.
  std::shared_ptr<lp::bus::Publisher> publisher = bus.CreatePublisher(cSampleTopic);

  // Assert.
  EXPECT_TRUE(bus.ContainsPublisher(publisher));
}

#pragma endregion // ContainsPublisher

#pragma region DeleteSubscriber

TEST(Bus, WhenExistingSubscriberIsDeletedFromBus_ThenTheBusDoesNotContainsTheSubscriber)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);
  std::shared_ptr<lp::bus::Subscriber> subscriber = bus.CreateSubscriber(
    cSampleTopic,
    [](std::shared_ptr<lp::messages::Datagram> datagram) {});

  // Act.
  bus.DeleteSubscriber(subscriber);

  // Assert.
  EXPECT_FALSE(bus.ContainsSubscriber(subscriber));
}

#pragma endregion // DeleteSubscriber

#pragma region DeletePublisher

TEST(Bus, WhenExistingPublisherIsDeletedFromBus_ThenTheBusDoesNotContainsThePublisher)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);
  std::shared_ptr<lp::bus::Publisher> publisher = bus.CreatePublisher(cSampleTopic);

  // Act.
  bus.DeletePublisher(publisher);

  // Assert.
  EXPECT_FALSE(bus.ContainsPublisher(publisher));
}

#pragma endregion // DeletePublisher

#pragma region Action Provide Consume

TEST(Bus, WhenRequestingAnActionAndAnActionProviderIsPresent_ThenTheResponseIsReceived)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);
  std::shared_ptr<lp::bus::ActionProvider> provider = bus.CreateActionProvider(
    cSampleActionTopicNamespace,
    [](std::shared_ptr<lp::bus::ActionProvider> provider, std::string invocationId, std::shared_ptr<lp::messages::Datagram> request)
    {
      std::shared_ptr<lp::messages::Datagram> response = std::make_shared<lp::messages::Datagram>();
      (*response) = cSampleDataMessage;

      provider->Respond(invocationId, response);
    });
  std::shared_ptr<lp::bus::ActionConsumer> consumer = bus.CreateActionConsumer(cSampleActionTopicNamespace);

  std::thread busRunner(&lp::bus::Bus::Start, &bus);

  // Act.
  std::shared_ptr<lp::messages::Datagram> requestData = std::make_shared<lp::messages::Datagram>();
  std::shared_ptr<lp::messages::Datagram> responseData = consumer->Request(requestData);

  std::this_thread::sleep_for(std::chrono::milliseconds(cBusRuntimeDurationMs));

  // Assert.
  EXPECT_NE(nullptr, responseData);
  EXPECT_NO_THROW(responseData->Get<std::string>());
  EXPECT_STREQ(cSampleDataMessage.c_str(), responseData->Get<std::string>().c_str());

  // Cleanup.
  bus.Stop();
  busRunner.join();
}

TEST(Bus, WhenRequestingAnActionAndNoActionProviderIsPresent_ThenNoResponseIsReceived)
{
  // Arrange.
  lp::bus::Bus bus(cSampleBusName);
  std::shared_ptr<lp::bus::ActionConsumer> consumer = bus.CreateActionConsumer(cSampleActionTopicNamespace);

  std::thread busRunner(&lp::bus::Bus::Start, &bus);

  // Act.
  std::shared_ptr<lp::messages::Datagram> requestData = std::make_shared<lp::messages::Datagram>();
  std::shared_ptr<lp::messages::Datagram> responseData = consumer->Request(requestData);

  std::this_thread::sleep_for(std::chrono::milliseconds(cBusRuntimeDurationMs));

  // Assert.
  EXPECT_EQ(nullptr, responseData);

  // Cleanup.
  bus.Stop();
  busRunner.join();
}

#pragma endregion // Action Provide Consume
