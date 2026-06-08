#include <gtk/gtk.h>
#include <gtk4-layer-shell.h>
#include <time.h>

#define BAR_HEIGHT 38

typedef struct {
    GtkLabel* clock_label;
} AppState;

static gboolean
update_clock(gpointer user_data)
{
    AppState* state = user_data;

    time_t now = time(NULL);
    struct tm* local = localtime(&now);

    char buffer[64];
    strftime(buffer, sizeof(buffer), "%H:%M", local);

    gtk_label_set_text(state->clock_label, buffer);

    return G_SOURCE_CONTINUE;
}

static void
on_apps_clicked(GtkButton* button, gpointer user_data)
{
    g_print("Apps button clicked\n");
}

static void
load_css(void)
{
    GtkCssProvider* provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(provider, "style/bar.css");

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(provider);
}

static void
activate(GtkApplication* app, gpointer user_data)
{
    GtkBuilder* builder;
    GtkWindow* window;
    GtkButton* apps_button;
    GtkLabel* clock_label;
    GtkLabel* wifi_label;
    GtkLabel* volume_label;
    GtkLabel* battery_label;
    GtkLabel* etc_label;

    builder = gtk_builder_new_from_file("ui/bar.ui");

    window = GTK_WINDOW(gtk_builder_get_object(builder, "bar_window"));
    apps_button = GTK_BUTTON(gtk_builder_get_object(builder, "apps_button"));
    clock_label = GTK_LABEL(gtk_builder_get_object(builder, "clock_label"));
    wifi_label = GTK_LABEL(gtk_builder_get_object(builder, "wifi_label"));
    volume_label = GTK_LABEL(gtk_builder_get_object(builder, "volume_label"));
    battery_label = GTK_LABEL(gtk_builder_get_object(builder, "battery_label"));
    etc_label = GTK_LABEL(gtk_builder_get_object(builder, "etc_label"));

    if (!window || !apps_button || !clock_label ||
        !wifi_label || !volume_label || !battery_label || !etc_label) {
        g_printerr("Could not load required widgets from ui/bar.ui\n");
        g_object_unref(builder);
        return;
    }

    gtk_window_set_application(window, app);

    /*
     * Turn this normal GTK window into a labwc/wlroots layer-shell bar.
     */
    gtk_layer_init_for_window(window);

    gtk_layer_set_layer(window, GTK_LAYER_SHELL_LAYER_TOP);

    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_TOP, TRUE);
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);

    /*
     * Reserve vertical space so normal windows do not overlap it.
     */
    gtk_layer_set_exclusive_zone(window, BAR_HEIGHT);

    /*
     * A bar usually should not take keyboard focus.
     */
    gtk_layer_set_keyboard_mode(window, GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);

    gtk_widget_add_css_class(GTK_WIDGET(apps_button), "module");
    gtk_widget_add_css_class(GTK_WIDGET(apps_button), "launcher");

    gtk_widget_add_css_class(GTK_WIDGET(clock_label), "module");
    gtk_widget_add_css_class(GTK_WIDGET(wifi_label), "module");
    gtk_widget_add_css_class(GTK_WIDGET(volume_label), "module");
    gtk_widget_add_css_class(GTK_WIDGET(battery_label), "module");
    gtk_widget_add_css_class(GTK_WIDGET(etc_label), "module");

    AppState* state = g_new0(AppState, 1);
    state->clock_label = clock_label;

    update_clock(state);
    g_timeout_add_seconds(1, update_clock, state);

    g_signal_connect(apps_button, "clicked", G_CALLBACK(on_apps_clicked), NULL);

    load_css();

    gtk_window_present(window);

    g_object_unref(builder);
}

int
main(int argc, char** argv)
{
    GtkApplication* app;
    int status;

    app = gtk_application_new("com.ardentdust.Bar",
        G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}