#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "pialarm.hpp"

#include "Parameters.h"
#include "StringHelper.h"
#include "AlarmSystem.h"
#include "RealTimeApplication.h"
#include "RfIdSensor.h"
#include "WebSocketMessage.h"
#include "WebSocketServer.h"

const int INVALID_ARGUMENTS = 1;
const int NOT_SUPPORTED_DATABASE = 2;
const int SENSOR_NOT_FOUND = 3;

const std::map<std::string, int> TEXT_TO_SENSOR_KIND =
{
  { "door", db::Sensor::Kind::Door },
  { "window", db::Sensor::Kind::Window },
  { "motion", db::Sensor::Kind::Motion },
  { "button", db::Sensor::Kind::Button },
  { "rfid", db::Sensor::Kind::RfId }
};

const std::map<std::string, int> TEXT_TO_NOTIFIER_KIND =
{
  { "bell", db::Notifier::Kind::Bell },
  { "message", db::Notifier::Kind::Message }
};

int addUser(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  if (args.size() != 3)
  {
    return INVALID_ARGUMENTS;
  }
  
  try 
  {
    // find the sensor...
    auto wSensor = litesql::select<db::Sensor>(*db, db::Sensor::Kind == db::Sensor::Kind::RfId)
      .one();
    auto wParameters = PiAlarm::Parameters::toVector(wSensor.parameters);
    auto wDevice = wParameters[0];
    auto wBaudRate = wParameters[1];
    // read the sensor
    SimOn::RfIdSensor wRfIdSensor(wDevice, wBaudRate);
    std::cout << "Please, scan RF ID for " << args[2] << "... " << std::flush;
    std::string wRfId;
    while (wRfIdSensor.read(wRfId) == false)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // insert user
    db::User wUser(*db);
    wUser.name = args[2];
    std::cout << wRfId << std::endl;
    wUser.rfid = wRfId;
    wUser.update();

    return 0;
  } 
  catch (litesql::NotFound e)
  {
    std::cout << "No RFID device found in the database..." << std::flush;
    return SENSOR_NOT_FOUND;
  }
}

int listUsers(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  bool wShowEvents = (args.size() == 3 && args[2] == "+events");

  auto wUsers = litesql::select<db::User>(*db)
    .all();

  for (auto &wUser : wUsers)
  {
    std::cout << PiAlarm::StringHelper::toString(wUser, wShowEvents) << std::endl;
  }
  return 0;
}

int deleteUser(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  try 
  {
    auto wUsers = litesql::select<db::User>(*db, db::User::Name == args[2])
      .one();
    wUsers.del();
  } 
  catch (litesql::NotFound e)
  {
    std::cout << "  " << args[2] << " is not a user." << std::endl;
    return INVALID_ARGUMENTS;
  }
  return 0;
}

int addSensor(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  if (args.size() < 4)
  {
    return INVALID_ARGUMENTS;
  }
  db::Sensor wSensor(*db);  
  wSensor.name = args[2];
  auto wSensorKind = TEXT_TO_SENSOR_KIND.find(args[3]);
  if (wSensorKind == TEXT_TO_SENSOR_KIND.end())
  {
    return INVALID_ARGUMENTS;
  }
  wSensor.kind = wSensorKind->second;
  wSensor.parameters = PiAlarm::Parameters::toParameters(args, 4);
  wSensor.update();

  return 0;
}

int listSensors(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  bool wShowEvents = (args.size() == 3 && args[2] == "+events");

  auto wSensors = litesql::select<db::Sensor>(*db)
    .all();

  for (auto &wSensor : wSensors)
  {
    std::cout << PiAlarm::StringHelper::toString(wSensor, wShowEvents) << std::endl;
  }
  return 0;
}

int addNotifier(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  if (args.size() < 4)
  {
    return INVALID_ARGUMENTS;
  }
  db::Notifier wNotifier(*db);  
  wNotifier.name = args[2];
  auto wNotifierKind = TEXT_TO_NOTIFIER_KIND.find(args[3]);
  if (wNotifierKind == TEXT_TO_NOTIFIER_KIND.end())
  {
    return INVALID_ARGUMENTS;
  }
  wNotifier.kind = wNotifierKind->second;
  wNotifier.parameters = PiAlarm::Parameters::toParameters(args, 4);
  wNotifier.update();

  return 0;
}

int listNotifiers(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  auto wNotifiers = litesql::select<db::Notifier>(*db)
    .all();

  for (auto &wNotifier : wNotifiers)
  {
    std::cout << PiAlarm::StringHelper::toString(wNotifier) << std::endl;
  }
  return 0;
}

int listEvents(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  auto wEvents = litesql::select<db::Event>(*db)
    .orderBy(db::Event::Id, false)
    .all();
  for (auto &wEvent : wEvents)
  {
    std::cout << PiAlarm::StringHelper::toString(wEvent) << std::endl;
  }
  return 0;
}

