#include <3ds.h>
#include <string.h>
#include <malloc.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include "archives.h"
#include "sha1.h"
#include "utils.h"
#include "cia.h"
#include "ttp.h"
#include "menu.h"
#include "libsu.h"

// KernelTimeMachine
// Safe CIA manager
// Licensed under GNU General Public License (GPL)
// Check out https://www.gnu.org/licenses/gpl.html

int main() {
	gfxInitDefault();
	consoleInit(GFX_TOP, &topConsole);
	consoleSelect(&topConsole);
	hidInit();
	if (suInit() == -1) {
		printf("\nError while performing kernel11 exploit.\nPlease try again. Press (START) to exit.");

		u32 kDown;
		while (aptMainLoop()) {
			hidScanInput();
			kDown = hidKeysDown();
			if (kDown & KEY_START) break;
		}
		return -1;
	}

	cfguInit();
	fsInit();
	amInit();

	fbTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	fbTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	fbBottom = gfxGetFramebuffer(GFX_BOTTOM, 0, NULL, NULL);

	u8 next = mainMenu();
	while (aptMainLoop()) {
		switch (next) {
			case 0: break;
			case 1: next = mainMenu(); break;
			//case 2: next = legitInstallMenu(); break;
			case 3: next = downgradeMenu(); break;
			//case 4: next = downgradeMSETMenu(); break;
			//case 5: next = downgradeBrowserMenu(); break;
			default: next = mainMenu();
		}
		if (next == 0) break;
	}

	consoleClear();
	puts("Rebooting");
	aptOpenSession();
	APT_HardwareResetAsync();
	aptCloseSession();

	gfxExit();
	while (1)
		svcSleepThread(UINT64_MAX);
}

/*void KernelTimeMachine(Handle amHandle) {

	am = (amHandle == NULL ? (amInit() == 0 ? *amGetSessionHandle() : NULL) : amHandle);
	if (am == NULL) return;
	main();

}*/
