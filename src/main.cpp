// SimplePublishBlocking.cpp

#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);

// System thread defaults to on in 6.2.0 and later and this line is not required
#ifndef SYSTEM_VERSION_v620
SYSTEM_THREAD(ENABLED);
#endif

SerialLogHandler logHandler(LOG_LEVEL_INFO);

const std::chrono::milliseconds publishPeriod = 30s;
unsigned long lastPublish;
CloudEvent event;
unsigned long count = 0;

void setup()
{
}

void loop()
{
  if (Particle.connected())
  {
    if ((lastPublish == 0) || (millis() - lastPublish >= publishPeriod.count()))
    {
      lastPublish = millis();

      particle::Variant obj;
      obj.set("zero", count);
      obj.set("time", Time.now());

      event.name("data");
      event.data(obj);
      Particle.publish(event);

      Log.info("publishing %s", obj.toJSON().c_str());

      // Wait while sending
      waitForNot(event.isSending, 60000);

      if (event.isSent())
      {
        Log.info("publish succeeded");
        event.clear();
      }
      else if (!event.isOk())
      {
        Log.info("publish failed error=%d", event.error());
        event.clear();
      }
      count++;
    }
  }
}