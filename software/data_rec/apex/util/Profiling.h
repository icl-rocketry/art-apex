#pragma once

// See https://forum.arduino.cc/t/profiling-arduino-code/977090/15

struct ProfileResult
{
  const char* Name;
  uint32_t Start, End;
  uint32_t ThreadID;
};

struct InstrumentationSession
{
  const char* Name;
};

class Instrumentor
{
  private:
    InstrumentationSession* m_CurrentSession;
    int m_ProfileCount;
  public:
    Instrumentor()
      : m_CurrentSession(nullptr), m_ProfileCount(0)
    {
    }

    void BeginSession(const char* name = "Session", const char* filepath = "results.json")
    {
      WriteHeader();
      m_CurrentSession = new InstrumentationSession{ name };
    }

    void EndSession()
    {
      WriteFooter();
      delete m_CurrentSession;
      m_CurrentSession = nullptr;
      m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult& result)
    {
      if (m_ProfileCount++ > 0)
        Serial.print(",");

      String name = result.Name;
      name.replace('"', '\'');

      Serial.println("{");
      Serial.println("\"cat\":\"function\",");
      Serial.print("\"dur\":");
      Serial.print(result.End - result.Start);
      Serial.println(",");
      Serial.println("\"name\":\"" + name + "\",");
      Serial.println("\"ph\":\"X\",");
      Serial.println("\"pid\":0,");
      Serial.println("\"tid\":0,");
      Serial.print("\"ts\":");
      Serial.println(result.Start);
      Serial.println( "}");
    }

    void WriteHeader() {
      Serial.println("{\"otherData\": {},\"traceEvents\":[");
    }

    void WriteFooter() {
      Serial.println( "]}");
    }

    static Instrumentor& Get() {
      static Instrumentor instance;
      return instance;
    }
};

class InstrumentationTimer
{
  public:
    InstrumentationTimer(const char* name)
      : m_Name(name), m_Stopped(false) {
      m_StartTimepoint = micros();
    }

    ~InstrumentationTimer() {
      if (!m_Stopped)
        Stop();
    }

    void Stop() {
      Instrumentor::Get().WriteProfile({ m_Name, m_StartTimepoint, micros(), 0 });
      m_Stopped = true;
    }
  private:
    const char* m_Name;
    bool m_Stopped;
    uint32_t m_StartTimepoint;
};


#if PROFILING
   #define PROFILE_BEGIN() Instrumentor::Get().BeginSession();
   #define PROFILE_FUNCTION() InstrumentationTimer timer(__PRETTY_FUNCTION__);
   #define PROFILE_END() Instrumentor::Get().EndSession();
#else
   #define PROFILE_BEGIN()
   #define PROFILE_FUNCTION()
   #define PROFILE_END()
#endif