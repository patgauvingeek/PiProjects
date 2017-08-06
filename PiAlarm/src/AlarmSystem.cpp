#include "AlarmSystem.h"

#include "SensorBehaviorFactory.h"

namespace PiAlarm
{
  AlarmSystem::AlarmSystem(std::shared_ptr<db::PiAlarm> db)
    : mState(AlarmSystemState::Unarmed)
    , mStateChangeTime()
    , mExpectingUnarmed(false)
    , mExpectingUnarmedTime()
    , mExpectingUnarmedEvent()
    , mDB(db)
    , mSensorBehaviors()
  {}

  AlarmSystem::~AlarmSystem()
  {
    db::Event wEvent(*mDB);
    insertEvent(db::Event::Trigger::SystemStopped, wEvent);
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

    db::Event wEvent(*mDB);
    insertEvent(db::Event::Trigger::SystemStarted, wEvent);
  }

  void AlarmSystem::update() 
  {
    // update state
    if (mState == AlarmSystemState::Arming && stateChangeDuration() > std::chrono::seconds(15))
    {
      mState = AlarmSystemState::Armed;
      mStateChangeTime = std::chrono::system_clock::now();

      db::Event wEvent(*mDB);
      insertEvent(db::Event::Trigger::SystemArmed, wEvent);
    }

    // Expecting unarmed failed.
    if (mExpectingUnarmed && expectingUnarmedDuration() > std::chrono::seconds(15))
    {
      //record alarm
      db::Alarm wAlarm(*mDB);
      insertAlarm(wAlarm);
      wAlarm.event().link(*mExpectingUnarmedEvent);

      mExpectingUnarmed = false;
      mExpectingUnarmedEvent.reset();
    }

    // sensors
    for (auto &wSensorBehavior : mSensorBehaviors)
    {
      wSensorBehavior->update();
    }
  }

  void AlarmSystem::doorOpened(db::Sensor &sensor)
  {
    auto wEvent = std::make_shared<db::Event>(*mDB);
    insertEvent(db::Event::Trigger::DoorOpened, *wEvent);
    wEvent->sensor().link(sensor);

    if (mState == AlarmSystemState::Armed)
    {
      // expecting unarmed before triggering an alarm
      if (!mExpectingUnarmed)
      {
        mExpectingUnarmed = true;
        mExpectingUnarmedTime = std::chrono::system_clock::now();
        mExpectingUnarmedEvent = wEvent;
      }
    }
  }

  db::Event AlarmSystem::insertEvent(int trigger, db::Sensor &sensor)
  {
    db::Event wEvent(*mDB);
    insertEvent(trigger, wEvent);
    wEvent.sensor().link(sensor);
    return wEvent;
  }

  void AlarmSystem::raiseAlert(db::Event &event)
  {
    // record alarm
    db::Alarm wAlarm(*mDB);
    insertAlarm(wAlarm);
    wAlarm.event().link(event);

    // ring bell
    // send messages
  }

  db::User AlarmSystem::getUserByRfId(std::string rfid)
  {
    return litesql::select<db::User>(*mDB, db::User::Rfid == rfid)
      .one();
  }
  
  void AlarmSystem::arm(db::Sensor &sensor, db::User &user)
  {
    if (mState == AlarmSystemState::Unarmed)
    {
      mState = AlarmSystemState::Arming;
      mStateChangeTime = std::chrono::system_clock::now();
      
      db::Event wEvent(*mDB);
      insertEvent(db::Event::Trigger::SystemArming, wEvent);
      wEvent.sensor().link(sensor);
      wEvent.user().link(user);
    }
  }

  void AlarmSystem::unarm(db::Sensor &sensor, db::User &user)
  { 
    if (mState != AlarmSystemState::Unarmed)
    {
      mState = AlarmSystemState::Unarmed;
      mStateChangeTime = std::chrono::system_clock::now();

      mExpectingUnarmed = false;
      mExpectingUnarmedEvent.reset();

      db::Event wEvent(*mDB);
      insertEvent(db::Event::Trigger::SystemUnarmed, wEvent);
      wEvent.sensor().link(sensor);
      wEvent.user().link(user);
    }
  }

  std::chrono::duration<double> AlarmSystem::stateChangeDuration() const
  {
    auto wNow = std::chrono::system_clock::now();
    auto wElapsed = wNow - mStateChangeTime;
    return wElapsed;
  }

  std::chrono::duration<double> AlarmSystem::expectingUnarmedDuration() const
  {
    auto wNow = std::chrono::system_clock::now();
    auto wElapsed = wNow - mStateChangeTime;
    return wElapsed;
  }

  void AlarmSystem::insertEvent(int trigger, db::Event &event)
  {
    std::time_t wNow = std::time(nullptr);
    std::localtime(&wNow);
    event.date = wNow;
    event.trigger = trigger;
    event.update();
  }

  void AlarmSystem::insertAlarm(db::Alarm &alarm)
  {
    std::time_t wNow = std::time(nullptr);
    std::localtime(&wNow);
    alarm.date = wNow;
    alarm.note = "";
    alarm.update();
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
