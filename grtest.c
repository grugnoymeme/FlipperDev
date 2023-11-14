#include <furi.h>
#include <gui/gui.h>
#include <storage/storage.h>
#include <dialogs/dialogs.h>



/* generated by fbt from .png files in images folder */
#include <grtest_icons.h>

#define ZIP_FOLDER ANY_PATH("zips")
#define ZIP_EXTENSION ".zip"
#define ZIP_FILETYPE "Zip Archive"

/** Main app instance */
typedef struct {
    Gui* gui;
    ViewPort* view_port;
    FuriMutex** mutex;
} UnzipApp;

/**
 * Allocate the unzip app
 * @returns an unzip app instance
 */
static UnzipApp* unzip_app_alloc() {
    UnzipApp* instance = malloc(sizeof(UnzipApp));
    instance->view_port = view_port_alloc();
    //view_port_draw_callback_set(instance->view_port, render_callback, instance);
    //view_port_input_callback_set(instance->view_port, input_callback, instance);
    instance->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(instance->gui, instance->view_port, GuiLayerFullscreen);
    instance->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    return instance;
}

/**
 * Free the unzip app
 * @param unzip_app The app to free
 */
static void unzip_app_free(UnzipApp* instance) {
    gui_remove_view_port(instance->gui, instance->view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(instance->view_port);
    furi_mutex_free(instance->mutex);
    free(instance);
}

int32_t grtest_app(void* p) {
    UNUSED(p);
    FURI_LOG_I("TEST", "Hello world");
    FURI_LOG_I("TEST", "I'm grtest!");

    UnzipApp* instance = unzip_app_alloc();
    FuriString* file_path = furi_string_alloc();

    furi_string_set(file_path, ZIP_FOLDER);
    DialogsFileBrowserOptions browser_options;
    dialog_file_browser_set_basic_options(
            &browser_options, ZIP_EXTENSION, &I_zip_file_10px);
    browser_options.hide_ext = true;
    browser_options.base_path = ZIP_FOLDER;

    DialogsApp* dialogs = furi_record_open(RECORD_DIALOGS);
    bool res = dialog_file_browser_show(dialogs, file_path, file_path, &browser_options);
    UNUSED(res);

    furi_record_close(RECORD_DIALOGS);
    furi_string_free(file_path);
    unzip_app_free(instance);
    return 0;
}
