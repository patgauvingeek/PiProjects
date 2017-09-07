#pragma once

#include <chrono>

#include "pialarm.hpp"

#include <vector>

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
      AlarmInProgress
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

      AlarmSystemState::Type state() const { return mState; }

    private:
      AlarmSystemState::Type mState;
      std::shared_ptr<db::PiAlarm> mDB;
      std::vector<std::shared_ptr<ISensorBehavior>> mSensorBehaviors;
      std::vector<std::shared_ptr<ITrigger>> mTriggers;
      
  };

}
