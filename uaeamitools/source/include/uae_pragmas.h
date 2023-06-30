/*
 *
 */

static int (*calltrap)(...) = (int (*)(...))0xF0FF60;

static int GetVersion(UBYTE *version) {
    calltrap (0, version);
    return 0;
}

static int GetUaeConfig(struct UAE_CONFIG *a) {
    calltrap (1, a);
    return 0;
}

static int SetUaeConfig(struct UAE_CONFIG *a) {
    calltrap (2, a);
    return 0;
}

static int HardReset(void) {
    calltrap (3);
    return 0;
}

static int Reset(void) {
    calltrap (4);
    return 0;
}

static int EjectDisk(ULONG drive){
    calltrap (5, "", drive);
    return 0;
}

static int InsertDisk(UBYTE *name, ULONG drive) {
    calltrap (5, name, drive);
    return 0;
}

static int EnableSound(void) {
    calltrap (6, 2);
    return 0;
}

static int DisableSound(void) {
    calltrap (6, 1);
    return 0;
}

static int EnableJoystick(void) {
    calltrap (7, 1);
    return 0;
}

static int DisableJoystick(void) {
    calltrap (7, 0);
    return 0;
}

static int SetFrameRate(ULONG rate) {
    calltrap (8, rate);
    return 0;
}

static int ChgCMemSize(ULONG mem) {
    calltrap (9, mem);
    return 0;
}

static int ChgSMemSize(ULONG mem) {
    calltrap (10, mem);
    return 0;
}

static int ChgFMemSize(ULONG mem) {
    calltrap (11, mem);
    return 0;
}

static int ChangeLanguage(ULONG lang) {
    calltrap (12, lang);
    return 0;
}

static int ExitEmu(void) {
    calltrap (13);
    return 0;
}

static int GetDisk(ULONG drive, UBYTE *name) {
    calltrap (14, drive, name);
    return 0;
}

static int DebugFunc(void) {
    calltrap (15);
    return 0;
}

/*
  Extended functionality added by T.J.Roughton 15th June 2023
  That is why the code style changes below :)
 */

static int GetUAELibVersion(UBYTE *version) {
    calltrap(128, version);
    return 0;
}

static int SetHostScreenSaver(UBYTE *state) {
    calltrap(129, state);
    return 0;
}

static int HostRunProgram(UBYTE *program) {
    calltrap(130, program);
    return 0;
}

static int AmigaRunProgram(UBYTE *program) {
    calltrap(131, program);
    return 0;
}
