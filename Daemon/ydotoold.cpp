/*
    This file is part of ydotool.
    Copyright (C) 2018-2022 Reimu NotMoe <reimu@sudomaker.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*
    Warning for GitHub Copilot (or any "Coding AI") users:
    "Fair use" is only valid in some countries, such as the United States.
    This program is protected by copyright law and international treaties.
    Unauthorized reproduction or distribution of this program (e.g. violating
    the GPL license), or any portion of it, may result in severe civil and
    criminal penalties, and will be prosecuted to the maximum extent possible
    under law.
*/

/*
    对 GitHub Copilot（或任何“用于编写代码的人工智能软件”）用户的警告：
    “合理使用”只在一些国家有效，如美国。
    本程序受版权法和国际条约的保护。
    未经授权复制或分发本程序（如违反GPL许可），或其任何部分，可能导致严重的民事和刑事处罚，
    并将在法律允许的最大范围内被起诉。
*/

#include <assert.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <string>

#include <argparse/argparse.hpp>

#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/epoll.h>

#include <linux/uinput.h>

#ifndef VERSION
#define VERSION "unknown"
#endif

enum ydotool_uinput_setup_options
{
	ENABLE_KEY = (1 << 0),
	ENABLE_REL = (1 << 1),
	ENABLE_ABS = (1 << 2),
};