int clearEvents(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  auto wEvents = litesql::select<db::Event>(*db)
    .all();  
  for (auto &wEvent : wEvents)
  {
    wEvent.del();
  }
  return 0;
}

int listAlarms(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  auto wAlarms = litesql::select<db::Alarm>(*db)
    .orderBy(db::Alarm::Id, false)
    .all();

  for (auto &wAlarm : wAlarms)
  {
    std::cout << PiAlarm::StringHelper::toString(wAlarm) << std::endl;
  }
  return 0;
}

int showLogs(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  auto wLogs = litesql::select<db::Log>(*db)
    .orderBy(db::Log::Id, false)
    .all();

  for (auto &wLog : wLogs)
  {
    std::cout << PiAlarm::StringHelper::toString(wLog) << std::endl;
  }
  return 0;
}

int run(const std::vector<std::string> &args, std::shared_ptr<db::PiAlarm> db)
{
  SimOn::RealTimeApplication::initialize();

  PiAlarm::AlarmSystem wAlarmSystem(db);
  wAlarmSystem.initialize();

  SimOn::WebSocketServer wWebSocketServer;

  wWebSocketServer.onNewConnection() += [&](SimOn::WebSocketServer & sender, const SimOn::WebSocket &webSocket)
    {
      std::cout << "new socket: " << webSocket.endPoint() << std::endl;
      wAlarmSystem.log("WebSocket.onNewConnection", webSocket.endPoint(), db::Log::Severity::Info);
    };
  wWebSocketServer.onHandshakeCompleted() += [&](SimOn::WebSocketServer & sender, SimOn::WebSocket &webSocket)
    {
      std::cout << "Handshake completed: " << webSocket.endPoint() << std::endl;
      wAlarmSystem.log("WebSocket.onHandshakeCompleted", webSocket.endPoint(), db::Log::Severity::Info);
      auto wMessage = PiAlarm::WebSocketMessage::create("State", wAlarmSystem.state());
      wWebSocketServer.sendAll(wMessage);
    };
  wWebSocketServer.onCommandReceived() += [&](SimOn::WebSocketServer & sender, SimOn::WebSocket &webSocket, const std::string command)
    {
      wAlarmSystem.log("WebSocket.onCommandReceived", command, db::Log::Severity::Info);
      if (command.compare("ping") == 0)
      {
        webSocket.send("pong");
        return;
      }
      webSocket.send("error!");
      std::cout << "Unknown command \"" << command << "\" sent by " << webSocket.endPoint() << std::endl;
    };
  wAlarmSystem.onStateChanged() += [&](PiAlarm::AlarmSystem & sender, PiAlarm::AlarmSystemState::Type state)
    {
      auto wMessage = PiAlarm::WebSocketMessage::create("State", state);
      wWebSocketServer.sendAll(wMessage);
    };
  wAlarmSystem.onCountdownChanged() += [&](PiAlarm::AlarmSystem & sender, std::chrono::seconds countdown)
    {
      auto wMessage = PiAlarm::WebSocketMessage::create("Countdown", countdown);
      wWebSocketServer.sendAll(wMessage);
    };

  while(!SimOn::RealTimeApplication::isTerminated())
  {      
    wAlarmSystem.update();
    wWebSocketServer.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}

typedef std::function<int(const std::vector<std::string>&, std::shared_ptr<db::PiAlarm>)> Command;

static std::map<std::string, Command> COMMANDS = {
  { "users.add", &addUser},
  { "users.list", &listUsers},
  { "users.delete", &deleteUser},
  { "sensors.add", &addSensor },
  { "sensors.list", &listSensors },
  { "notifiers.add", &addNotifier },
  { "notifiers.list", &listNotifiers },
  { "events.list", &listEvents },
  { "events.clear", &clearEvents },
  { "alarms.list", &listAlarms },
  { "logs", &showLogs },
  { "run", &run },
};

int main(int argc, char *argv[])
{
  std::vector<std::string> wArgs;

  for(auto i = 0; i < argc; i++)
  {
    wArgs.emplace_back(argv[i]);
  }

  // using SQLite3 as backend
  auto wDB = std::make_shared<db::PiAlarm>("sqlite3", "database=pialarm.db");
  // create tables, sequences and indexes
  if (wDB->needsUpgrade())
  {
    wDB->upgrade();
  }

  if (wArgs.size() > 1)
  {
    auto search = COMMANDS.find(wArgs[1]);
    if(search != COMMANDS.end())
    {
      int wError = search->second(wArgs, wDB);
      if (wError > 0)
      {
        std::cout << "failed" << std::endl;
      }
    }
    else
    {
      std::cout << "Command not found" << std::endl;
    }
  }

}
