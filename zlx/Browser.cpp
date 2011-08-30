#include "Browser.h"
#include "Bgshader.h"
#ifdef LIBXENON
#include <debug.h>
#include <xenos/xenos.h>

#include <diskio/ata.h>
extern "C" {
#include <input/input.h>
#include <xenon_nand/xenon_sfcx.h>
}
#include <console/console.h>
#include <diskio/diskio.h>
#include <usb/usbmain.h>
#include <time/time.h>
#include <xenon_soc/xenon_power.h>
#include <xenon_smc/xenon_smc.h>

#include <dirent.h>
#include <elf/elf.h>
#define FILE_TYPE	8
#define DIR_TYPE        4
#else
#include "posix\dirent.h"
#endif
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <diskio/diskio.h>

#define TR {printf("[Trace] in function %s, line %d, file %s\n",__FUNCTION__,__LINE__,__FILE__);}

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
//#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define CLAMP(x,low,high) {if(x<low)x=low;if(x>high)x=high;}

#define WHITE			0x00FFFFFF
#define BLACK			0x00000000
#define BLUE 			0x00FF0000
#define RED				0x000000FF
#define GREEN			0x0000FF00
#define BLUR_ALPHA 		0x88000000
#define ALPHA			0xFF000000
#define WARNING_COLOR	0xFF6f641a
#define ALERT_COLOR		0xFF1212b4

static unsigned int DefaultColor = WHITE | BLUR_ALPHA;
static unsigned int SelectedColor = WHITE | ALPHA;
static unsigned int FocusColor = WHITE | ALPHA;
static unsigned int BlurColor = WHITE | BLUR_ALPHA;

static unsigned int ProgressColorBack = BLACK | ALPHA;
static unsigned int ProgressColorFront = GREEN | ALPHA;

enum {
    PANEL_FILE_LIST = 0,
    PANEL_ACTION,
    PANEL_PROGRESS
};

lpBrowserActionEntry vAction[16];

typedef struct FileEntry {
    std::string name;
    int type;
} FileEntry;

static bool vEntrySort(FileEntry i, FileEntry j) {
    if (i.type == j.type) {
        return i.name < j.name;
    } else {
        return (i.type > j.type);
    }
}

static std::vector<FileEntry> vEntry;

// move to util ??

static void append_dir_to_path(char * path, char * dir) {
    if (!strcmp(dir, "..")) {
        int i = strlen(path);
        int delimcount = 0;

        while (i >= 0 && delimcount < 2) {
            if (path[i] == '/') {
                ++delimcount;

                if (delimcount > 1) {
                    path[i + 1] = '\0';
                }
            }
            --i;
        }
    } else if (!strcmp(dir, ".")) {
        return;
    } else {
        strcat(path, dir);
        strcat(path, "/");
    }
}

namespace ZLX {

    static float ActionPanelPosition[3] = {-0.65f, 0.2f, 0.07f};
    static float FileInfoPanelPosition[3] = {-1.f, 0.65f, 0.08f};
    static float FileListPanelPosition[3] = {-0.75f, -1.f, 0.07f};

    void Browser::SetDefaultColor(ZLXColor color) {
        DefaultColor = color;
    }

    void Browser::SetSelectedColor(ZLXColor color) {
        SelectedColor = color;
    }

    void Browser::SetFocusColor(ZLXColor color) {
        FocusColor = color;
    }

    void Browser::SetBlurColor(ZLXColor color) {
        BlurColor = color;
    }

    Browser::Browser() {
        entrySelected = 0;
        panelSelected = 0;
        actionSelected = 0;

        display_warning = 0;
        display_alert = 0;

        progressPct = 0.0f;

        nbAction = 0;
        ActionLaunchFile = NULL;
    }

    void Browser::RenderProgress() {

        float posX = -0.5f;
        float posY = -0.15f;

        float w = 1.f;
        float h = 0.3f;

        float borderSize = 0.03f;

        // white border
        Draw::DrawColoredRect(posX, posY, w, h, WHITE | ALPHA);
        // black bg
        Draw::DrawColoredRect(
                posX + (borderSize / 2),
                posY + (borderSize / 2),
                w - borderSize,
                h - borderSize,
                ProgressColorBack
                );

        int PctColor = ProgressColorFront;

        // progress things ...
        Draw::DrawColoredRect(
                posX + (borderSize / 2),
                posY + (borderSize / 2),
                (w - borderSize) * progressPct,
                h - borderSize,
                PctColor
                );
    }