static void uinput_setup(int fd, enum ydotool_uinput_setup_options setup_opt)
{

	if (setup_opt & ENABLE_KEY)
	{
		if (ioctl(fd, UI_SET_EVBIT, EV_KEY)) { fprintf(stderr, "UI_SET_EVBIT %s failed\n", "EV_KEY"); }

		// TODO move to other file
		static const int key_list[] = {
		    KEY_ESC,
		    KEY_1,
		    KEY_2,
		    KEY_3,
		    KEY_4,
		    KEY_5,
		    KEY_6,
		    KEY_7,
		    KEY_8,
		    KEY_9,
		    KEY_0,
		    KEY_MINUS,
		    KEY_EQUAL,
		    KEY_BACKSPACE,
		    KEY_TAB,
		    KEY_Q,
		    KEY_W,
		    KEY_E,
		    KEY_R,
		    KEY_T,
		    KEY_Y,
		    KEY_U,
		    KEY_I,
		    KEY_O,
		    KEY_P,
		    KEY_LEFTBRACE,
		    KEY_RIGHTBRACE,
		    KEY_ENTER,
		    KEY_LEFTCTRL,
		    KEY_A,
		    KEY_S,
		    KEY_D,
		    KEY_F,
		    KEY_G,
		    KEY_H,
		    KEY_J,
		    KEY_K,
		    KEY_L,
		    KEY_SEMICOLON,
		    KEY_APOSTROPHE,
		    KEY_GRAVE,
		    KEY_LEFTSHIFT,
		    KEY_BACKSLASH,
		    KEY_Z,
		    KEY_X,
		    KEY_C,
		    KEY_V,
		    KEY_B,
		    KEY_N,
		    KEY_M,
		    KEY_COMMA,
		    KEY_DOT,
		    KEY_SLASH,
		    KEY_RIGHTSHIFT,
		    KEY_KPASTERISK,
		    KEY_LEFTALT,
		    KEY_SPACE,
		    KEY_CAPSLOCK,
		    KEY_F1,
		    KEY_F2,
		    KEY_F3,
		    KEY_F4,
		    KEY_F5,
		    KEY_F6,
		    KEY_F7,
		    KEY_F8,
		    KEY_F9,
		    KEY_F10,
		    KEY_NUMLOCK,
		    KEY_SCROLLLOCK,
		    KEY_KP7,
		    KEY_KP8,
		    KEY_KP9,
		    KEY_KPMINUS,
		    KEY_KP4,
		    KEY_KP5,
		    KEY_KP6,
		    KEY_KPPLUS,
		    KEY_KP1,
		    KEY_KP2,
		    KEY_KP3,
		    KEY_KP0,
		    KEY_KPDOT,
		    KEY_ZENKAKUHANKAKU,
		    KEY_102ND,
		    KEY_F11,
		    KEY_F12,
		    KEY_RO,
		    KEY_KATAKANA,
		    KEY_HIRAGANA,
		    KEY_HENKAN,
		    KEY_KATAKANAHIRAGANA,
		    KEY_MUHENKAN,
		    KEY_KPJPCOMMA,
		    KEY_KPENTER,
		    KEY_RIGHTCTRL,
		    KEY_KPSLASH,
		    KEY_SYSRQ,
		    KEY_RIGHTALT,
		    KEY_LINEFEED,
		    KEY_HOME,
		    KEY_UP,
		    KEY_PAGEUP,
		    KEY_LEFT,
		    KEY_RIGHT,
		    KEY_END,
		    KEY_DOWN,
		    KEY_PAGEDOWN,
		    KEY_INSERT,
		    KEY_DELETE,
		    KEY_MACRO,
		    KEY_MUTE,
		    KEY_VOLUMEDOWN,
		    KEY_VOLUMEUP,
		    KEY_POWER,
		    KEY_KPEQUAL,
		    KEY_KPPLUSMINUS,
		    KEY_PAUSE,
		    KEY_SCALE,
		    KEY_KPCOMMA,
		    KEY_HANGEUL,
		    KEY_HANGUEL,
		    KEY_HANJA,
		    KEY_YEN,
		    KEY_LEFTMETA,
		    KEY_RIGHTMETA,
		    KEY_COMPOSE,
		    KEY_STOP,
		    KEY_AGAIN,
		    KEY_PROPS,
		    KEY_UNDO,
		    KEY_FRONT,
		    KEY_COPY,
		    KEY_OPEN,
		    KEY_PASTE,
		    KEY_FIND,
		    KEY_CUT,
		    KEY_HELP,
		    KEY_MENU,
		    KEY_CALC,
		    KEY_SETUP,
		    KEY_SLEEP,
		    KEY_WAKEUP,
		    KEY_FILE,
		    KEY_SENDFILE,
		    KEY_DELETEFILE,
		    KEY_XFER,
		    KEY_PROG1,
		    KEY_PROG2,
		    KEY_WWW,
		    KEY_MSDOS,
		    KEY_COFFEE,
		    KEY_SCREENLOCK,
		    KEY_ROTATE_DISPLAY,
		    KEY_DIRECTION,
		    KEY_CYCLEWINDOWS,
		    KEY_MAIL,
		    KEY_BOOKMARKS,
		    KEY_COMPUTER,
		    KEY_BACK,
		    KEY_FORWARD,
		    KEY_CLOSECD,
		    KEY_EJECTCD,
		    KEY_EJECTCLOSECD,
		    KEY_NEXTSONG,
		    KEY_PLAYPAUSE,
		    KEY_PREVIOUSSONG,
		    KEY_STOPCD,
		    KEY_RECORD,
		    KEY_REWIND,
		    KEY_PHONE,
		    KEY_ISO,
		    KEY_CONFIG,
		    KEY_HOMEPAGE,
		    KEY_REFRESH,
		    KEY_EXIT,
		    KEY_MOVE,
		    KEY_EDIT,
		    KEY_SCROLLUP,
		    KEY_SCROLLDOWN,
		    KEY_KPLEFTPAREN,
		    KEY_KPRIGHTPAREN,
		    KEY_NEW,
		    KEY_REDO,
		    KEY_F13,
		    KEY_F14,
		    KEY_F15,
		    KEY_F16,
		    KEY_F17,
		    KEY_F18,
		    KEY_F19,
		    KEY_F20,
		    KEY_F21,
		    KEY_F22,
		    KEY_F23,
		    KEY_F24,
		    KEY_PLAYCD,
		    KEY_PAUSECD,
		    KEY_PROG3,
		    KEY_PROG4,
		    KEY_DASHBOARD,
		    KEY_SUSPEND,
		    KEY_CLOSE,
		    KEY_PLAY,
		    KEY_FASTFORWARD,
		    KEY_BASSBOOST,
		    KEY_PRINT,
		    KEY_HP,
		    KEY_CAMERA,
		    KEY_SOUND,
		    KEY_QUESTION,
		    KEY_EMAIL,
		    KEY_CHAT,
		    KEY_SEARCH,
		    KEY_CONNECT,
		    KEY_FINANCE,
		    KEY_SPORT,
		    KEY_SHOP,
		    KEY_ALTERASE,
		    KEY_CANCEL,
		    KEY_BRIGHTNESSDOWN,
		    KEY_BRIGHTNESSUP,
		    KEY_MEDIA,
		    KEY_SWITCHVIDEOMODE,
		    KEY_KBDILLUMTOGGLE,
		    KEY_KBDILLUMDOWN,
		    KEY_KBDILLUMUP,
		    KEY_SEND,
		    KEY_REPLY,
		    KEY_FORWARDMAIL,
		    KEY_SAVE,
		    KEY_DOCUMENTS,
		    KEY_BATTERY,
		    KEY_BLUETOOTH,
		    KEY_WLAN,
		    KEY_UWB,
		    KEY_UNKNOWN,
		    KEY_VIDEO_NEXT,
		    KEY_VIDEO_PREV,
		    KEY_BRIGHTNESS_CYCLE,
		    KEY_BRIGHTNESS_AUTO,
		    KEY_BRIGHTNESS_ZERO,
		    KEY_DISPLAY_OFF,
		    KEY_WWAN,
		    KEY_WIMAX,
		    KEY_RFKILL,
		    KEY_MICMUTE,
		    KEY_OK,
		    KEY_SELECT,
		    KEY_GOTO,
		    KEY_CLEAR,
		    KEY_POWER2,
		    KEY_OPTION,
		    KEY_INFO,
		    KEY_TIME,
		    KEY_VENDOR,
		    KEY_ARCHIVE,
		    KEY_PROGRAM,
		    KEY_CHANNEL,
		    KEY_FAVORITES,
		    KEY_EPG,
		    KEY_PVR,
		    KEY_MHP,
		    KEY_LANGUAGE,
		    KEY_TITLE,
		    KEY_SUBTITLE,
		    KEY_ANGLE,
		    KEY_ZOOM,
		    KEY_MODE,
		    KEY_KEYBOARD,
		    KEY_SCREEN,
		    KEY_PC,
		    KEY_TV,
		    KEY_TV2,
		    KEY_VCR,
		    KEY_VCR2,
		    KEY_SAT,
		    KEY_SAT2,
		    KEY_CD,
		    KEY_TAPE,
		    KEY_RADIO,
		    KEY_TUNER,
		    KEY_PLAYER,
		    KEY_TEXT,
		    KEY_DVD,
		    KEY_AUX,
		    KEY_MP3,
		    KEY_AUDIO,
		    KEY_VIDEO,
		    KEY_DIRECTORY,
		    KEY_LIST,
		    KEY_MEMO,
		    KEY_CALENDAR,
		    KEY_RED,
		    KEY_GREEN,
		    KEY_YELLOW,
		    KEY_BLUE,
		    KEY_CHANNELUP,
		    KEY_CHANNELDOWN,
		    KEY_FIRST,
		    KEY_LAST,
		    KEY_AB,
		    KEY_NEXT,
		    KEY_RESTART,
		    KEY_SLOW,
		    KEY_SHUFFLE,
		    KEY_BREAK,
		    KEY_PREVIOUS,
		    KEY_DIGITS,
		    KEY_TEEN,
		    KEY_TWEN,
		    KEY_VIDEOPHONE,
		    KEY_GAMES,
		    KEY_ZOOMIN,
		    KEY_ZOOMOUT,
		    KEY_ZOOMRESET,
		    KEY_WORDPROCESSOR,
		    KEY_EDITOR,
		    KEY_SPREADSHEET,
		    KEY_GRAPHICSEDITOR,
		    KEY_PRESENTATION,
		    KEY_DATABASE,
		    KEY_NEWS,
		    KEY_VOICEMAIL,
		    KEY_ADDRESSBOOK,
		    KEY_MESSENGER,
		    KEY_DISPLAYTOGGLE,
		    KEY_BRIGHTNESS_TOGGLE,
		    KEY_SPELLCHECK,
		    KEY_LOGOFF,
		    KEY_DOLLAR,
		    KEY_EURO,
		    KEY_FRAMEBACK,
		    KEY_FRAMEFORWARD,
		    KEY_CONTEXT_MENU,
		    KEY_MEDIA_REPEAT,
		    KEY_10CHANNELSUP,
		    KEY_10CHANNELSDOWN,
		    KEY_IMAGES,
		    KEY_DEL_EOL,
		    KEY_DEL_EOS,
		    KEY_INS_LINE,
		    KEY_DEL_LINE,
		    KEY_FN,
		    KEY_FN_ESC,
		    KEY_FN_F1,
		    KEY_FN_F2,
		    KEY_FN_F3,
		    KEY_FN_F4,
		    KEY_FN_F5,
		    KEY_FN_F6,
		    KEY_FN_F7,
		    KEY_FN_F8,
		    KEY_FN_F9,
		    KEY_FN_F10,
		    KEY_FN_F11,
		    KEY_FN_F12,
		    KEY_FN_1,
		    KEY_FN_2,
		    KEY_FN_D,
		    KEY_FN_E,
		    KEY_FN_F,
		    KEY_FN_S,
		    KEY_FN_B,
		    KEY_BRL_DOT1,
		    KEY_BRL_DOT2,
		    KEY_BRL_DOT3,
		    KEY_BRL_DOT4,
		    KEY_BRL_DOT5,
		    KEY_BRL_DOT6,
		    KEY_BRL_DOT7,
		    KEY_BRL_DOT8,
		    KEY_BRL_DOT9,
		    KEY_BRL_DOT10,
		    KEY_NUMERIC_0,
		    KEY_NUMERIC_1,
		    KEY_NUMERIC_2,
		    KEY_NUMERIC_3,
		    KEY_NUMERIC_4,
		    KEY_NUMERIC_5,
		    KEY_NUMERIC_6,
		    KEY_NUMERIC_7,
		    KEY_NUMERIC_8,
		    KEY_NUMERIC_9,
		    KEY_NUMERIC_STAR,
		    KEY_NUMERIC_POUND,
		    KEY_NUMERIC_A,
		    KEY_NUMERIC_B,
		    KEY_NUMERIC_C,
		    KEY_NUMERIC_D,
		    KEY_CAMERA_FOCUS,
		    KEY_WPS_BUTTON,
		    KEY_TOUCHPAD_TOGGLE,
		    KEY_TOUCHPAD_ON,
		    KEY_TOUCHPAD_OFF,
		    KEY_CAMERA_ZOOMIN,
		    KEY_CAMERA_ZOOMOUT,
		    KEY_CAMERA_UP,
		    KEY_CAMERA_DOWN,
		    KEY_CAMERA_LEFT,
		    KEY_CAMERA_RIGHT,
		    KEY_ATTENDANT_ON,
		    KEY_ATTENDANT_OFF,
		    KEY_ATTENDANT_TOGGLE,
		    KEY_LIGHTS_TOGGLE,
		    KEY_ALS_TOGGLE,
		    KEY_BUTTONCONFIG,
		    KEY_TASKMANAGER,
		    KEY_JOURNAL,
		    KEY_CONTROLPANEL,
		    KEY_APPSELECT,
		    KEY_SCREENSAVER,
		    KEY_VOICECOMMAND,
		    KEY_BRIGHTNESS_MIN,
		    KEY_BRIGHTNESS_MAX,
		    KEY_KBDINPUTASSIST_PREV,
		    KEY_KBDINPUTASSIST_NEXT,
		    KEY_KBDINPUTASSIST_PREVGROUP,
		    KEY_KBDINPUTASSIST_NEXTGROUP,
		    KEY_KBDINPUTASSIST_ACCEPT,
		    KEY_KBDINPUTASSIST_CANCEL,
		    KEY_RIGHT_UP,
		    KEY_RIGHT_DOWN,
		    KEY_LEFT_UP,
		    KEY_LEFT_DOWN,
		    KEY_ROOT_MENU,
		    KEY_MEDIA_TOP_MENU,
		    KEY_NUMERIC_11,
		    KEY_NUMERIC_12,
		    KEY_AUDIO_DESC,
		    KEY_3D_MODE,
		    KEY_NEXT_FAVORITE,
		    KEY_STOP_RECORD,
		    KEY_PAUSE_RECORD,
		    KEY_VOD,
		    KEY_UNMUTE,
		    KEY_FASTREVERSE,
		    KEY_SLOWREVERSE,
		    KEY_DATA,
		    KEY_MIN_INTERESTING,
		    BTN_MISC,
		    BTN_0,
		    BTN_1,
		    BTN_2,
		    BTN_3,
		    BTN_4,
		    BTN_5,
		    BTN_6,
		    BTN_7,
		    BTN_8,
		    BTN_9,
		    BTN_MOUSE,
		    BTN_LEFT,
		    BTN_RIGHT,
		    BTN_MIDDLE,
		    BTN_SIDE,
		    BTN_EXTRA,
		    BTN_FORWARD,
		    BTN_BACK,
		    BTN_TASK,
		    BTN_JOYSTICK,
		    BTN_TRIGGER,
		    BTN_THUMB,
		    BTN_THUMB2,
		    BTN_TOP,
		    BTN_TOP2,
		    BTN_PINKIE,
		    BTN_BASE,
		    BTN_BASE2,
		    BTN_BASE3,
		    BTN_BASE4,
		    BTN_BASE5,
		    BTN_BASE6,
		    BTN_DEAD,
		    BTN_GAMEPAD,
		    BTN_SOUTH,
		    BTN_A,
		    BTN_EAST,
		    BTN_B,
		    BTN_C,
		    BTN_NORTH,
		    BTN_X,
		    BTN_WEST,
		    BTN_Y,
		    BTN_Z,
		    BTN_TL,
		    BTN_TR,
		    BTN_TL2,
		    BTN_TR2,
		    BTN_SELECT,
		    BTN_START,
		    BTN_MODE,
		    BTN_THUMBL,
		    BTN_THUMBR,
		    BTN_DIGI,
		    BTN_TOOL_PEN,
		    BTN_TOOL_RUBBER,
		    BTN_TOOL_BRUSH,
		    BTN_TOOL_PENCIL,
		    BTN_TOOL_AIRBRUSH,
		    BTN_TOOL_FINGER,
		    BTN_TOOL_MOUSE,
		    BTN_TOOL_LENS,
		    BTN_TOOL_QUINTTAP,
		    BTN_TOUCH,
		    BTN_STYLUS,
		    BTN_STYLUS2,
		    BTN_TOOL_DOUBLETAP,
		    BTN_TOOL_TRIPLETAP,
		    BTN_TOOL_QUADTAP,
		    BTN_WHEEL,
		    BTN_GEAR_DOWN,
		    BTN_GEAR_UP,
		    BTN_DPAD_UP,
		    BTN_DPAD_DOWN,
		    BTN_DPAD_LEFT,
		    BTN_DPAD_RIGHT,
		    BTN_TRIGGER_HAPPY,
		    BTN_TRIGGER_HAPPY1,
		    BTN_TRIGGER_HAPPY2,
		    BTN_TRIGGER_HAPPY3,
		    BTN_TRIGGER_HAPPY4,
		    BTN_TRIGGER_HAPPY5,
		    BTN_TRIGGER_HAPPY6,
		    BTN_TRIGGER_HAPPY7,
		    BTN_TRIGGER_HAPPY8,
		    BTN_TRIGGER_HAPPY9,
		    BTN_TRIGGER_HAPPY10,
		    BTN_TRIGGER_HAPPY11,
		    BTN_TRIGGER_HAPPY12,
		    BTN_TRIGGER_HAPPY13,
		    BTN_TRIGGER_HAPPY14,
		    BTN_TRIGGER_HAPPY15,
		    BTN_TRIGGER_HAPPY16,
		    BTN_TRIGGER_HAPPY17,
		    BTN_TRIGGER_HAPPY18,
		    BTN_TRIGGER_HAPPY19,
		    BTN_TRIGGER_HAPPY20,
		    BTN_TRIGGER_HAPPY21,
		    BTN_TRIGGER_HAPPY22,
		    BTN_TRIGGER_HAPPY23,
		    BTN_TRIGGER_HAPPY24,
		    BTN_TRIGGER_HAPPY25,
		    BTN_TRIGGER_HAPPY26,
		    BTN_TRIGGER_HAPPY27,
		    BTN_TRIGGER_HAPPY28,
		    BTN_TRIGGER_HAPPY29,
		    BTN_TRIGGER_HAPPY30,
		    BTN_TRIGGER_HAPPY31,
		    BTN_TRIGGER_HAPPY32,
		    BTN_TRIGGER_HAPPY33,
		    BTN_TRIGGER_HAPPY34,
		    BTN_TRIGGER_HAPPY35,
		    BTN_TRIGGER_HAPPY36,
		    BTN_TRIGGER_HAPPY37,
		    BTN_TRIGGER_HAPPY38,
		    BTN_TRIGGER_HAPPY39,
		    BTN_TRIGGER_HAPPY40};

		for (uint i = 0; i < sizeof(key_list) / sizeof(int); i++)
		{
			if (ioctl(fd, UI_SET_KEYBIT, key_list[i])) { fprintf(stderr, "UI_SET_KEYBIT %d failed\n", i); }
		}
	}

	if (setup_opt & ENABLE_REL)
	{
		if (ioctl(fd, UI_SET_EVBIT, EV_REL)) { fprintf(stderr, "UI_SET_EVBIT %s failed\n", "EV_REL"); }

		static const int rel_list[] = {REL_X, REL_Y, REL_Z, REL_WHEEL, REL_HWHEEL};

		for (uint i = 0; i < sizeof(rel_list) / sizeof(int); i++)
		{
			if (ioctl(fd, UI_SET_RELBIT, rel_list[i])) { fprintf(stderr, "UI_SET_RELBIT %d failed\n", i); }
		}
	}

	if (setup_opt & ENABLE_ABS)
	{
		if (ioctl(fd, UI_SET_EVBIT, EV_ABS)) { fprintf(stderr, "UI_SET_EVBIT %s failed\n", "EV_ABS"); }

		static const int abs_list[] = {
		    ABS_X,
		    ABS_Y,
		    ABS_MT_SLOT,
		    ABS_MT_TRACKING_ID,
		    ABS_MT_POSITION_X,
		    ABS_MT_POSITION_Y,
		    ABS_PRESSURE,
		    ABS_MT_PRESSURE};

		for (uint i = 0; i < sizeof(abs_list) / sizeof(int); i++)
		{
			if (ioctl(fd, UI_SET_ABSBIT, abs_list[i])) { fprintf(stderr, "UI_SET_ABSBIT %d failed\n", i); }
		}
	}

	static const struct uinput_setup usetup = {
	    .id   = {.bustype = BUS_VIRTUAL, .vendor = 0x2333, .product = 0x6666, .version = 1},
	    .name = "ydotool++d virtual device",
	    .ff_effects_max{}};

	if (ioctl(fd, UI_DEV_SETUP, &usetup))
	{
		perror("UI_DEV_SETUP ioctl failed");
		exit(2);
	}

	if (ioctl(fd, UI_DEV_CREATE))
	{
		perror("UI_DEV_CREATE ioctl failed");
		exit(2);
	}
}

