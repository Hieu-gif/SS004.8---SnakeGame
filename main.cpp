struct TermSaver {
    struct termios orig{};
    bool enabled = false;
    void enable() {
        if (enabled) return;
        tcgetattr(STDIN_FILENO, &orig);
        struct termios raw = orig;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);

        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        enabled = true;
    }
    void disable() {
        if (!enabled) return;
        tcsetattr(STDIN_FILENO, TCSANOW, &orig);
        enabled = false;
    }
} termSaver;
#endif

void hideCursor()  { cout << "\x1B[?25l"; }
void showCursor()  { cout << "\x1B[?25h"; }
void moveCursorHome(){ cout << "\x1B[H"; }
void clearScreenOnce() { cout << "\x1B[2J"; }

int getCharNonBlocking() {
#ifdef _WIN32
    if (_kbhit()) {
        int c = _getch();
        if (c == 0 || c == 224) { if (_kbhit()) _getch(); return 0; }
        return c;
    }
    return 0;
#else
    unsigned char ch = 0;
    ssize_t r = read(STDIN_FILENO, &ch, 1);
    if (r == 1) return ch;
    return 0;
#endif
}
