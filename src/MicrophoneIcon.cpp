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

bool MicrophoneIcon::getCapturingStatus() {
  return isCapturing;
}

void MicrophoneIcon::setCapturingStatus(bool capturing) {
  isCapturing = capturing;
}

void MicrophoneIcon::updateStatusIcon() {
  gtk_status_icon_set_from_file(icon, getIcon(isCapturing).c_str());
}

gboolean MicrophoneIcon::detectCapturing(gpointer userData) {
  auto microphoneIcon = (MicrophoneIcon*)userData;
  bool capturing = microphoneIcon->getMic()->isCapturing();
  if (capturing != microphoneIcon->getCapturingStatus()) {
    microphoneIcon->setCapturingStatus(capturing);
    microphoneIcon->updateStatusIcon();
  }
  return 1;
}

void MicrophoneIcon::activate(GtkApplication* app, gpointer userData) {
  GtkWidget* window;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Microphone Icon");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
  gtk_widget_show_all(window);

  ((MicrophoneIcon*)userData)->setupStatusIcon();
  g_timeout_add(800, MicrophoneIcon::detectCapturing, userData);
}

void MicrophoneIcon::setupStatusIcon() {
  isCapturing = alsaMic->isCapturing();
  icon = gtk_status_icon_new_from_file(getIcon(isCapturing).c_str());
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
    return "../images/microphone-on.png";
  }
  return "../images/microphone-off.png";
}
