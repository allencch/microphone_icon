#include <iostream>
#include "MicrophoneIcon.hpp"

using namespace std;

MicrophoneIcon::MicrophoneIcon() {
  icon = NULL;
  alsaMic = new AlsaMic();
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
  gtk_window_set_title(GTK_WINDOW(window), "Microphone Icon");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  ((MicrophoneIcon*)userData)->setupStatusIcon();
}

void MicrophoneIcon::setupStatusIcon() {
  bool capturing = alsaMic->isCapturing();
  icon = gtk_status_icon_new_from_file(getIcon(capturing).c_str());
  g_signal_connect(icon, "activate", G_CALLBACK(MicrophoneIcon::activateStatusIcon), this);
}

AlsaMic* MicrophoneIcon::getMic() {
  return alsaMic;
}

void MicrophoneIcon::activateStatusIcon(GtkStatusIcon* icon, gpointer userData) {
  auto microphoneIcon = (MicrophoneIcon*)userData;
  microphoneIcon->getMic()->toggle();
}

string MicrophoneIcon::getIcon(bool on) {
  if (on) {
    return "../images/microhpone-on.png";
  }
  return "../images/microphone-off.png";
}
