enum alertLevel {
    L_DEBUG, // grey
    L_INFO, // white
    L_PASS, // green
    L_WARN, // yellow
    L_FAIL, // red
    L_ERROR, // red, bold
    L_FATAL, // idk
    L_STATE
};

struct {
    bool allOK = false;
    bool statusAccel = false;
    bool statusGyro = false;
} sensors;

struct {
    bool testMode = true;
    bool showTimestamps = false;
    alertLevel minLevel = L_DEBUG; // all messages show
    bool useSerialColors = true;
} settings;

struct {
    uint8_t testRead = 5;
} pins;

struct eventList {
    bool preFlight = true;
    bool ready = false;
    bool abort = false;
};
// events.blah
struct eventList events;

struct {
    unsigned long bootTime;
    unsigned long setupCompleteTime;
    unsigned long setupDuration;
} timestamps;

const char* alertLevelNames[]  = {
    "DEBUG",
    "INFO ",
    "PASS ",
    "WARN ",
    "FAIL ",
    "ERROR",
    "FATAL",
    "STATE"
};

const char* ansiColors[] = {
    "\033[90m",
    "\033[0m",
    "\033[32m",
    "\033[33m",
    "\033[31m",
    "\033[1;31m",
    "\033[97;41m",
    "\033[36m"
};

const char* ansiReset = "\033[0m";