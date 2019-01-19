#pragma once

#include <chrono>

#include "pialarm.hpp"

#include <vector>

#include "Event.h"
#include "ISensorBehavior.h"
#include "ITrigger.h"

namespace PiAlarm
{
  namespace AlarmSystemState
  {
    enum Type
    {
      Unarmed,
      Armed,
      AlarmTripped
    };
  }

  class AlarmSystem
  {
    public:
      AlarmSystem(std::shared_ptr<db::PiAlarm> db);
      ~AlarmSystem();

      // Database Access Methods
      db::Event insertEvent(int trigger);
      db::Event insertEvent(int trigger, db::Sensor &sensor);
      db::Event insertEvent(int trigger, db::Sensor &sensor, db::User &user);
      db::Alarm insertAlarm();
      db::Alarm insertAlarm(db::Event &event);

      db::User getUserByRfId(std::string rfid);
      db::Event getEventById(int id);

      void log(std::string method, std::string what, int severity);

      // Alarm System features
      void initialize();
      void update();

      void arm();
      void unarm();
      void raiseAlarm(db::Alarm &alarm);
      void notifyCountdown(std::chrono::seconds countdown);

      AlarmSystemState::Type state() const { return mState; }
      const SimOn::Event<AlarmSystem&, AlarmSystemState::Type> & onStateChanged() { return mStateChangedEvent; };
      const SimOn::Event<AlarmSystem&, const std::chrono::seconds&> & onCountdownChanged() { return mCountdownChangedEvent; };

    private:
      AlarmSystemState::Type mState;
      std::shared_ptr<db::PiAlarm> mDB;
      std::vector<std::shared_ptr<ISensorBehavior>> mSensorBehaviors;
      std::vector<std::shared_ptr<ITrigger>> mTriggers;

      SimOn::Event<AlarmSystem&, AlarmSystemState::Type> mStateChangedEvent;
      SimOn::Event<AlarmSystem&, const std::chrono::seconds&> mCountdownChangedEvent;
      
  };

}