    void Browser::AddAction(lpBrowserActionEntry action) {
        if (action) {
            vAction[nbAction] = action;
            nbAction++;
        }
    }

    void Browser::RenderFileInfoPanel() {
#if 0
        if (vEntry.size() == 0)
            return;

        unsigned int color = DefaultColor;
        float leftOffset = -1.f;
        float topOffset = 0.65f;

        if (panelSelected == PANEL_FILE_LIST) {
            color &= FocusColor;
        } else {
            color &= BlurColor;
        }

        char bottomInfo[256];
        sprintf(bottomInfo, "File %d/%d", (entrySelected + 1), vEntry.size());
        m_font.DrawTextF(bottomInfo, color, FileInfoPanelPosition[0], FileInfoPanelPosition[1]);
        m_font.DrawTextF("Some Info here ...", color, FileInfoPanelPosition[0], FileInfoPanelPosition[1] + FileInfoPanelPosition[2]);
#endif
    }

    void Browser::RenderActionPanel() {

        for (int i = 0; i < nbAction; i++) {
            float posY = (ActionPanelPosition[0])+(ActionPanelPosition[2] * (i)); // + topOffset;
            unsigned int color = DefaultColor;
            if (actionSelected == i) {
                color = SelectedColor;
            }

            if (panelSelected == PANEL_ACTION) {
                color &= FocusColor;
            } else {
                color &= BlurColor;
            }

            if (vAction[i])
                m_font.DrawTextF(vAction[i]->name.c_str(), color, ActionPanelPosition[1], posY);
        }
    }

    void Browser::RenderTopPanel() {
        // m_font.DrawTextF("Everyday i'm browsin'", 0xFFFFFFFF, -1.f, -1.f);		
        m_font.Scale(0.8f);
        m_font.DrawTextF(currentPath, SelectedColor, -1.f, -0.83f);
        m_font.Scale(1.0f);
    }

    void Browser::RenderFileListPanel() {
        int start, end;
        float topOffset = -0.75f;
        float leftOffset = -1.f;
        float lineHeight = 0.07f;

        int size = vEntry.size();

        if (size == 0)
            return;

        start = entrySelected - MAX_DISPLAYED_ENTRIES / 2;
        if (start < 0)
            start = 0;

        end = MIN(MAX_DISPLAYED_ENTRIES, vEntry.size() - start) + start;

        if (end > size)
            end = size;

        for (int k = start; k < end; k++) {

            float posY = (FileListPanelPosition[0])+(FileListPanelPosition[2] * (k - start)); // + topOffset;
            unsigned int color = DefaultColor;
            if (entrySelected == k) {
                color = SelectedColor;
            }

            if (panelSelected == PANEL_FILE_LIST) {
                color &= FocusColor;
            } else {
                color &= BlurColor;
            }

            //printf("%s - %d\r\n", vEntry.at(k).name.c_str(), vEntry.at(k).type);


            if (vEntry.at(k).type == 1) {
                m_font.DrawTextF("[Dir]", color, -0.2f, posY);
            }
            m_font.DrawTextF(vEntry.at(k).name.c_str(), color, FileListPanelPosition[1], posY);
        }

    }

    void Browser::RenderBg() {
        if (bg == NULL)
            Bgshader::RenderBg();
        else
            Draw::DrawTexturedRect(-1.0f, -1.0f, 2.0f, 2.0f, bg);
    }

    void Browser::RenderApp() {
        // Begin to draw
        Begin();

        // Draw bg
        RenderBg();

        // Draw some text using font
        m_font.Begin();
        // need to fix that ..
        //m_font.DrawTextF("Error ?", SelectedColor, -0.55f, 0.3f);

        // Top part
        RenderTopPanel();

        // Render file listing
        RenderFileListPanel();

        // Render bottom part
        RenderFileInfoPanel();

        RenderActionPanel();

        if (panelSelected == PANEL_PROGRESS)
            RenderProgress();

        // Finished to draw font things ...
        m_font.End();

        // Draw all text + form
        Render();
        // Draw is finished
        End();
    }

