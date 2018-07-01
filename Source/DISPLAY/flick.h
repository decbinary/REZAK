#define FLICK_PERIOD	2
enum getset {Get,Set,Init};

void Flick_run (void);
enum flick GetSetInitFlick(enum getset type, int32_t timeorstate);
int GetSetInitError(enum getset type, int32_t timeorstate);
