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

#include "globalConsts.hpp"

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

	static std::string opt_socket_path_default;

	// TODO make this do something as the argparse default overrides this
	char * env_xrd = getenv("XDG_RUNTIME_DIR");
	if (env_xrd)
	{
		opt_socket_path_default = env_xrd;
		opt_socket_path_default += "/ydotool_socket";
	}
	else { opt_socket_path_default = "/tmp/.ydotool_socket"; }

	enum ydotool_uinput_setup_options opt_ui_setup = static_cast<ydotool_uinput_setup_options>(ENABLE_REL | ENABLE_KEY);

	static std::string opt_socket_permission;
	static std::string opt_socket_owner;
	static std::string opt_socket_path;
	static bool disableMouse{false};
	static bool disableKeyboard{false};
	static bool enableTouch{true};

	argparse::ArgumentParser program("ydotool++d", VERSION);
	program.add_argument("--socket-path", "-P")
	    .help("Set socket path")
	    .nargs(1)
	    .default_value(opt_socket_path_default)
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