    BOOL Browser::Warning(const char *message) {
        display_warning = 1;
        unsigned int color = WARNING_COLOR;

        memset(&old_ctrl, 0, sizeof (struct controller_data_s));
        memset(&ctrl, 0, sizeof (struct controller_data_s));

        while (display_warning == 1) {

            // Begin to draw
            Begin();

            // Draw bg
            RenderBg();

            Draw::DrawColoredRect(-0.5f, -0.5f, 1.0f, 1.0f, color);

            m_font.Begin();

            // need to fix that ..
            //m_font.DrawTextF("Error ?", SelectedColor, -0.55f, 0.3f);


            m_font.DrawTextF(message, SelectedColor, -0.55f, -0.5f);

            m_font.DrawTextF("Press X to continue", SelectedColor, -0.55f, 0.3f);
            m_font.DrawTextF("Press B to cancel", SelectedColor, 0.00f, 0.3f);

            m_font.End();

            // Draw all text + form
            Render();

            // Draw is finished
            End();

            // Update code ...
            {
                usb_do_poll();
                get_controller_data(&ctrl, 0);

                if (ctrl.b & !old_ctrl.b) {
                    display_warning = 0;
                    return FALSE;
                }
                if (ctrl.x & !old_ctrl.x) {
                    display_warning = 0;
                    return TRUE;
                }
                // Save the old value
                old_ctrl = ctrl;
                memset(&ctrl, 0, sizeof (struct controller_data_s));
            }
        }
        return FALSE;
    }

    void Browser::Alert(const char *message) {
        display_alert = 1;
        unsigned int color = ALERT_COLOR;

        memset(&old_ctrl, 0, sizeof (struct controller_data_s));
        memset(&ctrl, 0, sizeof (struct controller_data_s));

        while (display_alert == 1) {
            // Begin to draw
            Begin();

            RenderBg();

            Draw::DrawColoredRect(-0.5f, -0.5f, 1.0f, 1.0f, color);

            m_font.Begin();

            // need to fix that ..
            //m_font.DrawTextF("Error ?", SelectedColor, -0.55f, 0.3f);

            m_font.DrawTextF(message, SelectedColor, -0.55f, -0.5f);
            m_font.DrawTextF("Press B to continue", SelectedColor, -0.3f, 0.3f);

            m_font.End();

            // Draw all text + form
            Render();

            // Draw is finished
            End();

            // Update code ...
            {
                usb_do_poll();
                get_controller_data(&ctrl, 0);

                if (ctrl.b & !old_ctrl.b) {
                    display_alert = 0;
                    return;
                }
                // Save the old value
                old_ctrl = ctrl;
                memset(&ctrl, 0, sizeof (struct controller_data_s));
            }
        }
    }

    void Browser::SetProgressValue(float pct) {
        if (pct > 0.0f) {
            panelSelected = PANEL_PROGRESS;
            progressPct = pct;

            RenderApp();
        } else {
            panelSelected = PANEL_FILE_LIST;
        }
    }

    void Browser::Run(const char *pRessourcePath) {
        Init(pRessourcePath);

        // Disable auto render on format
        SetAutoRender(0);
#ifdef WIN32
        strcpy(currentPath, "c:/");
#else
        //        strcpy(currentPath, "uda:/");
        handle = -1;
        const char * s = NULL;
        handle = bdev_enum(handle, &s);
        if (handle < 0) {
            TR;
            strcpy(currentPath, "uda:/");
        } else {
            strcpy(currentPath, s);
            strcat(currentPath, ":/");
            TR;

            TR;
        }
        ScanDir();
#endif

        while (1) {
            RenderApp();
            Update();
        }
    }

    void Browser::ScanDir() {
        entrySelected = 0;


        //printf("\r\n\r\nScanDir : %s\r\n\r\n\r\n", currentPath);
        DIR * rep = opendir(currentPath);
        vEntry.clear();

        if (rep != NULL) {
            struct dirent * ent;

            while ((ent = readdir(rep)) != NULL) {
                // never display hidden files
                if (ent->d_name[0] == '.')
                    continue;
                //printf("%s\r\n", ent->d_name);
                FileEntry currentEntry;
                currentEntry.name = ent->d_name;
                currentEntry.type = (ent->d_type == DIR_TYPE) ? 1 : 0;
                vEntry.push_back(currentEntry);
            }

            closedir(rep);
        }
        // Sort array
        std::sort(vEntry.begin(), vEntry.end(), vEntrySort);

        return;
    }

