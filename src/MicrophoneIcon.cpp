#include <iostream>
#include "Alsa.hpp"
#include "MicrophoneIcon.hpp"

using namespace std;

snd_mixer_selem_regopt selem_regopt = {
  .ver = 1,
  .abstract = SND_MIXER_SABSTRACT_NONE,
  .device = "default",
};

MicrophoneIcon::MicrophoneIcon() {
  icon = NULL;
  mixer = NULL;
  setupMic();
}

MicrophoneIcon::~MicrophoneIcon() {
  if (mixer) {
    snd_mixer_free(mixer);
    snd_mixer_close(mixer);
  }
  mixer_shutdown();
}

int MicrophoneIcon::run(int argc, char** argv) {
  GtkApplication* app;
  app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(MicrophoneIcon::activate), this);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

void MicrophoneIcon::activate(GtkApplication* app, gpointer userData) {
  GtkWidget* window;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Test");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
  gtk_widget_show_all(window);

  ((MicrophoneIcon*)userData)->setupStatusIcon();
}

void MicrophoneIcon::setupStatusIcon() {
  icon = gtk_status_icon_new_from_file("../images/microphone-on.png");
  g_signal_connect(icon, "activate", G_CALLBACK(MicrophoneIcon::activateStatusIcon), this);
}

void MicrophoneIcon::activateStatusIcon(GtkStatusIcon* icon, gpointer userData) {
  cout << "here" << endl;
}


void MicrophoneIcon::setupMic() {
  create_mixer_object(&selem_regopt);

  snd_mixer_t* mixer = get_mixer();
  char name[] = "Capture";
  snd_mixer_elem_t* elem = get_mixer_elem_by_name(mixer, name);

  bool capturing = is_capturing(elem);
  cout << capturing << endl;
}
