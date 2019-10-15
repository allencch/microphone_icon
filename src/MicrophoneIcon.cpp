#include <iostream>
#include "MicrophoneIcon.hpp"

#ifndef DATADIR
#define RESOURCE_DIR "./images"
#else
#define RESOURCE_DIR DATADIR "/microphone_icon"
#endif


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
  updateStatusIcon();
}

void MicrophoneIcon::updateStatusIcon() {
  gtk_status_icon_set_from_file(icon, getIcon(isCapturing).c_str());
}

gboolean MicrophoneIcon::detectCapturing(gpointer userData) {
  auto microphoneIcon = (MicrophoneIcon*)userData;
  microphoneIcon->getMic()->reload();
  bool capturing = microphoneIcon->getMic()->isCapturing();
  if (capturing != microphoneIcon->getCapturingStatus()) {
    microphoneIcon->setCapturingStatus(capturing);
  }
  return 1;
}

void MicrophoneIcon::activate(GtkApplication* app, gpointer userData) {
  GtkWidget* window;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Microphone Icon");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
  // gtk_widget_show_all(window); // Do not show the window

  ((MicrophoneIcon*)userData)->setupStatusIcon();
  ((MicrophoneIcon*)userData)->setupIconMenu();
  g_timeout_add(800, MicrophoneIcon::detectCapturing, userData);
}

void MicrophoneIcon::setupStatusIcon() {
  isCapturing = alsaMic->isCapturing();
  icon = gtk_status_icon_new_from_file(getIcon(isCapturing).c_str());
  g_signal_connect(icon, "activate", G_CALLBACK(MicrophoneIcon::activateStatusIcon), this);
  g_signal_connect(icon, "popup-menu", G_CALLBACK(MicrophoneIcon::rightClickStatusIcon), this);
}

void MicrophoneIcon::setupIconMenu() {
  menu = gtk_menu_new();
  GtkWidget* item = gtk_menu_item_new_with_label("Exit");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

  g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(MicrophoneIcon::quit), this);
}

void MicrophoneIcon::quit(GtkMenuItem* menuItem, gpointer userData) {
  auto app = ((MicrophoneIcon*)userData)->app;
  g_application_quit(G_APPLICATION(app));
}

AlsaMic* MicrophoneIcon::getMic() {
  return alsaMic;
}

void MicrophoneIcon::activateStatusIcon(GtkStatusIcon* icon, gpointer userData) {
  auto microphoneIcon = (MicrophoneIcon*)userData;
  auto capturing = microphoneIcon->getMic()->toggle();
  microphoneIcon->setCapturingStatus(capturing);
}

void MicrophoneIcon::rightClickStatusIcon(GtkStatusIcon* icon,
                                          guint button,
                                          guint activationTime,
                                          gpointer userData) {
  auto menu = ((MicrophoneIcon*)userData)->menu;
  gtk_widget_show_all(menu);
  gtk_menu_popup(GTK_MENU(menu),
                 NULL,
                 NULL,
                 gtk_status_icon_position_menu,
                 icon,
                 button,
                 activationTime);
}

string MicrophoneIcon::getIcon(bool on) {
  if (on) {
    return string(RESOURCE_DIR) + "/microphone-on.png";
  }
  return string(RESOURCE_DIR) + "/microphone-off.png";
}
