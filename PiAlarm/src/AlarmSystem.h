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
      void doorClosed(db::Sensor &sensor);
      void windowOpened(db::Sensor &sensor);
      void windowClosed(db::Sensor &sensor);
      void motionDetected(db::Sensor &sensor);
      
      db::User getUserByRfId(std::string rfid);
      void arm(db::Sensor &sensor, db::User &user);
      void unarm(db::Sensor &sensor, db::User &user);
      void log(std::string method, std::string what, int severity);

      AlarmSystemState::Type state() const { return mState; }

    private:
      inline std::chrono::duration<double> stateChangeDuration() const;
      inline std::chrono::duration<double> expectingUnarmedDuration() const;

      void writeEvent(int trigger, db::Event &event);
      void writeAlarm(db::Alarm &alarm);

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