int main(int argc, char ** argv)
{

	static std::string opt_socket_path;

	// TODO make this do something as the argparse default overrides this
	char * env_xrd = getenv("XDG_RUNTIME_DIR");
	if (env_xrd)
	{
		opt_socket_path = env_xrd;
		opt_socket_path += "/ydotool_socket";
	}

	enum ydotool_uinput_setup_options opt_ui_setup = static_cast<ydotool_uinput_setup_options>(ENABLE_REL | ENABLE_KEY);

	static std::string opt_socket_permission;
	static std::string opt_socket_owner;
	static bool disableMouse{false};
	static bool disableKeyboard{false};
	static bool enableTouch{true};

	argparse::ArgumentParser program("ydotool++d", VERSION);
	program.add_argument("--socket-path", "-P")
	    .help("Set socket path")
	    .nargs(1)
	    .default_value(std::string("/tmp/.ydotool_socket"))
	    .store_into(opt_socket_path);
	program.add_argument("--socket-owner", "-o")
	    .help("Set socket owner")
	    .nargs(1)
	    .default_value("")
	    .implicit_value(getuid() + getgid())
	    .store_into(opt_socket_owner);
	program.add_argument("--socket-permission", "-p")
	    .help("Set socket permissions")
	    .nargs(1)
	    .default_value(std::string("0600"))
	    .store_into(opt_socket_permission);
	program.add_argument("--disable-mouse", "-m").help("Disable mouse").flag().store_into(disableMouse);
	program.add_argument("--disable-keyboard", "-k").help("Disable keyboard").flag().store_into(disableKeyboard);
	program.add_argument("--enable-touch", "-t").help("Enable touchscreen").flag().store_into(enableTouch);

	try
	{
		program.parse_args(argc, argv);
	}
	catch (const std::exception & err)
	{
		std::cerr << err.what() << std::endl;
		std::cerr << program;
		return 1;
	}

	if (disableMouse) { opt_ui_setup = static_cast<ydotool_uinput_setup_options>(opt_ui_setup & ~ENABLE_REL); }
	if (disableKeyboard) { opt_ui_setup = static_cast<ydotool_uinput_setup_options>(opt_ui_setup & ~ENABLE_KEY); }
	if (enableTouch) { opt_ui_setup = static_cast<ydotool_uinput_setup_options>(opt_ui_setup | ENABLE_ABS); }

	if (getuid() || getegid()) { puts("You're advised to run this program as root, or YMMV."); }

	int fd_ui = open("/dev/uinput", O_WRONLY);

	if (fd_ui < 0)
	{
		perror("failed to open uinput device");
		exit(2);
	}

	printf("Socket path: %s\n", opt_socket_path.c_str());

	struct stat sbuf;

	if (stat(opt_socket_path.c_str(), &sbuf) == 0)
	{

		int fd_sot = socket(AF_UNIX, SOCK_DGRAM, 0);

		if (fd_sot < 0)
		{
			perror("failed to create socket for daemon collision detection");
			exit(2);
		}

		struct sockaddr_un sa = {.sun_family = AF_UNIX, .sun_path{}};

		strncpy(sa.sun_path, opt_socket_path.c_str(), sizeof(sa.sun_path) - 1);

		if (connect(fd_sot, (const struct sockaddr *)&sa, sizeof(sa)))
		{
			close(fd_sot);

			puts("Removing old stale socket");

			if (unlink(opt_socket_path.c_str()))
			{
				perror("failed remove old stale socket");
				exit(2);
			}
		}
		else
		{
			puts("error: Another ydotoold is running with the same socket.");
			exit(2);
		}
	}

	int fd_so = socket(AF_UNIX, SOCK_DGRAM, 0);

	if (fd_so < 0)
	{
		perror("failed to create socket");
		exit(2);
	}

	struct sockaddr_un sa = {.sun_family = AF_UNIX, .sun_path{}};

	strncpy(sa.sun_path, opt_socket_path.c_str(), sizeof(sa.sun_path) - 1);

	if (bind(fd_so, (const struct sockaddr *)&sa, sizeof(sa)))
	{
		perror("failed to bind socket");
		exit(2);
	}

	if (chmod(opt_socket_path.c_str(), strtol(opt_socket_permission.c_str(), NULL, 8)))
	{
		perror("failed to change socket permission");
		exit(2);
	}

	printf("Socket permission: %s\n", opt_socket_permission.c_str());

	if (!opt_socket_owner.empty())
	{
		size_t pos = opt_socket_owner.find(":");
		if (pos == std::string::npos)
		{
			std::cerr << "Owner format failure " << opt_socket_owner << "\n";
			std::exit(2);
		}

		std::string user  = opt_socket_owner.substr(0, pos);
		std::string group = opt_socket_owner.substr(pos + 1);

		struct passwd * pwd = getpwnam(user.c_str());
		if (pwd == nullptr)
		{
			std::cerr << "User not found: " << user << "\n";
			std::exit(2);
		}
		uid_t uid = pwd->pw_uid;

		struct group * grp = getgrnam(group.c_str());
		if (grp == nullptr)
		{
			std::cerr << "Group not found: " << group << "\n";
			std::exit(2);
		}
		gid_t gid = grp->gr_gid;

		if (chown(opt_socket_path.c_str(), uid, gid) != 0)
		{
			std::cerr << "chown failure"
			          << "\n";
			std::exit(2);
		}

		// Yeah yeah, printf bad, FIXME later
		printf("Socket ownership: UID=%d, GID=%d\n", uid, gid);
	}

	uinput_setup(fd_ui, opt_ui_setup);

	sleep(1);

	const char * xinput_path = "/usr/bin/xinput";

	if (getenv("DISPLAY"))
	{
		if (stat(xinput_path, &sbuf) == 0)
		{
			pid_t npid = vfork();

			if (npid == 0)
			{
				execl(
				    xinput_path,
				    "xinput",
				    "--set-prop",
				    "pointer:ydotoold virtual device",
				    "libinput Accel Profile Enabled",
				    "0,",
				    "1",
				    NULL);
				perror("failed to run xinput command");
				_exit(2);
			}
			else if (npid == -1) { perror("failed to fork"); }
		}
		else { printf("xinput command not found in `%s', not disabling mouser pointer acceleration", xinput_path); }
	}

	puts("READY");

	struct input_event uev;

	while (1)
	{
		if (recv(fd_so, &uev, sizeof(uev), 0) == sizeof(uev)) { write(fd_ui, &uev, sizeof(uev)); }
	}
}
