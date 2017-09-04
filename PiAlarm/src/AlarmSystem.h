#pragma once

#include <chrono>

#include "pialarm.hpp"

#include <vector>

#include "ISensorBehavior.h"

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

      void initialize();
      void update();

      db::User getUserByRfId(std::string rfid);
      db::Event getEventById(int id);
      db::Event insertEvent(int trigger);
      db::Event insertEvent(int trigger, db::Sensor &sensor);
      db::Event insertEvent(int trigger, db::Sensor &sensor, db::User &user);
      db::Alarm insertAlarm();
      db::Alarm insertAlarm(db::Event &event);
      void log(std::string method, std::string what, int severity);

      void arm();
      void unarm();
      void raiseAlarm();

      AlarmSystemState::Type state() const { return mState; }

    private:
      AlarmSystemState::Type mState;
      std::shared_ptr<db::PiAlarm> mDB;
      std::vector<std::shared_ptr<ISensorBehavior>> mSensorBehaviors;
  };

}