    void Browser::Update() {
#ifdef WIN32
        W32Update();
#endif
        usb_do_poll();

        get_controller_data(&ctrl, 0);
        {
            //if (get_controller_data(&ctrl, 0)) {
            if (ctrl.up && !old_ctrl.up) {
                switch (panelSelected) {
                    case PANEL_FILE_LIST:
                        entrySelected--;
                        break;
                    case PANEL_ACTION:
                        actionSelected--;
                        break;
                }
            }

            if (ctrl.down && !old_ctrl.down) {
                switch (panelSelected) {
                    case PANEL_FILE_LIST:
                        entrySelected++;
                        break;
                    case PANEL_ACTION:
                        actionSelected++;
                        break;
                }
            }
            if (ctrl.b && !old_ctrl.b) {
                switch (panelSelected) {
                    case PANEL_FILE_LIST:
                        append_dir_to_path(currentPath, "..");
                        ScanDir();
                        break;
                }
            }
            if (ctrl.select && !old_ctrl.select) {
#ifdef LIBXENON
                switch (panelSelected) {
                    case PANEL_FILE_LIST:
                        const char * s;
                        do {
                            handle = bdev_enum(handle, &s);
                        } while (handle < 0);

                        strcpy(currentPath, s);
                        strcat(currentPath, ":/");
                        ScanDir();
                        break;
                }
#endif
            }
            if
                (
                    (ctrl.start && !old_ctrl.start) || (ctrl.a && !old_ctrl.a)
                    ) {
                switch (panelSelected) {
                    case PANEL_FILE_LIST:
                    {
                        if (vEntry.size() == 0)
                            break;

                        FileEntry currentEntry = vEntry.at(entrySelected);

                        if (currentEntry.type == 1) {
                            append_dir_to_path(currentPath, (char*) currentEntry.name.c_str());
                            ScanDir();
                        } else {
                            sprintf(currentFile, "%s/%s", currentPath, currentEntry.name.c_str());
                            if (ActionLaunchFile != NULL)
                                ActionLaunchFile(currentFile);
                        }
                        break;
                    }
                    case PANEL_ACTION:
                    {
                        if (nbAction == 0)
                            break;

                        // Exec action ...
                        //FileEntry currentEntry = vEntry.at(entrySelected);
                        lpBrowserActionEntry currentAction = vAction[actionSelected];
                        if (currentAction->action != NULL) {
                            //sprintf((char*)currentAction.param, "%s/%s", currentPath, currentEntry.name.c_str());
                            //currentAction->param = 
                            currentAction->action(currentAction);
                        }
                        break;
                    }
                }
            }

            if (ctrl.left && !old_ctrl.left) {
                //if(panelSelected!=PANEL_PROGRESS)
                panelSelected--;
            }
            if (ctrl.right && !old_ctrl.right) {
                //if(panelSelected!=PANEL_PROGRESS)
                panelSelected++;
            }

            // clamp ...
            CLAMP(entrySelected, 0, vEntry.size() - 1);
            CLAMP(actionSelected, 0, nbAction - 1);
            CLAMP(panelSelected, 0, 1);


            // Save the old value
            old_ctrl = ctrl;

            // Erase for the big loop
            memset(&ctrl, 0, sizeof (struct controller_data_s));
        }


    }

    void Browser::InitActionEntry() {

    }
    static char bTmp[256];

    void Browser::Init(const char *pRessourcePath) {
        usb_init();

        InitActionEntry();
#ifdef WIN32		
        sprintf(bTmp, "%s%s", pRessourcePath, "\\bg.png");
#else
        sprintf(bTmp, "%s%s", pRessourcePath, "/bg.png");
#endif
        Create(pRessourcePath, 0xFF0000FF, 0xFFFFFF00);
        LoadTextureFromFile(g_pVideoDevice, bTmp, &bg);

    };
}
