#include "ydotool.hpp"

#include "globalConsts.hpp"

#include <stdint.h>
#include <string.h>
#include <termios.h>

static const int32_t ascii2ctrlcode_map[12] = {
	KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT, -1, -1, -1, -1, -1, -1, KEY_PAGEUP, KEY_PAGEDOWN
};

static int opt_key_delay_ms = 20;
static int opt_key_hold_ms = 20;
static int opt_next_delay_ms = 0;

static struct termios old_tio;

static void restore_terminal() {
    // Restore the old terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

static void handle_signal(int sig) {
    if (sig == SIGINT) {
        restore_terminal();
        exit(0);
    }
}

static void configure_terminal() {
    struct termios new_tio;
    
    // Get the current terminal attributes
    tcgetattr(STDIN_FILENO, &old_tio);
    
    // Set the new attributes for the terminal (non-canonical, no echo)
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    atexit(restore_terminal); // Ensure terminal settings are restored on exit
    signal(SIGINT, handle_signal); // Handle CTRL-C
}

int tool_stdin(int argc, char **argv) {
    configure_terminal(); // Set terminal to raw mode

    printf("Type anything (CTRL-C to exit):\n");

    while (1) {
		char buffer[4] = {0};
		read(STDIN_FILENO, buffer, 3);

		printf("Key code: %d %d %d\n", buffer[0], buffer[1], buffer[2]);

		char c = buffer[0];

		// Convert char to keycode and flags based on the ascii2keycode_map
		int kdef = ascii2keycode_map[(int)c];

		if ((int)buffer[0] == 27 && (int)buffer[1] == 91 && (int)buffer[2] >= 65 && (int)buffer[2] <= 76) {
			kdef = ascii2ctrlcode_map[(int)buffer[2] - 65];
		}

		if (kdef == -1) continue; // Skip unsupported characters
		printf("  Maps to: %d\n", kdef);

		uint16_t kc = kdef & 0xffff; // Extract keycode
		bool isUppercase = (kdef & FLAG_UPPERCASE) != 0;
		bool isCtrl = (kdef & FLAG_CTRL) != 0;

		// Emit key events
		if (isUppercase) {
		printf("  Sending shift\n");
			uinput_emit(EV_KEY, KEY_LEFTSHIFT, 1, 1); // Press shift for uppercase
		}
		if (isCtrl) {
			printf("  Sending ctrl\n");
			uinput_emit(EV_KEY, KEY_LEFTCTRL, 1, 1); // Press ctrl
			}
			uinput_emit(EV_KEY, kc, 1, 1); // Key down
			usleep(opt_key_hold_ms * 1000); // Hold key
			uinput_emit(EV_KEY, kc, 0, 1); // Key up
		if (isCtrl) {
			uinput_emit(EV_KEY, KEY_LEFTCTRL, 0, 1); // Release ctrl
		}
		if (isUppercase) {
			uinput_emit(EV_KEY, KEY_LEFTSHIFT, 0, 1); // Release shift for uppercase
		}

		usleep(opt_key_delay_ms * 1000); // Delay between keys
    }

    return 0;
}
