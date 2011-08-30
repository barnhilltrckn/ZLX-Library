#pragma once

// Todo add singleton
// make skinning easier (paramater for panel size/position)

#include "zlx.h"
#include "Utils.h"
#include "Font.h"
#include "Draw.h"
#include "Console.h"
#ifndef LIBXENON
#include "posix\input.h"
#else
#include <input/input.h>
#endif

#include <string>

#define MAX_PATH 512
#define MAX_FILES 1000
#define MAX_DISPLAYED_ENTRIES 20

struct BrowserActionEntry {
    std::string name;
    void * param;
    void (*action)(void *param);
};

typedef BrowserActionEntry * lpBrowserActionEntry;

namespace ZLX 
{
	class Browser:public ZLX::Console
	{
	public:
		void SetDefaultColor(ZLXColor color);
		void SetSelectedColor(ZLXColor color);
		void SetFocusColor(ZLXColor color);
		void SetBlurColor(ZLXColor color);
		void AddAction(lpBrowserActionEntry action);
		void SetLaunchAction(void (*action)(char *)){
			ActionLaunchFile = action;
		};
                Browser();
	private:
		ZLXTexture * bg;		

		char currentPath[MAX_PATH];
		char currentFile[MAX_PATH];
				
		void (*ActionLaunchFile)(char *filename);
		int nbAction;
		
		// Selected
		int entrySelected;
		int actionSelected;
		int panelSelected;

		float progressPct;

		// handle to bdev
		int handle;

		int display_alert;
		int display_warning;

		controller_data_s ctrl;
		controller_data_s old_ctrl;

		void RenderProgress();
		void RenderFileInfoPanel();
		void RenderActionPanel();
		void RenderTopPanel();
		void RenderFileListPanel();
		void RenderApp();
		void RenderBg();

		void ScanDir();
		void Update();
		void InitActionEntry();
		void Init(const char *pRessourcePath);
	public:
		BOOL Warning(const char *message);
		void Alert(const char *message);
		void SetProgressValue(float pct);
		void Run(const char *pRessourcePath);
	};
}