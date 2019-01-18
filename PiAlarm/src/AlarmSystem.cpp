#include "AlarmSystem.h"

#include "SensorBehaviorFactory.h"
#include "TriggerFactory.h"

namespace PiAlarm
{
  AlarmSystem::AlarmSystem(std::shared_ptr<db::PiAlarm> db)
    : mState(AlarmSystemState::Unarmed)
    , mDB(db)
    , mSensorBehaviors()
    , mTriggers()
    , mStateChangedEvent()
  {}

  AlarmSystem::~AlarmSystem()
  {
    unarm();
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

    auto wNotifiers = litesql::select<db::Notifier>(*mDB)
      .orderBy(db::Notifier::Name)
      .all();

    for (auto &wNotifier : wNotifiers)
    {
      auto wTrigger = TriggerFactory::create(this, wNotifier);
      mTriggers.push_back(wTrigger);
    }

    insertEvent(db::Event::Trigger::SystemStarted);
  }

  void AlarmSystem::update() 
  {
    for (auto &wSensorBehavior : mSensorBehaviors)
    {
      wSensorBehavior->update();
    }
  }
  
  void AlarmSystem::arm()
  {
    if (mState == AlarmSystemState::Unarmed)
    { 
      for (auto &wTrigger : mTriggers)
      {
        wTrigger->deactivate();
      }
      mState = AlarmSystemState::Armed;
      mStateChangedEvent.raise(*this, mState);
    }
  }

  void AlarmSystem::unarm()
  { 
    if (mState != AlarmSystemState::Unarmed)
    {
      for (auto &wTrigger : mTriggers)
      {
        wTrigger->deactivate();
      }
      mState = AlarmSystemState::Unarmed;
      mStateChangedEvent.raise(*this, mState);
    }
  }
  
  void AlarmSystem::raiseAlarm(db::Alarm &alarm)
  {
    for (auto &wTrigger : mTriggers)
    {
      wTrigger->activate(alarm);
    }
    mState = AlarmSystemState::AlarmTripped;
    mStateChangedEvent.raise(*this, mState);
  }

}
