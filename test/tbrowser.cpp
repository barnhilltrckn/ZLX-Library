#include "Browser.h"
#include "elf/elf.h"


ZLX::Browser MyGui;

void BrowseriiAction(void *param) {
    printf("BrowseriiAction clicked\r\n");
    lpBrowserActionEntry _this = (lpBrowserActionEntry) param;
    _this->name = "Prout..";
}

BOOL FileIsElf(const char * filename) {
    BOOL ret = FALSE;
    FILE * f = fopen(filename, "rb");
    if (f) {
        // get the file size
        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        if (size < 0x10) {

        } else {
            void * header = malloc(0x10);
            fseek(f, 0, SEEK_SET);
            fread(header, 1, 0x10, f);

            unsigned int elf_header = 0x7F454C46;
            if (memcmp(&elf_header, header, 4) == 0) {
                ret = TRUE;
            }
            free(header);
        }
    }
    fclose(f);
    printf("FileIsElf:%s\r\n", (ret == TRUE) ? "true" : "false");
    return ret;
}

void ActionLaunchElf(char * filename) {
    char * elf = NULL;
    // read file by chunk
    FILE * f = fopen(filename, "rb");
    int size = 0;
    int chunk = 65536;
    float pct = 0.0f;

    if (f) {
        // get the file size
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        // 0
        fseek(f, 0, SEEK_SET);

        if (size == 0)
            return;


        elf = (char*) malloc(size);

        for (int i = 0; i < size; i += fread(elf + i, 1, chunk, f)) {
            pct = ((float) i / (float) (size));
            MyGui.SetProgressValue(pct);
        }
        fclose(f);

        // read elf header ...
        unsigned int elf_header = 0x7F454C46;
        if (memcmp(&elf_header, elf, 4) == 0) {
            elf_runFromMemory(elf, size);
        } else {

            MyGui.Alert("Not a valid elf !!");
        }

    }
    fclose(f);
}

int main() {


    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Browserii";
        action->action = BrowseriiAction;
        action->param = NULL;
        MyGui.AddAction(action);
    }
    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Browserii2";
        action->action = NULL;
        action->param = NULL;
        MyGui.AddAction(action);
    }

    MyGui.SetLaunchAction(ActionLaunchElf);
    MyGui.Run("uda://ressources/");
}