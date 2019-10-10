#include <iostream>
#include "MicrophoneIcon.hpp"

using namespace std;

MicrophoneIcon::MicrophoneIcon() {
  icon = NULL;
  alsaMic = new AlsaMic();
  alsaMic->isCapturing();
}

MicrophoneIcon::~MicrophoneIcon() {
  if (alsaMic) {
    delete alsaMic;
  }
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

AlsaMic* MicrophoneIcon::getMic() {
  return alsaMic;
}

void MicrophoneIcon::activateStatusIcon(GtkStatusIcon* icon, gpointer userData) {
  auto microphoneIcon = (MicrophoneIcon*)userData;
  microphoneIcon->getMic()->toggle();
}
