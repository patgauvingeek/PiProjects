#include "AlarmSystem.h"

#include "SensorBehaviorFactory.h"

namespace PiAlarm
{
  AlarmSystem::AlarmSystem(std::shared_ptr<db::PiAlarm> db)
    : mState(AlarmSystemState::Unarmed)
    , mDB(db)
    , mSensorBehaviors()
  {}

  AlarmSystem::~AlarmSystem()
  {
    insertEvent(db::Event::Trigger::SystemStopped);
  }

  db::Event AlarmSystem::insertEvent(int trigger)
  {
    db::Event wEvent(*mDB);
    std::time_t wNow = std::time(nullptr);
    std::localtime(&wNow);
    wEvent.date = wNow;
    wEvent.trigger = trigger;
    wEvent.update();
    return wEvent;
  }

  db::Event AlarmSystem::insertEvent(int trigger, db::Sensor &sensor)
  {
    auto wEvent = insertEvent(trigger);
    wEvent.sensor().link(sensor);
    return wEvent;
  }

  db::Event AlarmSystem::insertEvent(int trigger, db::Sensor &sensor, db::User &user)
  {
    auto wEvent = insertEvent(trigger, sensor);
    wEvent.user().link(user);
    return wEvent;
  }
  
  db::Alarm AlarmSystem::insertAlarm()
  {
    db::Alarm wAlarm(*mDB);
    std::time_t wNow = std::time(nullptr);
    std::localtime(&wNow);
    wAlarm.date = wNow;
    wAlarm.note = "";
    wAlarm.update();
    return wAlarm;
  }

  db::Alarm AlarmSystem::insertAlarm(db::Event &event)
  {
    auto wAlarm = insertAlarm();
    wAlarm.event().link(event);
    return wAlarm;
  }

  void AlarmSystem::raiseAlarm()
  {
    mState = AlarmSystemState::AlarmInProgress;

    // AlarmNotifier
  }

  db::User AlarmSystem::getUserByRfId(std::string rfid)
  {
    return litesql::select<db::User>(*mDB, db::User::Rfid == rfid)
      .one();
  }
  
  db::Event AlarmSystem::getEventById(int id)
  {
    return litesql::select<db::Event>(*mDB, db::Event::Id == id)
      .one();
  }

  void AlarmSystem::initialize()
  {
    auto wSensors = litesql::select<db::Sensor>(*mDB)
      .orderBy(db::Sensor::Name)
      .all();
   
    for (auto &wSensor : wSensors)
    {
      auto wSensorBehavior = SensorBehaviorFactory::create(this, wSensor);
      mSensorBehaviors.push_back(wSensorBehavior);
    }

    insertEvent(db::Event::Trigger::SystemStarted);
  }

  void AlarmSystem::update() 
  {
    // sensors
    for (auto &wSensorBehavior : mSensorBehaviors)
    {
      wSensorBehavior->update();
    }
  }
  
  void AlarmSystem::arm()
  {
    if (mState == AlarmSystemState::Unarmed)
    {
      mState = AlarmSystemState::Armed;
    }
  }

  void AlarmSystem::unarm()
  { 
    if (mState != AlarmSystemState::Unarmed)
    {
      mState = AlarmSystemState::Unarmed;
    }
  }

  void AlarmSystem::log(std::string method, std::string what, int severity)
  {
    db::Log wLog(*mDB);
    std::time_t wNow = std::time(nullptr);
    std::localtime(&wNow);
    wLog.date = wNow;
    wLog.severity = severity;
    wLog.method = method;
    wLog.what = what;
    wLog.update();
  }

}
