#pragma once

#include <chrono>
#include <memory>

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
      Arming,
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

      void doorOpened(db::Sensor &sensor);
      void raiseAlarm(db::Event &event);
      
      db::User getUserByRfId(std::string rfid);
      db::Event insertEvent(int trigger);
      db::Event insertEvent(int trigger, db::Sensor &sensor);

      void arm(db::Sensor &sensor, db::User &user);
      void unarm(db::Sensor &sensor, db::User &user);
      void log(std::string method, std::string what, int severity);

      AlarmSystemState::Type state() const { return mState; }

    private:
      inline std::chrono::duration<double> stateChangeDuration() const;
      inline std::chrono::duration<double> expectingUnarmedDuration() const;

      static void fillEvent(int trigger, db::Event &event);
      static void fillAlarm(db::Alarm &alarm);

    private:
      AlarmSystemState::Type mState;
      std::chrono::time_point<std::chrono::system_clock> mStateChangeTime;
      bool mExpectingUnarmed;
      std::chrono::time_point<std::chrono::system_clock> mExpectingUnarmedTime;
      std::shared_ptr<db::Event> mExpectingUnarmedEvent;
      std::shared_ptr<db::PiAlarm> mDB;
      std::vector<std::shared_ptr<ISensorBehavior>> mSensorBehaviors;
  };

}
